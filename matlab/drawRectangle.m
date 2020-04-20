function drawRectangle(point_min,point_max)
% description: two point draw a rectangle 
% Parameters:
%	* point_min: the point of Lower left corner
%	* point_max: the point of Upper right corner
% Return: None
% Example:
%		>> drawRectangle([1 1],[2 2]);


%% step1: get the position and size
% Description: ax ay is position of Lower left corner; l w is rectangle
%              size
% Attention:
ax = point_min(1);
ay = point_min(2);
l = point_max(1) - point_min(1);
w = point_max(2) - point_min(2);

%% step2: list all the point according to ax; ay; l; w
% Description: 
% Attention:
x = [ax,ax+l,ax+l,ax,ax];
y = [ay,ay,ay+w,ay+w,ay];

%% step1: draw picture 
% Description: in order to easy to look shrink the axis  
% Attention:
line(x,y)
axis([ax-l,ax+2*l,ay-w,ay+2*w]) %shrink the axis  