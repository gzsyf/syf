clc;
clear all;
format long 

eta = 0.5;beta = 1e-3;
P_0 = 10;UAV_h = 1;
number=100;d=0;  
node{1}.num=0;    
node{1}.near_id = zeros(1,number);
node{1}.x=0; node{1}.y=0; 
node{1}.E=5;node{1}.Qk=0;   
node{1}.QERj=0;node{1}.gather=0;  
node{1}.recieve=zeros(1,1);  
node{1}.data=zeros(number,2);  
E_da=5*10^-9;E_elec=50*10^-6;   
E_fs=10*10^-9;E_mp=0.0013*10^-9;  
d0=sqrt(E_fs/E_mp);n=400;             
length=100;  
width=100;R=20;  



Mj = 20e-3;aj = 6400;
bj = 0.003;Yj = 1/(1+exp(aj*bj)); 


for i = 1:number
node{i}.x = randi([0,100],1);
node{i}.y = randi([0,100],1);
node{i}.num = 0;   
node{i}.near_id = zeros(1,number);
node{i}.E = 5;   
node{i}.Qk = 0;   
node{i}.QERj = 0;   
end


for i = 1:number
    plot(node{i}.x,node{i}.y,'ro-'),hold on; 
end
Qk = 0;
sum_k= 0;Qk_x = 0;
Qk_y = 0;Qj = 0;
sum_j = 0;Qj_x = 0;
Qj_y = 0;a =0;
for UAV_x = 0:1:100
for UAV_y = 0:1:100
for i = 1:number
x = node{i}.x;
y = node{i}.y;
format long 
node{i}.Qk = (eta*beta*P_0)/((x-UAV_x)^2+(y-UAV_y)^2+UAV_h^2);
node{i}.QERj = (Mj/(1+exp(-aj*(node{i}.Qk-bj)))-Mj*Yj)/(1-Yj);
sum_k = sum_k + node{i}.Qk;    
sum_j = sum_j + node{i}.QERj;  
end
if sum_k > Qk
Qk = sum_k;
sum_k =0;
Qk_x = UAV_x;
Qk_y = UAV_y;
end
if sum_j >  Qj
Qj = sum_j;
sum_j =0;
Qj_x = UAV_x;
Qj_y = UAV_y;
end
end
end
plot(Qk_x,Qk_y,'g*-'),hold on;
text(Qk_x,Qk_y,'best linear')  
plot(Qj_x,Qj_y,'r*-'),hold on;
text(Qj_x,Qj_y,'best unlinear')  

figure(1)
set(gca,'ytick',0:2e-2:3e-1);
set(gcf,'name','best position')
title('best engry position')
ylabel('total engry')
xlabel('module')
text(5,Qk+0.01,'linear'),hold on
text(10,Qj+0.01,'unlinear'),hold on 
bar(5, Qk),hold on;
bar(10,Qj)

figure(2)
for i =1:number
axis([0 110 -1e-4 1e-3]);
set(gca,'xtick',0:10:110);
set(gca,'ytick',-1e-4:1e-4:1e-3);
set(gcf,'name','linear engry');
bar(i, node{i}.Qk,'r'),hold on;
end

figure(3)
for i =1:number
axis([0 110 -1e-10 1e-9]);
set(gca,'xtick',0:10:110);
set(gca,'ytick',-1e-10:1e-10:1e-9);
set(gcf,'name','unlinear engry');
bar(i, node{i}.QERj,'r'),hold on;
end
Qk_x,Qk_y
Qj_x,Qj_y
