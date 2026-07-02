const int pinSensResist = 13; // D13
const int pinSensAmb = 4; // D4
const int pinPWMResist = 16; // RX2
const int pinPWMCooler = 17; // TX2

int freq = 1000;
int resolucao = 16; // Resolução do PWM = 2^8 bits = 255
int dutyCycleM = 168*256;
int dcOffset = 80*256;

unsigned long tempoTroca = 0;
int estadoAquecedor = dutyCycleM - dcOffset;
long intervalo = 20*1000;

const uint8_t senoide_cooler[120] = {173, 177, 182, 186, 191, 195, 199, 204, 208, 212, 216, 219, 223, 227, 230, 233, 236, 239, 241, 244, 246, 248, 250, 251, 253, 254, 254, 255, 255, 255, 255, 255, 254, 254, 253, 251, 250, 248, 246, 244, 241, 239, 236, 233, 230, 227, 223, 219, 216, 212, 208, 204, 199, 195, 191, 186, 182, 177, 173, 168, 163, 159, 154, 150, 145, 141, 137, 132, 128, 124, 120, 117, 113, 109, 106, 103, 100, 97, 95, 92, 90, 88, 86, 85, 83, 82, 82, 81, 81, 80, 81, 81, 82, 82, 83, 85, 86, 88, 90, 92, 95, 97, 100, 103, 106, 109, 113, 117, 120, 124, 128, 132, 137, 141, 145, 150, 154, 159, 163, 168};

bool first = 1;
int cont = 0;

int estadoCooler = 0;

unsigned long tempoAmostragem100ms = 0; // Para o Cooler

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


uint16_t getPWM_Senoide(long t_indice) {
  int num = 161 - 1; // 160
  int i = t_indice;
  
  int index = i % num;
  int direct = ((i % (2 * num)) >= num) ? -1 : 1;
  int signal = ((i % (4 * num)) >= 2 * num) ? -1 : 1;
  
  uint16_t valor = (direct == 1) ? lut_seno_1_4[index] : lut_seno_1_4[num - index];
  
  // O offset central da ventoinha é 43008.
  // Rebater a onda em torno deste centro impede que o PWM caia para a zona morta do motor,
  // mantendo o vento estritamente senoidal com excursão entre ~20481 e 65535.
  return (signal == 1) ? valor : (86016 - valor); 
}

// int main(){
//     for (long i = 0; i < 641; i++) {
//         printf("%ld; %u\n", i, getPWM_Senoide(i));
//     }
// }
    

void setup() {
  Serial.begin(115200);

  ledcAttach(pinPWMCooler, freq, resolucao);

  analogSetPinAttenuation(pinSensResist, ADC_0db); //Equivalente ao processo de aumento de resolução com Amp Op 
  analogSetPinAttenuation(pinSensAmb, ADC_0db);

  pinMode(pinSensResist, INPUT);
  pinMode(pinSensAmb, INPUT);
}

void loop() {
  cont++;

  estadoCooler = getPWM_Senoide(cont);
  ledcWrite(pinPWMCooler, estadoCooler);

  int mv1 = analogReadMilliVolts(pinSensResist);
  int mv2 = analogReadMilliVolts(pinSensAmb);

  Serial.print(mv1);
  Serial.print(",");
  Serial.print(mv2);
  Serial.print(",");
  Serial.print(estadoCooler);
  Serial.println(",");

  delay(100);
}

