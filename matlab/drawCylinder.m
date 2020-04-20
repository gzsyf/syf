function drawCylinder(varargin)
% drow a cylinder
% Parameters:
%   * x: central point x
%   * y: central point y
%	* R: radius
%	* h: height
%   * m: Split number
% Return: None
% Example:
%		>> drawCylinder([3,3,1,10,100])
% Version 1.0, Data 2020.4.2,  Author: syf


inArgs = {[3,3,1,10,100]};
inArgs(1:nargin) = varargin;
[x1,y1,R,h,m] = deal(inArgs{:});

[x,y,z]=cylinder(R,m);
x = x + x1;
y = y + y1;
z=h*z;
mesh(x,y,z)
hold on
theta = linspace(0,2*pi,40);       
X=R*cos(theta)+x1;                 
Y=R*sin(theta)+y1;                     
Z=repelem(h,40);                 
fill3(X,Y,Z,[0,0,1]);         

