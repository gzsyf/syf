function drawFFT(Fs,L,fun)
% description....
% Parameters:
%	* Fs: Sampling frequency,
%         more high that Sampling accurate 
%         Fs/2 > mas_frequency(fun)!
%	* L: Sampling points
%        more high that Sampling more long
%   * fun: input a function of char
% Return:
%	* None
% Example:
%		>> drawFFT(1000,2000,'8*sin(10.*t)');

%% Step 1: define object signal
% Description: 
% Attention:
T=1/Fs;
t=(0:L-1)*T;  % Sampling sequence (up to Fs and L)

if ischar(fun) % change str to funcyion
    fun=inline(fun);
end
x = fun(t);

%% Step 2: FFT (Enhanced version of discrete Fourier transform)
% Description: 
% Attention:
y=fft(x);
P = (y).*conj(y);
%% Step 3: FFT (Enhanced version of discrete Fourier transform)
% Description: 
% Attention:
f = Fs*(0:(L-1))/L;
subplot(1,2,1)
plot(f,P);
subplot(1,2,2)
plot(t,x); 

%% Reference :
% [1] w and f relationship
% https://www.zybang.com/question/7ad79840c48615b6308c8c0136b04d3b.html
% [2] come feom
% http://www.360doc.com/content/19/1205/21/60231779_877705216.shtml

