% PrjObsPertContinuoRed

g=tf([1],[1 1])
gss=ss(g)
% Forma Can?nica Control?vel
A=[-1];B=[1];C=[1];D=0;

%Projeto 1
p=-2; o=.1*p;

%Controlador
K=acker(A,B,p)

%Observador
L=acker(A',C',o);L=L'

ssMF=ss(A-B*K,B,C,D)
gf=tf(ssMF)
Nb=gf.den{1}(1,2)/gf.num{1}(1,2)

figure(2); step(g,Nb*gf)

% Observador de Perturbacoes constantes por partes 
% rampa e senoide
w0=.5;

% rampa engloba observador de degraus
Ac=0; Cc=1; 

Ar = [0 1;0 0];  Cr=[1 0 ]; 

% Aa=[A B*Cc;
%       0 0 0 Ac];
% Ba=[B;0]; Ca=[C 0];
% 
% Aa=[A B*Cr;
%       [0 0 0; 0 0 0]; Ar];
% Ba=[B;0]; Ca=[C 0];
% 
% L=acker(Aa',Ca',[o,o,o,o]);L=L'
% Lc=L(4);

%return
%Perturbacoes Ctes + Senoidais
As=[0 1;
    -w0^2 0];
Cs=[1 0];


% Aa=[A     B*Cc B*Cs;
%     0 0 0  Ac   0 0;
%     [0 0 0 0;0 0 0 0]   As];
% Ba=[B;0;0;0]; Ca=[C 0 0 0];
% 
% L=acker(Aa',Ca',[o,o,o,.3*o,.3*o,.3*o]);L=L'
% Lc=L(4);Ls1=L(5);Ls2=L(6);

Ar = [0 0;1 0];  Cr=[0 1]; 

As=[0 -w0^2;
       1     0 ];
Cs=[0 1];

Aa=[A     B*Cr B*Cs;
    [0; 0]   Ar   [0 0; 0 0];
    [0; 0]   [0 0; 0 0] As];
Ba=[B;0;0;0;0]; Ca=[C 0 0 0 0];

L=acker(Aa',Ca',[3*p,o,o,o,o]);L=L'
%L(2:5)=0;
Lp=L(1);Lr1=L(2);Lr2=L(3);Ls1=L(4);Ls2=L(5);
Lp=Lp;

ObsPertContinuoRed
sim('ObsPertContinuoRed')