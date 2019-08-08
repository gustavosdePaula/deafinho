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

#define PIN_DOOR_1        1
#define PIN_DOOR_2        2

#define PIN_LED_SOUND_EXTERN  10
#define PIN_LED_SOUND_INTERN  9

// INPUTS
#define PIN_MICROPHONE_1    A0
#define PIN_MICROPHONE_2    A5

#define PIN_ANIMAL_SENSOR_TRIGGER 5
#define PIN_ANIMAL_SENSOR_ECHO    6



Ultrasonic ultrasonic_animal(PIN_ANIMAL_SENSOR_TRIGGER, PIN_ANIMAL_SENSOR_ECHO);

//Variables

int sound_extern = 0;
int sound_intern = 0;

int microphone = 0;
int animal_distance = 0;

bool control_door = false;

int T=10000;
int steps = 1000;

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


  digitalWrite(PIN_DOOR_1,control_door);
  digitalWrite(PIN_DOOR_2,!control_door);

  LOG("Porta ");
  INFO(control_door);
}

void door_stop(){

  digitalWrite(PIN_DOOR_1,0);
  digitalWrite(PIN_DOOR_2,0);

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
  pinMode(PIN_DOOR_1, OUTPUT);
  
  // pinMode(PIN_BUZZER, OUTPUT);


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
  

  animal_distance = animal_distance_reader();

  if(animal_distance > 100){
    door(1);
  }
  else{
    door(0);
  }

/* 
  sound_extern = microphone_reader(PIN_MICROPHONE_1);

  if(sound_extern > 200){
    turn_led(PIN_LED_SOUND_EXTERN,1); //liga led externo
  }
  else {
    turn_led(PIN_LED_SOUND_EXTERN,0); //liga led externo
  }

  sound_intern = microphone_reader(PIN_MICROPHONE_2);

  if(sound_intern < 100){
    turn_led(PIN_LED_SOUND_INTERN,1); //liga led indicativo som interno
  }
  else {
    turn_led(PIN_LED_SOUND_INTERN,0); //liga led interno
  }
*/
}
