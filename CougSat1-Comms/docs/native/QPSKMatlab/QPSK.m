%% Reset the current setup
clc;
clear all;
close all;

%%%%% Generate RF signal from random data %%%%%
%% Generate random data to transmit
% Data is -1 or 1
numSym = 200;
dataI  = randi([0 1], numSym, 1) * 2 - 1;
dataQ  = randi([0 1], numSym, 1) * 2 - 1;

%% Modulation defines
baud      = 250000;         % 250kHz
carrierF  = 435000000;      % 435MHz
RFSampleF = carrierF * 100; % 43.5GHz
duration  = numSym / baud;
RFSamples = round(duration * RFSampleF);

%% Generate baseband signals and filter
% Output is a trapezoidal wave to reduce high frequency harmonics
baseBandI     = zeros(1, RFSamples);
baseBandQ     = zeros(1, RFSamples);
trapezoidF    = 2000000; % 2MHz
trapezoidRate = 2 * trapezoidF / RFSampleF;
for i = 2 : RFSamples
  t = i / RFSampleF;

  baseBandI(i) = baseBandI(i - 1) + trapezoidRate * dataI(ceil(t * baud)); 
  baseBandQ(i) = baseBandQ(i - 1) + trapezoidRate * dataQ(ceil(t * baud));

  baseBandI(i) = max(-1, min(1, baseBandI(i)));
  baseBandQ(i) = max(-1, min(1, baseBandQ(i)));
end

%% Mix baseband with carrier
modPhaseJitter = 0.01; % jitters 1% of 2pi
modPhaseOffset = 0.3; 
signalTX       = zeros(1, RFSamples);
for i = 1 : RFSamples
  t = i / RFSampleF;

  phi = modPhaseOffset + modPhaseJitter * randn() * 2 * pi;

  yI = baseBandI(i) * cos(2 * pi * carrierF * t + phi) * sqrt(2) / 2;
  yQ = baseBandQ(i) * sin(2 * pi * carrierF * t + phi) * sqrt(2) / 2;

  signalTX(i) = yI + yQ;
end

%%%%% Data is transmitted %%%%%
%% Add noise
% randn is a gaussian distribution with std. dev. of 1
snr = 10; % link budget calculated >10dB snr
for i = 1 : RFSamples
    signalTX(i) = signalTX(i) + 10 ^ (-snr / 10) * randn();
end

%% SDR demodulates into I/Q %%%%%
% doppler effects at 7.6km/s = 25ppm, clock stability = 5ppm
demodCarrierF    = carrierF * (1 + 30e-6); % 30ppm off
demodPhaseOffset = 0.4;
demodPhaseJitter = 0.01; % jitters 1% of 2pi
disp("Carrier offset: " + (demodCarrierF - carrierF));

signalRXI = zeros(1, RFSamples);
signalRXQ = zeros(1, RFSamples);
for i = 1 : RFSamples
  t = i / RFSampleF;

  phi = demodPhaseOffset + demodPhaseJitter * randn() * 2 * pi;

  signalRXI(i) = signalTX(i) * cos(2 * pi * demodCarrierF * t + phi);
  signalRXQ(i) = signalTX(i) * sin(2 * pi * demodCarrierF * t + phi);
end

%% Low pass received signal to remove the USB frequencies
[filterB, filterA] = butter(6, carrierF / (RFSampleF / 2), "low");
signalRXI = filter(filterB, filterA, signalRXI);
signalRXQ = filter(filterB, filterA, signalRXQ);

%% Sample mixer output
% Round the samples to the nearest bit value, adds quantization noise
SDRSampleF = 2e6; % 2MHz
samples    = duration * SDRSampleF;
adcBits    = 7;
adcMax     = (2 ^ adcBits) - 1;
sdrRXI     = zeros(1, samples);
sdrRXQ     = zeros(1, samples);
for i = 1 : samples
  t = i * RFSampleF / SDRSampleF;
  sdrRXI(i) = round(signalRXI(t) * adcMax) / adcMax;
  sdrRXQ(i) = round(signalRXQ(t) * adcMax) / adcMax;
