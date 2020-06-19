function drawSphere(varargin)
% display a 3D-sphere in the current axes
% Parameters:
%	* l: the length of sphere
%	* w: the width of sphere
%	* h: the heigth of sphere
% Return: None
% Example:
%		>> drawSphere(1,1,1,3)
% Version 1.0, Data 2020.4.2,  Author: syf

%% Default input arguments 
inArgs = { ...
  1 , ...   % Default the length of sphere
  1, ...    % Default width of sphere
  1, ...
  3};
% Replace default input arguments by input values
inArgs(1:nargin) = varargin;
% Create  variables
[x0,y0,z0, r] = deal(inArgs{:});

%% draw a sphere
[a, b, c]=sphere();
mesh(r * a + x0 , r * b + y0 , r * c + z0 );