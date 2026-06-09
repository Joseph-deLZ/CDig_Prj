% Prj1CDig_m
%
% Controle PI Anti-WindUp de Processo Térmico
%
% @AB ENE/UnB Condrole Digital

% resposta do processo térmico ao sinal PRBS
load ma31500
showPrj1 = 0;

Ta=30;     % Temp. Ambiente - Entrada a ser medida em tempo-real

Tid=6;       % Taxa de amostragem em da identificação em s

PO = 60;  % Ponto de Operação em ºC
% Frequencia cooler Perturbação senoidal
w0=0.06544984694979; % rad/s. Período = 96 seg
w0=0.09817477042468; % rad/s. Período = 64 seg

umax=1024;  % Resolução do PWM ou 12 V do aquecimento máximo
umin=0;

 % Condições iniciais (ou deixar 0).
xs1i=0; xii=0; xs2i=0; x1i=0; x2i=0; x1ei=0; x2ei=0;

%  P2DZ de referência: ganho, 2 polos, atraso e zero - só para se familiarizar
% <<<<< Estes valores deverão ser atualizados por cada grupo >>>>>>>>>
Kp=0.09078613274052; 
Tp1=352.9649296152092;
Tp2=48.86312641418012;
Tz=249.2256137163764;
Td=4.66250225154633;

%%%%%%%%%%%%%%%%%%%%
% Identificação dos Parâmetros a partir dos Sinais Medidos
um=ma.signals(2).values;         N=length(um);
y=ma.signals(1).values;ym=y(:);
tm=0:length(um)-1;tm=tm';

transit=2000; % descarta transitorio de Tamb ao P.O.
ui=um(transit:length(um));yi=ym(transit:length(ym));
Ni=length(ui); tid=0:Ni-1;tid=tid';
uid=ui-mean(ui); yid=yi-mean(yi);

%%%% avaliar diferentes partições data/datav. Utilzar o modelo com melhor fitness!
len_id=ceil(.65*length(uid));
data=iddata(yid(1:len_id), uid(1:len_id), Tid);
datav=iddata(yid(len_id:length(uid)), uid(len_id:length(uid)), Tid);

% delay=delayest(data);  ge=procest(data,'P2DZ','InputDelay',delay); %atribui delay
ge=procest(data,'P2DZ');  % tb estima Td   
compare(datav,ge);

h=get(ge)    % get object handle   

%%%%%%%%%%%%%%%%%%%%%%%
% Process model with transfer function:        
%                    1+Tz*s                    
%   G(s) = Kp * ------------------ * exp(-Td*s)
%               (1+Tp1*s)(1+Tp2*s)             
%                                              
%                T(s)              (Tz*s+1)                    
%   Ga(s) = ------ =  -------------------------- * exp(-Td*s)
%                Ta(s)     (Tp1*s+1)(Tp2*s+1)     

kp=h.Kp, tz=h.Tz; tp1=h.Tp1; tp2=h.Tp2; td=h.Td;

% Função de Transferência Modal  
% T(s)/Ta(s)  =      A/(Tp1.s +1) +      B/(Tp2.s+1)
% T(s)/U(s)  = Kp*A/(Tp1.s +1) + Kp*B/(Tp2.s+1)

A = (Tz-Tp1)/(Tp2-Tp1);
B = 1 - A;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
if showPrj1 ==1;
% Malha Aberta para identificação PRBS
    open Termico_FP
    sim Termico_FP

% Malha Fechada - Controle PI-AntiWindup
    open Prj1CDig
    sim Prj1CDig
end  

%%%%%%%%%%%%%%%%%%%%%%%%%
% Projeto EE contínuo com canla PI
ssd.A = [-1/Tp1 0; 0 -1/Tp2]; ssd.B = [Kp;Kp]; ssd.C=[A/Tp1 B/Tp2];

%Projeto K
Aa=[ssd.A(1,:) 0
    ssd.A(2,:) 0
    -ssd.C     0]  %xi
Ba=[ssd.B; 0];
Ca=[ssd.C  0];

%Projeto K
p=[-0.0008, -0.01,];
p=[-4.6/Tp1, -1/Tz];      % Melhor opção -> colocar um pólo sobre o zero da FT!!
%p=[-100/Tp1, -1/Tz];    

Ka=acker(Aa,Ba,[p,3*p(1)])
Ki=-Ka(3),K=Ka(1:2),
Nb=-Ki/p(1);      % (Nb + Ki s)/s    Nb s + Ki  -> s=-Ki/Nb =  Nb => -Ki/p  

%%%%%%%%%%%%%%%%%%%%%%% 
% Observador aumentado 

% Projeto L
As=[0 1;
    -w0^2 0];
Cs=[1 0];

Aa=[ssd.A    ssd.B*Cs;
    [0 0;0  0]   As];
Ba=[ssd.B;0;0]; Ca=[ssd. C 0 0];

o = 12*[-1/Tp1, -1/Tp2, -1/Tp1, -1/Tp1]
L=acker(Aa',Ca',o)'
Ls1=L(3);Ls2=L(4);

open Prj2CDig
sim Prj2CDig