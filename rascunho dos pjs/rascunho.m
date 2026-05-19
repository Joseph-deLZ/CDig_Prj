% deve ser executado depois do script 'AnaliseCDig.m'
% --- 6. CONVERSÃO E PROJETO NO SISO TOOL ---

% Converte o modelo identificado (idproc) para uma Função de Transferência clássica (tf)
G_identificada = tf(modelo);
%G_identificada = zpk(modelo);

% Abre o Control System Designer (SISO Tool) com a sua planta carregada no bloco 'G'
controlSystemDesigner(G_identificada);

% Nota: Se você estiver usando uma versão muito antiga do MATLAB, use o comando abaixo:
% sisotool(G_identificada);