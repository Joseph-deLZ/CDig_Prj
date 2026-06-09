ControleEEcanalI

Lc=.1;Ki=.1; % s? para estar definido da 1a vez

g=tf([5 6 7],[1 2 3 4])

% Forma Canonica Controlavel
A=[0 1 0;0 0 1;-4 -3 -2];B=[0;0;1];C=[7 6 5];D=0;

%Polos MF - controlador e observador
p=-1; o=10*p;

K=acker(A,B,[p,-.6-1.02i,-.6+1.02i])
L=acker(A',C',[o,o,o]);L=L'

ssMF=ss(A-B*K,B,C,D);gf=tf(ssMF);
Nb=gf.den{1}(1,4)/gf.num{1}(1,4)

sim('ControleEEcanalI')

figure(1),clf
plot(y.time,y.signals.values); hold on
disp('Simule PI. Depois mude chaves: Canal PI; ">>dbcont"') 
keyboard   % p/ mudar a posi??o das chaves na simulacao

%Projeto 2 - Canal Integral - Sistema Aumentado
%Aa=[1 -C;      % xi
%    0 A(1,:);
%    0 A(2,:);
%    0 A(3,:)]
%Ba=[0;B];
%Ca=[0 C];

Aa=[A(1,:) 0
    A(2,:) 0
    A(3,:) 0
    -C     0]  %xi
Ba=[B; 0];
Ca=[C  0];

%Ka=acker(Aa,Ba,[p,p,p,10*p])
Ka=acker(Aa,Ba,[p,-.6-1.0198i,-.6+1.0198i,1*p])

%Ki=-Ka(1),K=Ka(2:4),
Ki=-Ka(4),K=Ka(1:3),

L=acker(A',C',[o,o,o]);L=L'

Nb=-Ki/(1*p);   % Zero canal PI Cancela polo 
%Nb=0;

sim('ControleEEcanalI')
figure(1),plot(y.time,y.signals.values(:,2),'b');
disp('Simule. Depois mude chaves: Obs Pert.; ">>dbcont"') 
keyboard
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Observador de Perturba??es constantes por partes
%Modelo de perturba??es constantes:

K=acker(A,B,[p,-.6-1.0198i,-.6+1.0198i])

ssMF=ss(A-B*K,B,C,D); gf=tf(ssMF);
Nb=gf.den{1}(1,4)/gf.num{1}(1,4);
 
Ac=0; Cc=1; 
Aa=[A     B;
    0 0 0 Ac];
Ba=[B;0]; Ca=[C 0];

L=acker(Aa',Ca',[o,o,o,o]);L=L';
%L=acker(Aa',Ca',[o,o,o,o]);L=L';
Lc=L(4);

sim('ControleEEcanalI')
figure(1),plot(y.time,y.signals.values(:,2),'k');
legend('r','yEE','yEE_PI','yEE_Obs')
dir
