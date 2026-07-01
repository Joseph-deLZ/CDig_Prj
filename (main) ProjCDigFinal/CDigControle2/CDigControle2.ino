// ==============================================================================
// Prj2CDig - Controle PI Anti-Windup com Observador de Estados e Perturbação
// ==============================================================================

const int pinSensResist = 13; // D13
const int pinSensAmb = 4;     // D4
const int pinPWMResist = 16;  // RX2
const int pinPWMCooler = 17;  // TX2

int freq = 1000; // Frequência de 1kHz para suportar a resolução real de 16 bits no ESP32
int resolucao = 16; // 16 bits (0 a 65535)

float dutyCycle_16bit = 9216.0;
int ponto_operacao = 60;

int cont = 0; // Contador global de segundos reais
int decisegundos = 0; // Contador de décimos de segundo (para a senoide suave)
long indice_senoide = 0; // Índice dinâmico que varre a LUT

float setpoint_dinamico = 60.0;
int estadoCooler_16bit = 0;
bool controle_ativo = false;
bool senoide_ativa = false;

// LUT de 1/4 de onda da senoide (161 posições)
const uint16_t lut_seno_1_4[161] = {
  43008, 43229, 43450, 43671, 43892, 44113, 44334, 44555, 44775, 44996, 45216, 45436,
  45656, 45875, 46095, 46313, 46532, 46750, 46968, 47186, 47403, 47620, 47836, 48052,
  48267, 48482, 48696, 48910, 49123, 49335, 49547, 49759, 49969, 50179, 50389, 50597,
  50805, 51012, 51219, 51424, 51629, 51833, 52036, 52238, 52439, 52640, 52839, 53038,
  53235, 53432, 53627, 53822, 54015, 54208, 54399, 54589, 54779, 54967, 55153, 55339,
  55524, 55707, 55889, 56070, 56249, 56428, 56605, 56780, 56955, 57128, 57299, 57470,
  57638, 57806, 57972, 58137, 58300, 58461, 58622, 58780, 58937, 59093, 59247, 59400,
  59550, 59700, 59847, 59994, 60138, 60281, 60422, 60561, 60699, 60835, 60970, 61102,
  61233, 61362, 61490, 61615, 61739, 61861, 61981, 62099, 62216, 62330, 62443, 62554,
  62663, 62770, 62875, 62979, 63080, 63180, 63277, 63373, 63466, 63558, 63647, 63735,
  63821, 63904, 63986, 64066, 64143, 64219, 64292, 64364, 64433, 64500, 64565, 64629,
  64690, 64749, 64806, 64860, 64913, 64964, 65012, 65058, 65103, 65145, 65185, 65222,
  65258, 65292, 65323, 65352, 65379, 65404, 65427, 65448, 65466, 65482, 65496, 65508,
  65518, 65526, 65531, 65534, 65535
};

// Mapeamento dinâmico da senoide
uint16_t getPWM_Senoide(long t_indice) {
  int num = 161 - 1; // 160
  int i = t_indice;
  
  int index = i % num;
  int direct = ((i % (2 * num)) >= num) ? -1 : 1;
  int signal = ((i % (4 * num)) >= 2 * num) ? -1 : 1;
  
  uint16_t valor = (direct == 1) ? lut_seno_1_4[index] : lut_seno_1_4[num - index];
  
  // Mantem o vento estritamente senoidal com excursão entre 20481 e 65535 para que o motor não pare.
  return (signal == 1) ? valor : (86016 - valor); 
}

