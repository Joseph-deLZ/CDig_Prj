Ta=27.4; % Temp. Ambiente média do processo

umax=256*256; % Resolução do PWM (16 bits) ou 12 V do aquecimento máximo
umin=0;

% Condições iniciais (ou deixar 0).
xs1i=0; xii=0; xs2i=0; x1i=0; x2i=0; x1ei=0; x2ei=0;

% P2DZ antigo nominal obtido:
Kp = 0.8864/256.0;
Tp1 = 323.16;
Tp2 = 37.636;
Tz = 278.32;
Td = 5; % Configurado manualmente --> Apresentou o melhor fit!

Tr = 30;

PO = 60;  % Ponto de Operação em ºC

w0 = 0.09817477042468; % Frequencia do cooler perturbação senoidal (período de 64s)

A = (Tz-Tp1)/(Tp2-Tp1);
B = 1 - A;

% Projeto EE contínuo com canal PI
ssd.A = [-1/Tp1 0; 0 -1/Tp2]; ssd.B = [Kp;Kp]; ssd.C=[A/Tp1 B/Tp2];

% Projeto proporcional K
Aa = [ssd.A(1,:) 0
    ssd.A(2,:) 0
    -ssd.C     0];  %xi
Ba = [ssd.B; 0];
Ca = [ssd.C  0];

p = [-2.2/88.9, -1/Tz]; % --> Configurado manualmente para cravar 30s de Tsubida no Prjt contínuo

Ka = acker(Aa,Ba,[p,3*p(1)]);
Ki = -Ka(3);
K = Ka(1:2);
Nb = -Ki/p(1);      % (Nb + Ki s)/s    Nb s + Ki  -> s=-Ki/Nb =  Nb => -Ki/p  

%%%%%%%%%%%%%%%%%%%%%%% 
% Observador aumentado 

% Projeto L
As=[0 1;
    -w0^2 0];
Cs=[1 0];

Aa=[ssd.A    ssd.B*Cs;
    [0 0;0  0]   As];

Ba=[ssd.B;0;0];
Ca=[ssd. C 0 0];

o = 12*[-1/Tp1, -1/Tp2, -1/Tp1, -1/Tp1];
L = acker(Aa',Ca',o);
Ls1 = L(3);
Ls2 = L(4);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PROJETO NO EE DISCRETIZADO

% ===========================
% DISCRETIZAÇÃO DO OBSERVADOR

Ts = 5; % Taxa de amostragem (Obrigatório igual a Td)

Sys_Cont_Obs = ss(Aa, Ba, Ca, 0); % Matrizes 4x4 do Observador (Aa, Ba, Ca, 0)

Sys_Disc_Obs = c2d(Sys_Cont_Obs, Ts, 'zoh'); %Discretiza usando Zero-Order Hold (ZOH)

o_disc = exp(o * Ts); % Mapeamento EXATO dos polos do Observador para o plano Z

L_discreto = acker(Sys_Disc_Obs.A', Sys_Disc_Obs.C', o_disc)'; % Cálculo da matriz L discreta verdadeira

% Imprime tudo no terminal formatado para C++
clc;
fprintf('float Ad[4][4] = {\n  {%.6f, %.6f, %.6f, %.6f},\n  {%.6f, %.6f, %.6f, %.6f},\n  {%.6f, %.6f, %.6f, %.6f},\n  {%.6f, %.6f, %.6f, %.6f}\n};\n\n', Sys_Disc_Obs.A');
fprintf('float Bd[4] = {%.6f, %.6f, %.6f, %.6f};\n\n', Sys_Disc_Obs.B);
fprintf('float Cd[4] = {%.6f, %.6f, %.6f, %.6f};\n\n', Sys_Disc_Obs.C);
fprintf('float Ld[4] = {%.6f, %.6f, %.6f, %.6f};\n\n', L_discreto);

% ============================
% DISCRETIZAÇÃO DO CONTROLADOR

% Discretiza somente a planta P2DZ original (sem o integrador)
Sys_Plant_Cont = ss(ssd.A, ssd.B, ssd.C, 0);
Sys_Plant_Disc = c2d(Sys_Plant_Cont, Ts, 'zoh');

Ad_p = Sys_Plant_Disc.A;
Bd_p = Sys_Plant_Disc.B;
Cd_p = Sys_Plant_Disc.C;

% Monta a matriz aumentada já no domínio discreto. Isso garante que a
% matemática case com o 'xi = xi + erro' do ESP
Aa_disc_ctrl = [Ad_p, zeros(2,1);
               -Cd_p, 1];
Ba_disc_ctrl = [Bd_p; 0];

% Encontra onde o ZOH jogou o zero da planta
z_zero_discreto = zero(Sys_Plant_Disc); 

% Mapeia o seu polo dominante (que dita o Tr = 88.9)
z_dom = exp(p(1) * Ts);

% Mapeia o polo rápido (3x)
z_rapido = exp(3 * p(1) * Ts);

% Força o 2º polo exatamente sobre o zero discreto
p_disc = [z_dom, z_zero_discreto, z_rapido];

% Aplica o Ackermann agora no sistema puramente discreto
Ka_disc = acker(Aa_disc_ctrl, Ba_disc_ctrl, p_disc);

K_discreto = Ka_disc(1:2);
Ki_discreto = -Ka_disc(3);

% Mapeia o Fator Nb
Nb_discreto = Ki_discreto / (1 - z_dom);

% Imprime para copiar pro C++
fprintf('float Kd[2] = {%.6f, %.6f};\n', K_discreto);
fprintf('float Ki_d = %.6f;\n', Ki_discreto);
fprintf('float Nb_d = %.6f;\n', Nb_discreto);

open Prj2CDig
sim Prj2CDig
