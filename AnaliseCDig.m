% --- 1. CARREGAMENTO E EXTRAÇÃO ---
csv_data_path = 'varreduras/Varredura_6h_10b.csv';

tabela = readtable( ...
    csv_data_path, ...
    detectImportOptions(csv_data_path) ...
    );

% Extraindo a temperatura
y_temp_f = tabela.deltaTFinal;
u_f = tabela.dcPWM;
Ts = 1;

% % --- 2. CORTE DO WARM-UP ---
% % Aplica o mesmo corte em todos os sinais para manter a sincronia
% inicio = 6*60*1000; 
% y_temp_f = y_temp_f(inicio:end);
% u_f = u_bruto(inicio:end);

% --- 3. PREPARAÇÃO PARA IDENTIFICAÇÃO (DETREND) ---
% Para o MATLAB identificar o ganho do aquecedor, usamos o desvio
y_id = detrend(y_temp_f, 'constant');
u_id = detrend(u_f, 'constant');
data = iddata(y_id, u_id, Ts);

% --- 5. IDENTIFICAÇÃO DO MODELO ---
% Cria a estrutura P2DZ (2 polos, 1 zero, com atraso)
modelo_init = idproc('P2DZ');

% Define o valor do atraso (ex: 5 segundos) e trava para o MATLAB não alterar
modelo_init.Structure.Td.Value = 5; 
modelo_init.Structure.Td.Free = false; 

% Roda a identificação com o atraso travado
modelo = procest(data, modelo_init);

% Exibe o Fit e as Constantes
figure('Name', 'Validação do Modelo');
compare(data, modelo);
present(modelo);

%Kp = modelo.Kp