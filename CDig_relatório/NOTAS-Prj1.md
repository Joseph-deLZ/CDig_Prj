> data 08/04/2026

- download dos drivers para windows 11 - via site (https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads) - "CP210x Universal Windows Driver v11.5.0 12/31/2025"

- (comando `supportPackageInstaller` ou pela gui: `Add-Ons >> Manage Add-Ons >> MATLAB Support Package for Arduino Hardware`) download 

os que são baixados por padrão:
- I2C
- SPI
- Servo


passos q devem ser repetidos dps:
- executar comando `arduinosetup` no matlab R2025b (ou `Hardware Setup` na gui)

- comentar no texto: "Take a closer look at the chip next to the voltage regulator on the board and check its name. For this board, it comes with the Silabs CP2102 chip." (fonte: https://randomnerdtutorials.com/install-esp32-esp8266-usb-drivers-cp210x-windows/ )

- boa referência para informações à respeito da esp32: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitm-1/user_guide.html#getting-started

- ESP32 NodeMcu – DevKit v1


---
> data 09/04/2026 (em aula)

- obs: usar tempo de pico como especificação para o projeto 1

---
> 15/04
- matlab: "use the Arduino Explorer app or type a = arduino. For unofficial (clone) Arduino boards, type a = arduino(port,board)."

---
> 06/05

- novo material: termômetro digital
- colocar ref à parte do datasheet de onde foram tirados os valores sugeridos para R e C dos filtros passa baixas
- escrever: problema de sobrecorrente em um dos transistores BC548
            - descoberto após cálculo da corrente para escolha dos resistores q garantem 1/4 W
            - TIP: colocar datasheet
- coolterm: software para a coleta dos dados


- [ ] talvez: olhar notas no samsung notes

---
> 11/05

- download do "Simulink Support Package for Arduino Hardware"
- ctrl + E: config param : escolher a placa correta -> necessário para definir o `Pin number` = 16 no bloco de `PWM` da biblioteca "Simulink Support Package for Arduino Hardware"


---
> 18/05


- em `Preferences >> Time Delays`, no `Control System Design`, Padé order foi alterado para 1!

---
> 19/05

colocar no relatório:
- período de warmup de 4 min
- [!?] colocar configurações feitas no Arduino IDE -> referência para o vídeo

mini roteiro(minha parte do vídeo):
```
- modelo -> TF
- comando pra abrir o CSD - SISO TOOL
    -> abre com TF -> G(s)

- disposição das janelas
- root Locus Editor
    -> Design Requirements:
              -> % de overshoot: 45
              -> aproximação de 2a ordem
              -> w0: ao menos 0.098
- Compensator Editor:
    -> inserir um integrador
    -> inserir um zero real -> por padrão está em -1

- root Locus Editor: arastar os polos de malha fechada -> observar mudança na resposta
```

---
> 2 de jun.

Colocar no relatório de CDig : Ajuste do sobrepasso % no Control System Designer ("siso tool")