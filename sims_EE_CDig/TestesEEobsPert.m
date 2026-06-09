ControleEE

g=tf([5 6 7],[1 2 3 4])
figure(1),pzmap(g)

gss=ss(g)
% Forma Can?nica Control?vel
A=[0 1 0;0 0 1;-4 -3 -2];B=[0;0;1];C=[7 6 5];D=0;

%Projeto 1
p=-3; o=3*p; %o=10*p;

%p=-3; o=3*p;K=acker(A,B,[p,p,p])
K=acker(A,B,[p,-.6-1.0198i,-.6+1.0198i])

%K=acker(A,B,[p,p,p])
L=acker(A',C',[o,o,o]);L=L'

ssMF=ss(A-B*K,B,C,D)
gf=tf(ssMF)
Nb=gf.den{1}(1,4)/gf.num{1}(1,4)

figure(2); step(g,Nb*gf)

ControleEE
sim('ControleEE')
%return

%Observador de Perturba??es constantes por partes
%Modelo de perturbacoes constantes:
w0=.5;

Ac=0; Cc=1; 

Aa=[A B*Cc;
    0 0 0 Ac];
Ba=[B;0]; Ca=[C 0];

L=acker(Aa',Ca',[o,o,o,o]);L=L'
Lc=L(4);

%return
%Perturbacoes Ctes + Senoidais
As=[0 1;
    -w0^2 0];
Cs=[1 0];

Aa=[A     B*Cc B*Cs;
    0 0 0  Ac   0 0;
    [0 0 0 0;0 0 0 0]   As];
Ba=[B;0;0;0]; Ca=[C 0 0 0];

L=acker(Aa',Ca',[o,o,o,.3*o,.3*o,.3*o]);L=L'
Lc=L(4);Ls1=L(5);Ls2=L(6);

ControleEE_ObsPert
sim('ControleEE_ObsPert')