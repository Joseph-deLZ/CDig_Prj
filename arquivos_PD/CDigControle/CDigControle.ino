//Definição dos pinos

const int pinSensResist = 13; // D13
const int pinSensAmb = 4; // D4
const int pinPWMResist = 16; // RX2
const int pinPWMCooler = 17; // TX2

int freq = 5000;
int resolucao = 8; // Resolução do PWM = 2^8 bits = 255
int dutyCycle = 36;

bool first = 1;
int cont = 0;

unsigned long tempoTroca = 0;
int estadoAquecedor = dutyCycle;
long intervalo = 6*60*1000;

int ponto_operacao = 60;
float erro_anterior = 0;
float delta_u_anterior = 0;

const uint8_t senoide_cooler[120] = {173, 177, 182, 186, 191, 195, 199, 204, 208, 212, 216, 219, 223, 227, 230, 233, 236, 239, 241, 244, 246, 248, 250, 251, 253, 254, 254, 255, 255, 255, 255, 255, 254, 254, 253, 251, 250, 248, 246, 244, 241, 239, 236, 233, 230, 227, 223, 219, 216, 212, 208, 204, 199, 195, 191, 186, 182, 177, 173, 168, 163, 159, 154, 150, 145, 141, 137, 132, 128, 124, 120, 117, 113, 109, 106, 103, 100, 97, 95, 92, 90, 88, 86, 85, 83, 82, 82, 81, 81, 80, 81, 81, 82, 82, 83, 85, 86, 88, 90, 92, 95, 97, 100, 103, 106, 109, 113, 117, 120, 124, 128, 132, 137, 141, 145, 150, 154, 159, 163, 168};

unsigned long tempoAmostragem = 0;
// const float q0 = 3.42225; // Calculado via Tustin para K = 3.25 e a = 0.106
// const float q1 = -3.07775; // Calculado via Tustin

const float q0 = 3.25; // Calculado via ZOH (Step-Invariant) para K = 3.25 e a = 0.106
const float q1 = -2.9055; // Calculado via ZOH (Step-Invariant)

void setup() {
  Serial.begin(115200);

  ledcAttach(pinPWMResist, freq, resolucao);
  ledcAttach(pinPWMCooler, freq, resolucao);

  analogSetPinAttenuation(pinSensResist, ADC_0db); //Equivalente ao processo de aumento de resolução com Amp Op 
  analogSetPinAttenuation(pinSensAmb, ADC_0db);

  pinMode(pinSensResist, INPUT);
  pinMode(pinSensAmb, INPUT);
}


