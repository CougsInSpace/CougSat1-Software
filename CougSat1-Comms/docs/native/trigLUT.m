%% Reset the current setup
clc;
clear all;
close all;

%%
n = 360;
x = 0 : (2 * pi) / n : 2 * pi * (n - 1) / n;

factor = 100;
nComp = n * factor;
xComp = 0 : (2 * pi) / nComp : 2 * pi * (nComp - 1) / nComp;

s = sin(x);
sComp = sin(xComp);

xFractional = floor(xComp / (2 * pi) * n);
sFractional = sin(xFractional / n * (2 * pi));

b = 10;

sInt = floor((s + 1) / 2 * (2^(b) - 1) + 0.5);

rsme = sqrt(mean((sComp - sFractional).^2));
disp(rsme);

LUT_SINE = s;
LUT_SINE_QUANTIZED = sInt;

hold on;
plot(x, s);
plot(xComp, sComp);
plot(xComp, sFractional);