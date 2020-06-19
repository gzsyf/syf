function drawCylinder(varargin)
% drow a cylinder
% Parameters:
%   * x: central point x
%   * y: central point y
%	* R: radius
%	* h1: height first Cylinder
%   * h2: height second Cylinder
%   * m: Split number
% Return: None
% Example:
%		>> drawCylinder(3,3,1,10,30,100)
% Version 1.0, Data 2020.4.2,  Author: syf

%% step1:初始化数据
inArgs = {3,3,1,10,30,100};
inArgs(1:nargin) = varargin;
[x1,y1,R,h1,h2,m] = deal(inArgs{:});
%% step1:画第一个圆柱
[x,y,z]=cylinder(R,m);
x = x + x1;
y = y + y1;
z=h1*z;
Cylinder1 = mesh(x,y,z);
set(Cylinder1,'FaceColor','r')
hold on
%% step1:画第二个圆柱
z2=[repelem(h1 ,length(z));repelem(h2 ,length(z))];
Cylinder2 = mesh(x,y,z2);
set(Cylinder2,'FaceColor','b')
hold on
%% step1:画盖
theta = linspace(0,2*pi,40);       
X=R*cos(theta)+x1;                 
Y=R*sin(theta)+y1;                     
Z=repelem(h2,40);                 
fill3(X,Y,Z,[0,0,1]);         

