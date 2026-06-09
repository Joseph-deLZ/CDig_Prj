ControleEE

g=tf([5 6 7],[1 2 3 4])
figure(1),pzmap(g)

gss=ss(g)
% Forma Can?nica Control?vel
A=[0 1 0;0 0 1;-4 -3 -2];B=[0;0;1];C=[7 6 5];D=0;

%Projeto 1
p=-3; o=3*p;

K=acker(A,B,[p,-.6-1.02i,-.6+1.02i])
K=acker(A,B,[p,p,p])
L=acker(A',C',[o,o,o]);L=L'

ssMF=ss(A-B*K,B,C,D)
gf=tf(ssMF)
Nb=gf.den{1}(1,4)/gf.num{1}(1,4)

figure(2); step(g,Nb*gf)

sim('ControleEE')