end

% Plot I/Q from the SDR, time domain
figure;
subplot(5, 1, 1);
plot(sdrRXI);
title("I from SDR")
ylim([-0.5, 0.5]);
subplot(5, 1, 2);
plot(sdrRXQ);
title("Q from SDR");
ylim([-0.5, 0.5]);

%% Remove carrier (freq and phase) offset using a costas loop
% Symbols may be rotated by 0, 90, 180, or 270. Removed with a known preamble
% Output is integrated (rolling average) to low pass the signal
freqOffset = demodCarrierF - carrierF;
phiOffset  = demodPhaseOffset;
costasI    = zeros(1, samples);
costasQ    = zeros(1, samples);
sdrRXIOut  = zeros(1, samples);
sdrRXQOut  = zeros(1, samples);

integrateSamples = round(SDRSampleF / baud / 2);
disp("Integrating " + integrateSamples + " samples");

phi   = zeros(1, samples);
error = zeros(1, samples);
for i = 1 : samples
  t = i / SDRSampleF;
  if i > 1
    % Take the cross product between symbol vector and ideal symbol vector
    error(i) = sign(costasI(i - 1)) * costasQ(i - 1) - sign(costasQ(i - 1)) * costasI(i - 1);
    error(i) = error(i) / sqrt(2) / sqrt(costasI(i - 1) ^ 2 + costasQ(i - 1) ^ 2);
    phi(i) = phi(i - 1) + error(i);
  end
  % Remove the offset by applying a rotation transformation
  costasI(i) = sdrRXI(i) * cos(phi(i)) + sdrRXQ(i) * sin(phi(i));
  costasQ(i) = sdrRXQ(i) * cos(phi(i)) - sdrRXI(i) * sin(phi(i));
    
  % Take the rolling average to low pass filter the output signal
  start = max(1, i - integrateSamples + 1);
  for j = start : i
    sdrRXIOut(i) = sdrRXIOut(i) + costasI(j);
    sdrRXQOut(i) = sdrRXQOut(i) + costasQ(j);
  end
end
sdrRXIOut = sdrRXIOut./integrateSamples;
sdrRXQOut = sdrRXQOut./integrateSamples;

% Plot the I/Q out, time domain
subplot(5, 1, 3);
plot(sdrRXIOut);
title("Demodulated I");
ylim([-0.5 0.5]);
subplot(5, 1, 4);
plot(sdrRXQOut);
title("Demodulated Q");
ylim([-0.5 0.5]);
subplot(5, 1, 5);
error = error./ (2 * pi / SDRSampleF);
plot(error);
title("Frequency Offset");

%% Sample I/Q every bit period
dataRXISDR = zeros(1, numSym);
dataRXQSDR = zeros(1, numSym);
dataRXI    = zeros(1, numSym);
dataRXQ    = zeros(1, numSym);
for i = 1 : numSym
  t = (i - 0.5) * SDRSampleF / baud;
    
  dataRXISDR(i) = sdrRXI(t);
  dataRXQSDR(i) = sdrRXQ(t);
  dataRXI(i)    = sdrRXIOut(t);
  dataRXQ(i)    = sdrRXQOut(t);
end

% Plot the I/Q in and out, phase domain
figure;
subplot(2,2,1);
plot(sdrRXI, sdrRXQ);
title("I/Q from SDR");
axis([-0.8 0.8 -0.8 0.8]);
subplot(2,2,3);
scatter(dataRXISDR, dataRXQSDR, 10, "filled");
axis([-0.8 0.8 -0.8 0.8]);
title("I/Q from SDR");
subplot(2,2,2);
plot(sdrRXIOut, sdrRXQOut);
title("I/Q demodulated");
axis([-0.8 0.8 -0.8 0.8]);
subplot(2,2,4);
scatter(dataRXI, dataRXQ, 10, "filled");
axis([-0.8 0.8 -0.8 0.8]);
title("I/Q demodulated");