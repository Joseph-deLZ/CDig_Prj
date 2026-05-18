//Definição dos pinos

const int pinSensResist = 15; // D15
const int pinSensAmb = 4; // D4
const int pinPWMResist = 16; // RX2
const int pinPWMCooler = 17; // TX2

const int windowSize = 9; // Tamanho do filtro de mediana

float bufferSensResist[windowSize];
int index1 = 0;

float bufferSensAmb[windowSize];
int index2 = 0;

int freq = 5000;
int resolucao = 8; // Resolução do PWM = 2^8 bits = 255
int dutyCycle = 128;

unsigned long tempoTroca = 0;
int estadoAquecedor = 0;
long intervalo = 30000;

void setup() {
  Serial.begin(115200);

  ledcAttach(pinPWMResist, freq, resolucao);
  ledcAttach(pinPWMCooler, freq, resolucao);

  analogSetPinAttenuation(pinSensResist, ADC_0db); //Equivalente ao processo de aumento de resolução com Amp Op 
  analogSetPinAttenuation(pinSensAmb, ADC_0db);

  pinMode(pinSensResist, INPUT);
  pinMode(pinSensAmb, INPUT);

  // Inicializa ambos os buffers
  for(int i = 0; i < windowSize; i++) {
    bufferSensResist[i] = 0;
    bufferSensAmb[i] = 0;
  }
}

void loop() {
  // ledcWrite(pinPWMCooler, dutyCycle);

    // 2. Lógica do Sinal Constante para teste
  if (millis() - tempoTroca >= intervalo) {
    tempoTroca = millis();
    
    // Alterna entre 0 e 42 (30%)
    estadoAquecedor = (estadoAquecedor == 0) ? 42 : 42; //PWM_max teórico = 42
    ledcWrite(pinPWMResist, estadoAquecedor);
    
    Serial.printf(">>> MUDANÇA: Aquecedor em %d por %ld ms\n", estadoAquecedor, intervalo);
  }

  int mv1 = analogReadMilliVolts(pinSensResist);
  int mv2 = analogReadMilliVolts(pinSensAmb);

  Serial.printf("S1_Bruto:%d - S2_Bruto:%d === ", 
                mv1, mv2);

  float leituraBruta1 = mv1 / 10.0;
  float leituraBruta2 = mv2 / 10.0;

  Serial.printf("T1_Bruto:%.1f - T2_Bruto:%.1f\n", 
                leituraBruta1, leituraBruta2);

  // float filtrada1 = getMedianFiltered(leituraBruta1, bufferSensResist, index1);
  // float filtrada2 = getMedianFiltered(leituraBruta2, bufferSensAmb, index2);

  // Output formatado para o Serial Plotter ou Debug
  // Serial.printf("T1_Bruto:%.1f T1_Filt:%.1f | T2_Bruto:%.1f T2_Filt:%.1f\n", 
  //               leituraBruta1, filtrada1, leituraBruta2, filtrada2);

  delay(1000);
}

// Sua função de Insertion Sort (In-place)
void insertionSort(float arr[], int n) {
  for (int i = 1; i < n; i++) {
    float key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

// Função de Mediana Genérica
// O segredo está no 'int &index', que permite atualizar o valor global do índice
float getMedianFiltered(float newValue, float currentBuffer[], int &index) {
  // 1. Grava no buffer específico
  currentBuffer[index] = newValue;
  index = (index + 1) % windowSize;

  // 2. Cria cópia local para ordenação
  float tempArray[windowSize];
  for(int i = 0; i < windowSize; i++) tempArray[i] = currentBuffer[i];

  // 3. Ordena a cópia
  insertionSort(tempArray, windowSize);

  // 4. Retorna o valor central
  return tempArray[4];
}