%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Script: pointMove.m
% Author: syf
% Date  : 2020-4-1
% Introduction : we can see the point move in the 3d line
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Step 1: drow a 3d line
t=0:0.01:10;    
x=sin(t);
y=cos(t);
z=t;
plot3(x,y,z);
hold on

%% Step 2: set the initial point
h = plot3(sin(0),cos(0),0,'or');

%% Step 3: start to move the point on the line
for t = 0:0.01:10
    x=sin(t);
    y=cos(t);
    z=t;
    set(h,'Xdata',x,'Ydata',y,'Zdata',z);
    pause(0.01);   % ????
end

%% Reference :
% [1] https://zhidao.baidu.com/question/1862144424821976987.html