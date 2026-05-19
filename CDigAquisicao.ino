//Definição dos pinos

const int pinSensResist = 13; // D13
const int pinSensAmb = 4; // D4
const int pinPWMResist = 16; // RX2
const int pinPWMCooler = 17; // TX2

int freq = 5000;
int resolucao = 8; // Resolução do PWM = 2^8 bits = 255
int dutyCycle = 32;

bool first = 1;
int cont = 0;

class PRBSGenerator {
private:
    int b, j, m;
    int* x;           // Registrador de deslocamento
    int counter = 0;  // Contador para o holding time (m)
    int currentBit = 0;

public:
    PRBSGenerator(int _b, int _m) : b(_b), m(_m) {
        // Define o tap de feedback (j) exatamente como no MATLAB
        j = 1;
        if (b == 5) j = 2;
        else if (b == 7) j = 3;
        else if (b == 9) j = 4;
        else if (b == 10) j = 3;
        else if (b == 11) j = 2;

        x = new int[b];
        // Inicialização aleatória do registrador (rand(1,b) > 0.5)
        randomSeed(analogRead(0));
        for (int i = 0; i < b; i++) {
            x[i] = random(0, 2);
        }
    }

    int getNextBit() {
        if (counter == 0) {
            // Pega o último bit como saída (x(b) no MATLAB)
            currentBit = x[b - 1];

            // Calcula o feedback: xor(y(n), x(b-j))
            // No C++ (0-indexed), x(b) é x[b-1] e x(b-j) é x[b-j-1]
            int feedback = currentBit ^ x[b - j - 1];

            // Desloca o registrador (x = [feedback x(1:b-1)])
            for (int i = b - 1; i > 0; i--) {
                x[i] = x[i - 1];
            }
            x[0] = feedback;
        }

        counter++;
        if (counter >= m) counter = 0;

        return currentBit;
    }
};

// Instanciação: b=10 bits, m=28 (cada bit dura 28 ciclos de loop)
PRBSGenerator meuPrbs(11, 21);

void setup() {
  Serial.begin(115200);

  ledcAttach(pinPWMResist, freq, resolucao);
  ledcAttach(pinPWMCooler, freq, resolucao);

  analogSetPinAttenuation(pinSensResist, ADC_0db); //Equivalente ao processo de aumento de resolução com Amp Op 
  analogSetPinAttenuation(pinSensAmb, ADC_0db);

  pinMode(pinSensResist, INPUT);
  pinMode(pinSensAmb, INPUT);

  Serial.println("Análise de TF iniciada");
}


void loop() {

  int copy = -1;

  if (first == 1){
    cont = cont + 1;
    if (cont > 6*60){
      first = 0;
    }
      
    else if (cont > 2*60){
      ledcWrite(pinPWMResist, dutyCycle-15);
    }
  }

  else {
    // 1. Gera o bit atual do PRBS
    int prbsStatus = meuPrbs.getNextBit();
    copy = prbsStatus;
    // 2. Mapeia o bit para o PWM físico
    // Se prbsStatus for 1, liga o aquecedor no seu ponto de operação (ex: 32)
    int valorPwm = (prbsStatus == 1) ? dutyCycle+15 : dutyCycle-15; //32 de dutycyle leva Temperatura p/ 60° em RP
    ledcWrite(pinPWMResist, valorPwm);
  }

  // 3. Realiza a leitura dos sensores (com a sua lógica diferencial)
  int mvResist = analogReadMilliVolts(pinSensResist);
  int mvAmb = analogReadMilliVolts(pinSensAmb);

  // 4. Logger para o CoolTerm (CSV)
  // Tempo; Tensão; Estado_PRBS
  Serial.printf("%lu;%d;%d;%d;\n", millis(), mvResist, mvAmb, copy);

  delay(1000); // Frequência de amostragem de 1Hz
}
