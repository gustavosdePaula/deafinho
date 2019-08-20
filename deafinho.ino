// Requisitos

/* 
  ler microfone
  emitir som
  abrir porta
  fechar porta
   
  converter a amplitude (defasar) // tratamento

   
  0, 1, 2, 3 Portas digitais
*/
 
#include "Ultrasonic.h"
// #include "Arduino.h"
// #include "Tone.h"



#define LOG(x) Serial.print(x)
#define INFO(x) Serial.print(x); Serial.print('\n')

// OUTPUTS
#define PIN_BUZZER        5
#define PIN_BUZZER_2      6

#define PIN_LED_SOUND_EXTERN  A2
#define PIN_LED_SOUND_INTERN  9

// INPUTS
#define PIN_MICROPHONE_1    A4
#define PIN_MICROPHONE_2    A5

#define PIN_ANIMAL_SENSOR_TRIGGER A5
#define PIN_ANIMAL_SENSOR_ECHO    A0


// CONSTANTS VALUES
#define   ANIMAL_MAX_DISTANCE_VALUE 12
#define   ANIMAL_MIN_DISTANCE_VALUE 1
#define   SOUND_EXTERN_LEVEL_VALUE 500
#define   SOUND_INTERN_LEVEL_VALUE 100
#define   TIMER_DELAY 1000


// MOTORS DEFINE

#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN

#define pinMotor3PWM 5

#define qtdeCI   1

#define bitMotor3A 5
#define bitMotor3B 7



Ultrasonic ultrasonic_animal(PIN_ANIMAL_SENSOR_TRIGGER, PIN_ANIMAL_SENSOR_ECHO);

//Variables

int sound_extern = 0;
int sound_intern = 0;

int microphone = 0;
int animal_distance = 0;

bool control_door = false;

int T=10000;
int steps = 1000;

int timer = 0;
int last_timer = 0;

void test_Speaker(){

  // analogWrite(PIN_BUZZER,200);
  // delay(1);
  // analogWrite(PIN_BUZZER,5);
  // delay(1);
  INFO("Testando o speaker");

  for(int i=0;i<steps;i++){
    analogWrite(PIN_BUZZER, (10 + i*240/steps)); 
    delayMicroseconds(T);
  }
  for(int i=steps;i>=0;i--){
    analogWrite(PIN_BUZZER, (10 + i*240/steps)); 
    delayMicroseconds(T);
  }
}


int animal_distance_reader(){
  INFO("Medindo a distancia do animal");

  int _animal_distance = ultrasonic_animal.read(); //distance in CM
  INFO(animal_distance);
  return _animal_distance;
}



void door(bool control_door){
  // false  ->  open
  // true   ->  close


  ci74HC595Write(bitMotor3A, control_door);
  ci74HC595Write(bitMotor3B, !control_door);

  LOG("Porta ");
  INFO(control_door);
}

void door_stop(){

  ci74HC595Write(bitMotor3A, LOW);
  ci74HC595Write(bitMotor3B, LOW);

  INFO("Porta Desativada"); 
}

int test_Microphone(uint8_t  _pin){
  int microfone = analogRead(_pin);  
  return microfone;
  // LOG(microfone);
  // LOG("\t");
}

void setup() {
  // configs

  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED_SOUND_EXTERN, OUTPUT);
  
  // pinMode(PIN_BUZZER, OUTPUT);

  ///Motor setup
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  pinMode(pinEnable, OUTPUT);
  pinMode(pinDS, OUTPUT);

  pinMode(pinMotor3PWM, OUTPUT);
   
  digitalWrite(pinEnable, LOW);
  analogWrite(PIN_LED_SOUND_EXTERN, 255);
  timer = millis();


}

void testMotor(){
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


int microphone_reader(uint8_t  _pin){
  int s=0;
  int aux=100000;
  for(int i=0;i<250;i++){
    if(aux >= s)
      aux = s; // pega o Valor Minimo

    s = analogRead(_pin);
  }

  return aux;
}

void turn_led(uint8_t  _pin, bool _status){
  digitalWrite(_pin,_status);
  INFO("Led " + _status);
}

void loop() {
  
  last_timer = millis()-timer;
  
  if (last_timer >= TIMER_DELAY)
  {
    timer = millis();
  }
  
  


  animal_distance = animal_distance_reader();

  // INFO(animal_distance);

  // delay(50);
  
  analogWrite(PIN_LED_SOUND_EXTERN, 255);
  delay(1000);
  analogWrite(PIN_LED_SOUND_EXTERN, 0);
  delay(1000);

  if(animal_distance > ANIMAL_MIN_DISTANCE_VALUE && animal_distance < ANIMAL_MAX_DISTANCE_VALUE){
    door(1);
    timer = millis();
    
  }
  else{
    door(0);
    timer = millis();
  }

  if(last_timer > TIMER_DELAY){
    door_stop();    
    // INFO("Timer---------------------------------------------------------------- ");
  }


/* 
  sound_extern = microphone_reader(PIN_MICROPHONE_1);

  if(sound_extern > SOUND_EXTERN_LEVEL_VALUE){
    turn_led(PIN_LED_SOUND_EXTERN,1); //liga led externo
  }
  else {
    turn_led(PIN_LED_SOUND_EXTERN,0); //liga led externo
  }

  sound_intern = microphone_reader(PIN_MICROPHONE_2);

  if(sound_intern < SOUND_INTERN_LEVEL_VALUE){
    turn_led(PIN_LED_SOUND_INTERN,1); //liga led indicativo som interno
  }
  else {
    turn_led(PIN_LED_SOUND_INTERN,0); //liga led interno
  }
*/
}
