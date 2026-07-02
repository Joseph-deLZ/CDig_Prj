% Prj1CDig_m
%
% Controle PI Anti-WindUp de Processo Térmico
%
% @AB ENE/UnB Condrole Digital

% resposta do processo térmico ao sinal PRBS
load ma31500

Ta=30;     % Temp. Ambiente - Entrada a ser medida em tempo-real
Ta=27;
W=0;       % Perturbação
Ts=1;       % Taxa de amostragem em s
PO = 60;  % Ponto de Operação em ºC

%%%%%%%%%%%% Frequencia cooler Perturbação senoidal

w0=0.09817477042468; % rad/s. Período = 64 seg

Kp = 0.8864/256.0;
Tp1 = 323.16;
Tp2 = 37.636;
Tz = 278.32;
Td = 5;

% %%%%%%%%%%%%%%%%%%%%
% % Identificação dos Parâmetros a partir dos Sinais Medidos
% um=ma.signals(2).values;         N=length(um);
% y=ma.signals(1).values;ym=y(:);
% tm=0:length(um)-1;tm=tm';
% 
% transit=2000; % descarta transitorio de Tamb ao P.O.
% ui=um(transit:length(um));yi=ym(transit:length(ym));
% Ni=length(ui); tid=0:Ni-1;tid=tid';
% uid=ui-mean(ui); yid=yi-mean(yi);
% 
% %%%% avaliar diferentes partições data/datav. Utilzar o modelo com melhor fitness!
% len_id=ceil(.75*length(uid));
% data=iddata(yid(1:len_id), uid(1:len_id), Ts);
% datav=iddata(yid(len_id:length(uid)), uid(len_id:length(uid)), Ts);
% 
% delay=delayest(data);
% ge=procest(data,'P2DZ')
% compare(datav,ge)
% 
% % o atraso pode tb ser especificado, procest(data,'P2DZ','InputDelay',kk)
% h=get(ge)   
% 
% return
% %%%%%%%%%%%%%%%%%%%%%%%
% 
% % Process model with transfer function:        
% %                    1+Tz*s                    
% %   G(s) = Kp * ------------------ * exp(-Td*s)
% %               (1+Tp1*s)(1+Tp2*s)             
% %                                              
% %                T(s)              (Tz*s+1)                    
% %   Ga(s) = ------ =  -------------------------- * exp(-Td*s)
% %                Ta(s)     (Tp1*s+1)(Tp2*s+1)     
% 
% h=get(ge);
% kp=h.Kp, tz=h.Tz; tp1=h.Tp1; tp2=h.Tp2; td=h.Td;
% 
% % (Ka+Kt+Kw)*Ct = 1/Tp1 = 0.00283314
% Ct = 0.0002;
% % (Ka+Kt+Kw) =14.1657
% Ka = 1; 
% Kw = 4;
% Kt = 9.1657;
% 
% Kp2 =  1/(Kt*Ct*Tp1); % ajuste de ganho de T(s)/U(s)

% Função de Transferência Modal  
% T(s)/Ta(s)  =      A/(Tp1.s +1) +      B/(Tp2.s+1)
% T(s)/U(s)  = Kp*A/(Tp1.s +1) + Kp*B/(Tp2.s+1)


A = (Tz-Tp1)/(Tp2-Tp1);
B = 1 - A;

% Capacitância térmica do processo.
Ct = 0.0002;

P=3.25;
I=0.106;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Malha Aberta para identificação PRBS
open Termico_FP
sim Termico_FP

% Malha Fechada - Controle PI-AntiWindup
open Prj1CDig
sim Prj1CDig

%%%%%%%%%%%%%%%%%%%%%%%%%
% Projeto PI discreto
gpt =zpk(-1/Tz,[-1/Tp1 ...
    -1/Tp2], Kp);
gpt.InputDelay=Td;

Tc=2*Td
Tc=Td
gv=c2d(gpt,Tc)

% keyboard

% Sisotool(gd) [zeta=0.8] 
% =>  P + I*Tc/(z-1) => P(z- (-P+I Tc)/P)

%===== Prj via SisoTool ajuste (zeta,wn) para obter Mp=25% Simulação ====
%                                                 wn[rad/s]   Mp   tp[s]   ts[s]   u_max
%Tc=Td: [zeta=0.707,Mp teo=4.326]  0.137   25    28     73.8    1.0       
P = 0.909
Z = 0.84

I=-(P*Z-P)/Tc    % Ganho do Canal Integral

open Prj1dCDig
sim Prj1dCDig
