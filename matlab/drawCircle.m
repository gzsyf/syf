function drawCircle( x, y, r )
% draw a circile
% Parameters:
%	* x,y: the xy-axis of the center of circle 
%	* r: radius of circle 
% Return: plot a circle 
% Example:
%		>> drawCircle(1,1,1);


theta = 0:2*pi/3600:2*pi;
Circle1 = x + r*cos(theta);
Circle2 = y + r*sin(theta);
plot(Circle1,Circle2,'--k')
axis equal
end