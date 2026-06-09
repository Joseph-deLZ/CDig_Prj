
% Processo de n?vel de l?quido de 2a ordem
% UnB-GRAV-LAVSI

% Autor: Adolfo Bauchspiess @ Bras?lia, 11 de junho de 2008

% Parametros do Processo 
Ar=6*25.4;  % cm2 - ?rea da se??o transversal dos tanques  

k12=38; % v?lvula entre tanques no 3o engaste
%k12=28; % v?lvula entre tanques no 2o engaste
%k12=16; % v?lvula entre tanques no 1o engaste

k1=0;    % par?metro do furo do tanque de entrada
k2=11.5; % par?metro do furo do tanque de sa?da

hmax=16;  % altura m?xima antes de sair ?gua pelo ladr?o
qmax=90;  % vaz?o m?xima da bomba +
qmin=-85; % vaz?o m?xima da bomba -

qb=35;    % vaz?o no ponto de opera??o
qb=20;

h2b = (qb/k2)^2;         % n?vel 2 no ponto de opera??o
h1b=(qb/k12)^2 + h2b;    % n?vel 1 no ponto de opera??o
a=k12/(2*Ar*sqrt(h1b-h2b)); % vari?vel auxiliar
b=k2/(2*Ar*sqrt(h2b));      % vari?vel auxiliar

%Representa??o no Espa?o de Estados - Forma Can?nica Control?vel
A=[ 0      1;
   -a*b -2*a-b];
B=[ 0;     1];
C=[ a/Ar   0];


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VALORES PARA A PrCDin214
%qmax=inf;qmin=-inf;hmax=500;
%A=[ 0  1;
%   -.005 -.5];
%B=[ 0;     1];
%C=[ .001   0];

p=-.2;  % Posi??o para os p?los de MF
o=3*p;    % Din?mica do observador
p=-1/5;o=3*p;

K=acker(A,B,[p p])
L=acker(A',C',[o o]);L=L';

ssMF=ss(A-B*K,B,C,[0])
gf=tf(ssMF);

Nb=gf.den{1}(1,3)/gf.num{1}(1,3)  % Fator de ajuste de ganho

%liq2EE
%sim('liq2EE')

% Projeto 2 - Canal PI - Sistema Aumentado

Aa=[A(1,:) 0;
    A(2,:) 0;
     -C    0;]
Ba=[B; 0];
Ca=[C 0];
K=acker(Aa,Ba,[p,p,p])
Ki=-K(3);K1=K(1);K2=K(2);

L=acker(A',C',[o,o]);L=L'
L1=L(1);L2=L(2);

Kp=-Ki/p; 

PrLiq2EE_PI_AntiWindup
sim('PrLiq2EE_PI_AntiWindup')
