/******************************************************************************************
 * Mapa do Sketch
 * 1- Cabeçalho (bibliotecas, definições, variáveis e objetos globais)
 * 2- setup()
 * 3- loop()
 * 4- ci74HC595Write()
 * 5- delayPWM()
 * 
 * Desenvolvido por Ideias Tecnologia / Brincando com Ideias
 */
#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN

#define pinMotor3PWM 5

#define qtdeCI   1

#define bitMotor3A 5
#define bitMotor3B 7

void ci74HC595Write(byte pino, bool estado);

void setup() {
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinEnable, OUTPUT);
   pinMode(pinDS, OUTPUT);

   pinMode(pinMotor3PWM, OUTPUT);
   
   digitalWrite(pinEnable, LOW);
   Serial.begin(9600);
}

void loop() {
 
  Serial.println("Motores A=HIGH B=LOW - Fecha Porta - Cabo Azul: A, Cabo Branco: B");
  ci74HC595Write(bitMotor3A, HIGH);
  ci74HC595Write(bitMotor3B, LOW);
  delay(1000);

  Serial.println("Motores A=LOW B=HIGH - Abre Porta - Cabo Azul: A, Cabo Branco: B");
  ci74HC595Write(bitMotor3A, LOW);
  ci74HC595Write(bitMotor3B, HIGH);
  delay(1000);
}

void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
  
          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
          
          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT
          
          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}
