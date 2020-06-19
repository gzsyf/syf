function [RouthTable,Conclusion] = routh(polequ)
% ��˹�о�bai���ϵͳ��du���Ժ���
% ���룺
% polequ = ������������;
% �����zhi
% RouthTable = ��˹��
% Conclusion = ϵͳ��dao���ȶ�����ڶ��ٸ����ȶ��ĸ��Ľ���
% ����:
% [RouthTable,Con] = routh([1 2 3 4 5]);
% RouthTable =
% 1 3 5
% 2 4 0
% 1 5 0
% -6 0 0
% 5 0 0
% Con =
% ���ж�ϵͳ�� 2 �����ȶ���!
%
%
n=length(polequ);
polequ=reshape(polequ,1,n);
if mod(n,2)==0
n1=n/2;
else
n1=(n+1)/2;
polequ=[polequ,0];
end
routh=reshape(polequ,2,n1);
RouthTable=zeros(n,n1);
RouthTable(1:2,:)=routh;
i=3;
while 1;
% =========�������1(��һ��Ϊ0�������в�Ϊ0)=====================
if RouthTable(i-1,1)==0 && sum(RouthTable(i-1,2:n1))~=0
polequ = conv(polequ,[1 3]);
n=length(polequ);
if mod(n,2)==0
n1=n/2;
else
n1=(n+1)/2;
polequ=[polequ,0];
end
routh=reshape(polequ,2,n1);
RouthTable=zeros(n,n1);
RouthTable(1:2,:)=routh;
i=3;
end
% ==========������˹��===========================================
ai=RouthTable(i-2,1)/RouthTable(i-1,1);
for j=1:n1-1
RouthTable(i,j)=RouthTable(i-2,j+1)-ai*RouthTable(i-1,j+1);
end
% ==========�������2(ȫ0��)======================================
if sum(RouthTable(i,:))==0
k=0;
l=1;
F=zeros(1,n1);
while n-i-k>=0
F(l)=n-i+1-k;
k=k+2;
l=l+1;
end
RouthTable(i,:)=RouthTable(i-1,:).*F(1,:);
end
% =========����==================================================
i=i+1;
if i>n
break;
end
end
% =============outhput===========
r=find(RouthTable(:,1)<0);
if isempty(r)==1
Conclusion='��Ҫ�ж�ϵͳ�ȶ�!';
else
n2=length(r);
m=n2;
for i=1:n2-1
if r(i+1)-r(i)==1
m=m-1;
end
end
str1='���ж�ϵͳ�� ';
if r(n2)==n
str2=num2str(m*2-1);
else
str2=num2str(m*2);
end
str3=' �����ȶ���!';
Conclusion = [str1,str2,str3];
end