void loop() {
  // 1. GARANTE A AMOSTRAGEM RÍGIDA DE 1Hz (Ts = 1 segundo)
  if (millis() - tempoAmostragem >= 1000) {
    tempoAmostragem = millis();

    // 2. REALIZA A LEITURA DOS SENSORES
    int mvResist = analogReadMilliVolts(pinSensResist);
    int mvAmb = analogReadMilliVolts(pinSensAmb);
    // float y_abs = (mvResist-mvAmb) / 10.0; // Controlar temperatura diferencial 
    float y_abs = mvResist / 10.0; // Controlar temperatura absoluta

    float pwm_absoluto = dutyCycle;
    float setpoint_dinamico = ponto_operacao; 
    float erro_atual = 0;
    float delta_u = 0;
    int estadoCooler = 0;

    // 3. MÁQUINA DE ESTADOS DO EXPERIMENTO
    if (first == 1) {
      cont++; 

      // --- FASE A: WARM-UP (0 a 4 minutos) ---
      if (cont <= 4 * 60) {
      // if (cont <= 10) {
        pwm_absoluto = dutyCycle - 10; 
        estadoCooler = 0;
        erro_anterior = 0;
        delta_u_anterior = 0;
      }
      
      // --- FASE B: DEGRAUS DE SETPOINT (4 a 19 minutos) ---
      else if (cont > 4 * 60 && cont <= 19 * 60) {
      // else if (cont > 10 && cont <= 19 * 60){
        estadoCooler = 0;
        ledcWrite(pinPWMCooler, estadoCooler);

        // Alterna +-3°C a cada 6 minutos (360s)
        int tempo_pos_warmup = cont - (4 * 60);
        // int tempo_pos_warmup = cont - (10);
        int numero_do_ciclo = tempo_pos_warmup / (6 * 60);
        setpoint_dinamico = (numero_do_ciclo % 2 == 0) ? ponto_operacao + 5 : ponto_operacao - 5;

        // 1. Calcula a ação pelo PI de Tustin
        erro_atual = setpoint_dinamico - y_abs;
        delta_u = (q0 * erro_atual) + (q1 * erro_anterior) + delta_u_anterior;
        pwm_absoluto = dutyCycle + delta_u;

        // 2. ANTI-WINDUP IMEDIATO: Satura a variável antes de atualizar a memória
        if (pwm_absoluto > 128.0) pwm_absoluto = 128.0;
        if (pwm_absoluto < 0.0)   pwm_absoluto = 0.0;

        // 3. ATUALIZA HISTÓRICO: Guarda apenas a variação REAL que o hardware aceitou
        erro_anterior = erro_atual;
        delta_u_anterior = pwm_absoluto - dutyCycle;
      }
      
      // --- FASE C: DEGRAUS + PERTURBAÇÃO CONSTANTE (19 a 40 minutos) ---
      else if (cont > 19 * 60 && cont <= 40 * 60) {
        estadoCooler = 168; // Perturbação constante (meia carga)
        ledcWrite(pinPWMCooler, estadoCooler);

        int tempo_pos_warmup = cont - (4 * 60);
        int numero_do_ciclo = tempo_pos_warmup / (6 * 60);
        setpoint_dinamico = (numero_do_ciclo % 2 == 0) ? ponto_operacao + 5 : ponto_operacao - 5;

        // 1. Calcula a ação pelo PI de Tustin
        erro_atual = setpoint_dinamico - y_abs;
        delta_u = (q0 * erro_atual) + (q1 * erro_anterior) + delta_u_anterior;
        pwm_absoluto = dutyCycle + delta_u;

        // 2. ANTI-WINDUP IMEDIATO: Satura a variável antes de atualizar a memória
        if (pwm_absoluto > 128.0) pwm_absoluto = 128.0;
        if (pwm_absoluto < 0.0)   pwm_absoluto = 0.0;

        // 3. ATUALIZA HISTÓRICO: Guarda apenas a variação REAL que o hardware aceitou
        erro_anterior = erro_atual;
        delta_u_anterior = pwm_absoluto - dutyCycle;
      }
      
      // --- FASE D: DEGRAUS + PERTURBAÇÃO SENOIDAL (40 a 52 minutos) ---
      else if (cont > 40 * 60 && cont <= 52 * 60) {
        // 1. Calcula os segundos decorridos especificamente NESTA fase
        int tempo_fase_senoide = cont - (40 * 60);
        
        // 2. Aplica o truque do módulo para varrer o vetor de 0 a 119 ciclicamente
        // Subtraímos 1 para o índice começar exatamente em 0 no primeiro segundo
        int indice_lut = (tempo_fase_senoide - 1) % 120;
        
        // 3. Extrai o valor do PWM do cooler direto da tabela e escreve no hardware
        estadoCooler = senoide_cooler[indice_lut];
        ledcWrite(pinPWMCooler, estadoCooler);

        // A alternância de setpoint de 6 em 6 minutos continua intocada e funcional
        int tempo_pos_warmup = cont - (4 * 60);
        int numero_do_ciclo = tempo_pos_warmup / (6 * 60);
        setpoint_dinamico = (numero_do_ciclo % 2 == 0) ? ponto_operacao + 5 : ponto_operacao - 5;

        // 1. Calcula a ação pelo PI de Tustin
        erro_atual = setpoint_dinamico - y_abs;
        delta_u = (q0 * erro_atual) + (q1 * erro_anterior) + delta_u_anterior;
        pwm_absoluto = dutyCycle + delta_u;

        // 2. ANTI-WINDUP IMEDIATO: Satura a variável antes de atualizar a memória
        if (pwm_absoluto > 128.0) pwm_absoluto = 128.0;
        if (pwm_absoluto < 0.0) pwm_absoluto = 0.0;

        // 3. ATUALIZA HISTÓRICO: Guarda apenas a variação REAL que o hardware aceitou
        erro_anterior = erro_atual;
        delta_u_anterior = pwm_absoluto - dutyCycle;
      }
      
      // --- FIM DO EXPERIMENTO COMPLETO (Após 52 minutos) ---
      else {
        first = 0; 
      }
    } 
    
    // --- MODO DE SEGURANÇA PÓS-EXPERIMENTO ---
    else {
      estadoCooler = 0;
      ledcWrite(pinPWMCooler, estadoCooler);
      pwm_absoluto = 0;
      erro_atual = 0;
      delta_u = 0;
    }

    // 5. ENVIO PARA O HARDWARE
    ledcWrite(pinPWMResist, (int)pwm_absoluto);

    // 6. LOGGER PARA O COOLTERM
    Serial.printf("%d;%.2f;%d;%.2f;%.2f;%d;%.2f;%d;\n", 
                  (int)(millis()/1000), y_abs, (int)pwm_absoluto, erro_atual, delta_u, estadoCooler, setpoint_dinamico, mvAmb);
  }
}
