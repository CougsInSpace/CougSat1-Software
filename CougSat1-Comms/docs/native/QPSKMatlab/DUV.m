%% Reset the current setup
clc;
clear all;
close all;

%% Settings
modIndex = 0.8;
dataStrength = 0.5;
carrierF = 12000;
baud = 1000;
bandWidth = baud * 3;
snr = 3;
audioLowpass = 5000;
dataLowpass = baud; % Second order low pass

%% Generate baseband FM from audio
% Read audio file
[audio, sampleF] = audioread("sorry_dave.wav");
samples = length(audio);
audio = audio / max(abs(audio)); % Normalize to +/- 1

% Generate data
numSym = ceil(samples / sampleF * baud);
dataI  = randi([0 1], numSym, 1) * 2 - 1;
dataQ  = randi([0 1], numSym, 1) * 2 - 1;
basebandI     = zeros(samples, 1);
basebandQ     = zeros(samples, 1);
trapezoidRate = 2 * bandWidth / sampleF;
for i = 2 : samples
  t = i / sampleF;

  basebandI(i) = basebandI(i - 1) + trapezoidRate * dataI(ceil(t * baud)); 
  basebandQ(i) = basebandQ(i - 1) + trapezoidRate * dataQ(ceil(t * baud));

  basebandI(i) = max(-1, min(1, basebandI(i)));
  basebandQ(i) = max(-1, min(1, basebandQ(i)));
end

% Shift the data up to the subcarrier
dataBaseband = zeros(samples, 1);
for i = 1 : samples
    t = i / sampleF;
    I = cos(2 * pi * t * carrierF) * basebandI(i);
    Q = sin(2 * pi * t * carrierF) * basebandQ(i);
    dataBaseband(i) = I + Q;
end

% Add the two together
fmBaseband = audio * (1 - dataStrength) + dataBaseband * dataStrength;

%% Modulate audio into the FM
phase = zeros(1, samples);
for i = 2 : samples
    phase(i) = fmBaseband(i) + phase(i-1);
end
radioI = cos(phase * modIndex);
radioQ = sin(phase * modIndex);
for i = 1 : samples
    radioI(i) = radioI(i) + randn() * 10 ^(-snr / 10);
    radioQ(i) = radioQ(i) + randn() * 10 ^(-snr / 10);
end
amplitude = max(max(abs(radioI)), max(abs(radioQ))); % normalize to +/- 1
radioI = radioI / amplitude;
radioQ = radioQ / amplitude;
combined = transpose([radioI; radioQ]);
audiowrite("IQOut.wav", combined, sampleF);

%% Demodulate FM
fmBasebandRX = zeros(samples, 1);

for i = 2 : samples
  t = i / sampleF;
  di = radioI(i) - radioI(i - 1);
  dq = radioQ(i) - radioQ(i - 1);
  fmBasebandRX(i) = radioI(i) * dq - radioQ(i) * di;
end
fmBasebandRX = fmBasebandRX / max(abs(fmBasebandRX));

%% Extract audio band
[filterB, filterA] = butter(6, audioLowpass / (sampleF / 2), "low");
audioRX = filter(filterB, filterA, fmBasebandRX);

%% Extract data band
% Filter around the carrier
[filterB, filterA] = butter(6, [(carrierF - bandWidth) (carrierF + bandWidth)] / (sampleF / 2));
dataRX = filter(filterB, filterA, fmBasebandRX);

dataRXI = zeros(1, samples);
dataRXQ = zeros(1, samples);
demodI = zeros(1, samples);
demodQ = zeros(1, samples);

w = tan(dataLowpass / sampleF * pi);
d = 1 + w ^ 2 + w * sqrt(2);
n = w ^ 2 / d;
d1 = (2 * w ^ 2 - 2) / d;
d2 = (w ^ 2 + 1 - w * sqrt(2)) / d;

phi = zeros(samples, 1);
error = zeros(samples, 1);
for i = 3 : samples
  t = i / sampleF;
  if i > 3
    error(i) = sign(dataRXI(i - 1)) * dataRXQ(i - 1) - sign(dataRXQ(i - 1)) * dataRXI(i - 1);
    error(i) = error(i) / sqrt(2) / sqrt(dataRXI(i - 1) ^ 2 + dataRXQ(i - 1) ^ 2);
    % error(i) = max(-0.0005, min(error(i), 0.0005));
    % TODO implement a filter with a band reject around the baud rate
    %phi(i) = phi(i - 1) - error(i);
  end
  
  demodI(i) = dataRX(i) * cos(2 * pi * carrierF * t + phi(i));
  demodQ(i) = dataRX(i) * sin(2 * pi * carrierF * t + phi(i));  
  
  % Apply filter
  dataRXI(i) = n * demodI(i) + 2 * n * demodI(i - 1) + n * demodI(i) - d1 * dataRXI(i - 1) - d2 * dataRXI(i - 2);
  dataRXQ(i) = n * demodQ(i) + 2 * n * demodQ(i - 1) + n * demodQ(i) - d1 * dataRXQ(i - 1) - d2 * dataRXQ(i - 2);
end
amplitude = max(max(abs(dataRXI)), max(abs(dataRXQ)));
dataRXI = dataRXI / amplitude;
dataRXQ = dataRXQ / amplitude;
%[filterB, filterA] = butter(6, baud / (sampleF / 2), "low");
%dataRXI = filter(filterB, filterA, dataRXI);
%dataRXQ = filter(filterB, filterA, dataRXQ);

%% Plot the information
dF = sampleF / samples;
f = -sampleF/2:dF:sampleF/2-dF;

figure;
subplot(2, 3, 1);
plot(audio);
ylim([-1.2 1.2]);
title("Audio in");
subplot(2, 3, 2);
plot(basebandI, basebandQ);
axis([-1.2 1.2 -1.2 1.2]);
title("Data in");
subplot(2, 3, 3);
plot(f, abs(fftshift(fft(fmBaseband)))/samples);
xlim([0, sampleF / 2]);
title("Baseband TX");
subplot(2, 3, 4);
plot(f, abs(fftshift(fft(fmBasebandRX)))/samples);
xlim([0, sampleF / 2]);
title("Baseband RX");
subplot(2, 3, 5);
plot(f, abs(fftshift(fft(audioRX)))/samples);
xlim([0, audioLowpass * 2]);
title("Audio RX");
subplot(2, 3, 6);
plot(dataRXI, dataRXQ);
axis([-1.2 1.2 -1.2 1.2]);
title("Data RX");

figure;
plot(f, abs(fftshift(fft(error)))/samples);
xlim([0, sampleF / 2]);


%soundsc(audioRX, sampleF);