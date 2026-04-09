% Prj1CDig_m
%
% Controle PI Anti-WindUp de Processo Térmico
%
% @AB ENE/UnB Condrole Digital

% resposta do processo térmico ao sinal PRBS
load ma31500

Ta=30;     % Temp. Ambiente - Entrada a ser medida em tempo-real
Ts=1;       % Taxa de amostragem em s
PO = 50;  % Ponto de Operação em ºC

%%%%%%%%%%%% Frequencia cooler Perturbação senoidal
w0=0.06544984694979; % rad/s. Período = 96 seg
w0=0.09817477042468; % rad/s. Período = 64 seg

%  P2DZ: ganho, 2 polos, atraso e zero
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
len_id=ceil(.75*length(uid));

data=iddata(yid(1:len_id), uid(1:len_id), Ts);
datav=iddata(yid(len_id:length(uid)), uid(len_id:length(uid)), Ts);

kk=delayest(data);
ge=procest(data,'P2DZ','InputDelay',kk)
%%%%%%%%%%%%%%%%%%%%%%%

% Process model with transfer function:        
%                    1+Tz*s                    
%   G(s) = Kp * ------------------ * exp(-Td*s)
%               (1+Tp1*s)(1+Tp2*s)             
%                                              
%                T(s)              (Tz*s+1)                    
%   Ga(s) = ------ =  -------------------------- * exp(-Td*s)
%                Ta(s)     (Tp1*s+1)(Tp2*s+1)     

h=get(ge);
kp=h.Kp, tz=h.Tz; tp1=h.Tp1; tp2=h.Tp2; td=h.Td;

% (Ka+Kt+Kw)*Ct = 1/Tp1 = 0.00283314
Ct = 0.0002;
% (Ka+Kt+Kw) =14.1657
Ka = 1; 
Kw = 4;
Kt = 9.1657;

Kp2 =  1/(Kt*Ct*Tp1); % ajuste de ganho de T(s)/U(s)

% Função de Transferência Modal  
% T(s)/Ta(s)  =      A/(Tp1.s +1) +      B/(Tp2.s+1)
% T(s)/U(s)  = Kp*A/(Tp1.s +1) + Kp*B/(Tp2.s+1)


A = (Tz-Tp1)/(Tp2-Tp1);
B = 1 - A;

% Capacitância térmica do processo.
Ct = 0.0002;

% Controlador PI
P=3;I=0.05;

open Termico_FP
sim Termico_FP

open Prj1CDig
sim Prj1CDig