// Máquina de Estados
void atualizarMaquinaDeEstados(int t) {

  // 0 - 240 = WARM_UP
  if (t < 240) {
    setpoint_dinamico = ponto_operacao;
    estadoCooler_16bit = 0;
    controle_ativo = false;
    senoide_ativa = false;
  }
  // 240 - 640 = PO-5, FAN(OFF), SEN(OFF)
  else if (t < 240 + 400) {
    setpoint_dinamico = ponto_operacao - 5;
    estadoCooler_16bit = 0;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 640 - 1040 = PO+5, FAN(OFF), SEN(OFF)
  else if (t < 240 + 800) {
    setpoint_dinamico = ponto_operacao + 5;
    estadoCooler_16bit = 0;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 1040 - 1240 = PO-5, FAN(OFF), SEN(OFF)
  else if (t < 240 + 1000) {
    setpoint_dinamico = ponto_operacao - 5;
    estadoCooler_16bit = 0;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 1240 - 1440 = PO-5, FAN(ON)[Meia_Carga], SEN(OFF)
  else if (t < 240 + 1200) {
    setpoint_dinamico = ponto_operacao - 5;
    estadoCooler_16bit = 43008; // Valor inicial da senoide / meia carga do cooler
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 1440 - 1840 = PO+5, FAN(ON)[Meia_Carga], SEN(OFF)
  else if (t < 240 + 1600) {
    setpoint_dinamico = ponto_operacao + 5;
    estadoCooler_16bit = 43008;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 1840 - 2040 = PO-5, FAN(ON)[Meia_Carga], SEN(OFF)
  else if (t < 240 + 1800) {
    setpoint_dinamico = ponto_operacao - 5;
    estadoCooler_16bit = 43008;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 2040 - 2240 = PO-5, FAN(ON), SEN(ON)
  else if (t < 240 + 2000) {
    setpoint_dinamico = ponto_operacao - 5;
    controle_ativo = true;
    senoide_ativa = true;
  }
  // 2240 - 2640 = PO+5, FAN(ON), SEN(ON)
  else if (t < 240 + 2400) {
    setpoint_dinamico = ponto_operacao + 5;
    controle_ativo = true;
    senoide_ativa = true;
  }
  // 2640 - 3040 = PO-5, FAN(ON), SEN(ON)
  else if (t < 240 + 2800) {
    setpoint_dinamico = ponto_operacao - 5;
    controle_ativo = true;
    senoide_ativa = true;
  }
  // 3040 - 3240 = PO+5, FAN(ON), SEN(ON)
  else if (t < 240 + 3000) {
    setpoint_dinamico = ponto_operacao + 5;
    controle_ativo = true;
    senoide_ativa = true;
  }
  // 3240 - 3440 = PO+5, FAN(ON)[Meia_Carga], SEN(OFF)
  else if (t < 240 + 3200) {
    setpoint_dinamico = ponto_operacao + 5;
    estadoCooler_16bit = 43008;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 3440 - 3840 = PO-5, FAN(ON)[Meia_Carga], SEN(OFF)
  else if (t < 240 + 3600) {
    setpoint_dinamico = ponto_operacao - 5;
    estadoCooler_16bit = 43008;
    controle_ativo = true;
    senoide_ativa = false;
  }
  // 3840 - inf = Desliga tudo
  else {
    setpoint_dinamico = 0;
    estadoCooler_16bit = 0;
    controle_ativo = false;
    senoide_ativa = false;
  }
}

// VARIÁVEIS E MATRIZES DO ESPAÇO DE ESTADOS (16-bits / Ts = 5s)

int Ts = 5;

float x_hat[4] = {0.0, 0.0, 0.0, 0.0}; 
float x_next[4] = {0.0, 0.0, 0.0, 0.0}; 
float xi = 0.0; // Integral do erro
float u_aplicado_16bit = 0.0; 

// Valores de Ad, Bd, Cd, Ld, Kd, Ki_d e Nb_d obtidos via MatLab disponibilizado pelo professor
float Ad[4][4] = {
  {0.984647, 0.000000, 0.016495, 0.042199},
  {0.000000, 0.875595, 0.015547, 0.040587},
  {0.000000, 0.000000, 0.881921, 4.801608},
  {0.000000, 0.000000, -0.046279, 0.881921}
};

float Bd[4] = {0.017179, 0.016212, 0.000000, 0.000000};

float Cd[4] = {0.000486, 0.022398, 0.000000, 0.000000};

float Ld[4] = {268.619625, 35.665721, 187.870228, -101.898335};

float Kd[2] = {0.537509, 18.214815};
float Ki_d = 97.163316;
float Nb_d = 834.839529;

unsigned long tempoAmostragem100ms = 0; // Pro Cooler
unsigned long tempoAmostragem5s = 0; // Pro Observador

void setup() {
  Serial.begin(115200);

  ledcAttach(pinPWMResist, freq, resolucao);
  ledcAttach(pinPWMCooler, freq, resolucao);
  analogSetPinAttenuation(pinSensResist, ADC_0db); 
  analogSetPinAttenuation(pinSensAmb, ADC_0db);

  pinMode(pinSensResist, INPUT);
  pinMode(pinSensAmb, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // MÁQUINA DE ESTADOS E VENTILADOR (Roda a cada 100ms)
  if (currentMillis - tempoAmostragem100ms >= 100) {
    tempoAmostragem100ms = currentMillis;
    
    decisegundos++;
    if (decisegundos % 10 == 0) {
      cont++; // Incrementa a cada 1 segundo real
    }

    // Define Valor da referência, ativação do controle e estado da perturbação senoidal
    atualizarMaquinaDeEstados(cont);

    // Gestão dinâmica da senoide
    if (senoide_ativa) {
      estadoCooler_16bit = getPWM_Senoide(indice_senoide);
      indice_senoide++;
    } else {
      indice_senoide = 0; // Reseta pra onda sempre iniciar do zero
    }

    // Atualiza o vento suavemente (10x por segundo)
    ledcWrite(pinPWMCooler, estadoCooler_16bit);
  }

  // OBSERVADOR E CONTROLADOR (Ts = 5s)
  if (currentMillis - tempoAmostragem5s >= 1000*Ts) {
    tempoAmostragem5s = currentMillis;

    int mvResist = analogReadMilliVolts(pinSensResist);
    int mvAmb = analogReadMilliVolts(pinSensAmb);
    float Y_real = mvResist / 10.0;
    
    float Y_estimado = 0;
    float erro_obs = 0;
    float e_sin_estimado = 0;

    if (controle_ativo) {
      // A matemática matricial é baseada na variação em torno do PO!
      float delta_Y_real = Y_real - ponto_operacao;
      float delta_R = setpoint_dinamico - ponto_operacao;

      // O Gêmeo digital avalia a variação da temperatura
      Y_estimado = (Cd[0] * x_hat[0]) + (Cd[1] * x_hat[1]) + (Cd[2] * x_hat[2]) + (Cd[3] * x_hat[3]);
      erro_obs = delta_Y_real - Y_estimado;
      e_sin_estimado = x_hat[2];

      // Valor do erro de controle
      float erro_controle = delta_R - delta_Y_real;
      
      // Anti-windup condicional para o limite de meia carga (32768)
      if (u_aplicado_16bit > 0.0 && u_aplicado_16bit < 32768.0) {
          xi = xi + (erro_controle * (float)Ts);
      }

      // Ação feedforward e rejeição
      float U_PI = (Nb_d * delta_R) + (Ki_d * xi); // Nb multiplica a VARIAÇÃO da referência
      float U_s = U_PI - ((Kd[0] * x_hat[0]) + (Kd[1] * x_hat[1]));
      float u_calc = U_s - e_sin_estimado;

      // Retorna para o referencial absoluto somando a base do aquecedor
      u_aplicado_16bit = dutyCycle_16bit + u_calc; 

      // Saturação restrita à meia carga 
      if (u_aplicado_16bit > 32768.0) u_aplicado_16bit = 32768.0;
      if (u_aplicado_16bit < 0.0)   u_aplicado_16bit = 0.0;

      // Calcula a variação de energia que REALMENTE consegue passar pelo fio
      float u_calc_saturado = u_aplicado_16bit - dutyCycle_16bit;

      // 4. O Gêmeo Digital se atualiza
      for (int i = 0; i < 4; i++) {
        x_next[i] = (Ad[i][0] * x_hat[0]) + (Ad[i][1] * x_hat[1]) + 
                    (Ad[i][2] * x_hat[2]) + (Ad[i][3] * x_hat[3]) + 
                    (Bd[i] * u_calc_saturado) + // <-- GÊMEO ENXERGA A SATURAÇÃO
                    (Ld[i] * erro_obs);
      }
      for (int i = 0; i < 4; i++) {
        x_hat[i] = x_next[i];
      }

    } 
    else {
      // Modo Warm-up ou Desligado
      if (setpoint_dinamico == ponto_operacao) { 
        // Durante o warm-up dá um PWM um pouco menor que o duty cycle normal para não dar overshoot
        u_aplicado_16bit = dutyCycle_16bit - 2650.0; 
      } else {
        u_aplicado_16bit = 0; // Fim do experimento
      }
      xi = 0.0;
    }

    // Aplica a tensão necessária
    ledcWrite(pinPWMResist, (int)(u_aplicado_16bit));

    // LOGGER (A cada 5s)
    float Y_estimado_Absoluto = Y_estimado + ponto_operacao; // Recupera a estimativa absoluta para o gráfico somando o PO
    Serial.printf("%d;%.2f;%.2f;%d;%.2f;%.2f;%d;%.2f;%d;\n",
                  cont, Y_real, Y_estimado_Absoluto, (int)u_aplicado_16bit, erro_obs, e_sin_estimado, estadoCooler_16bit, setpoint_dinamico, mvAmb);
  }
}