function drawCuboid(varargin)
% display a 3D-cube in the current axes
% Parameters:
%	* point1: the left bottom point of cuboid 
%	* point2: the right top point of cuboid 
% Return: None
% Example:
%		>> drawCuboid([0 0 0],[4 4 10])
% Version 1.0, Data 2020.4.2,  Author: syf

% Default input arguments 
inArgs = { ...
  [0 0 0 ] , ...  % Default edge sizes (x,y and z)
  [4 4 10 ] , ... % Default coordinates of the origin point of the cube
  50};
% Replace default input arguments by input values
inArgs(1:nargin) = varargin;
% Create  variables
[point1,point2, m] = deal(inArgs{:});

Block=cell(1);                      

Block{1}.x1 = point1(1);Block{1}.x2=point2(1);
Block{1}.y1 = point1(2);Block{1}.y2=point2(2);
Block{1}.z1 = point1(3);Block{1}.z2=point2(3);

X=linspace(Block{1}.x2,Block{1}.x2);
Y=linspace(Block{1}.y1,Block{1}.y2);
for k=Block{1}.z1:(Block{1}.z2-Block{1}.z1)/m:Block{1}.z2
    Z=linspace(k,k);
    plot3(X,Y,Z,'b')
    hold on
end
X=linspace(Block{1}.x1,Block{1}.x1);
Y=linspace(Block{1}.y1,Block{1}.y2);
for k=Block{1}.z1:(Block{1}.z2-Block{1}.z1)/m:Block{1}.z2
    Z=linspace(k,k);
    plot3(X,Y,Z,'b')
    hold on
end
X=linspace(Block{1}.x1,Block{1}.x2);
Y=linspace(Block{1}.y1,Block{1}.y1);
for k=Block{1}.z1:(Block{1}.z2-Block{1}.z1)/m:Block{1}.z2
    Z=linspace(k,k);
    plot3(X,Y,Z,'b')
    hold on
end
X=linspace(Block{1}.x1,Block{1}.x2);
Y=linspace(Block{1}.y2,Block{1}.y2);
for k=Block{1}.z1:(Block{1}.z2-Block{1}.z1)/m:Block{1}.z2
    Z=linspace(k,k);
    plot3(X,Y,Z,'b')
    hold on
end
X = [point1(1); point1(1); point2(1);point2(1)];
Y = [point1(2); point2(2); point2(2);point1(2)];
Z = [point2(3); point2(3); point2(3); point2(3)];
fill3(X,Y,Z,[0,0,1])

