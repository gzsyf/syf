%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Script: FFT test
% Include : fft()
% Author: syf
% Date  : 2019-6-17 
% Introduction : 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Step 0: Clear Memory & Command Window
clc;
clear all;
close all;

%% Step 1:define the Sampling frequency and Sampling points
Fs = 1000;   % Sampling frequency
             % more high that Sampling accurate
             % Fs/2 > mas_frequency(fun)!
L=2000;   % Sampling points
          % more high that Sampling more long

%% Step 2: define object signal
T=1/Fs;
t=(0:L-1)*T;  % Sampling sequence (up to Fs and L)
x=sin(10.*t); % object signal

%% Step 3: FFT (Enhanced version of discrete Fourier transform)
y=fft(x);

%% Step 3: FFT (Enhanced version of discrete Fourier transform)
f = Fs*(0:(L-1))/L;
subplot(1,2,1)
plot(f,abs(y));
subplot(1,2,2)
plot(t,x); 

%% Reference :
% [1] w and f relationship
% https://www.zybang.com/question/7ad79840c48615b6308c8c0136b04d3b.html
% [2] come feom
% http://www.360doc.com/content/19/1205/21/60231779_877705216.shtml

