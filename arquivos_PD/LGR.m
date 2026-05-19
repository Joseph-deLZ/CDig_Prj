s = tf('s');

Kp = 0.8864;
Tp1 = 323.16;
Tp2 = 37.636;
Td = 5;
Tz = 278.32;

Hap = (Kp*(1+Tz*s)*(1-(Td/2)*s))/((1+Tp1*s)*(1+Tp2*s)*(1+(Td/2)*s));

% Antigos (para Mp_teórica = 45%)
%K = 2.7675; 
%A = 0.1378;


% Atuais (com Mp_teórica ajustada para 35%)
K = 3.25;
A = 0.106;

% Pelo fato do sistema NÃO SER de segunda ordem, fazer o projeto para Mp = 45% no
% LGR vai gerar um sobresinal maior que 45%, por isso tive que baixar para 35%.

G = K*(s+A)/s;

%rlocus(Hap);
%rlocus(G*Hap);

step(feedback(G*Hap, 1));