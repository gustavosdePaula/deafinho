// Requisitos

/* 
  ler microfone
  emitir som
  abrir porta
  fechar porta
   
  converter a amplitude (defasar) // tratamento

   

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

int microphone_reader(){
  INFO("Ouvindo ...");
  
  int _microphone;

  // analogRead(PIN_MICROPHONE);
  
  return _microphone;
}

int animal_distance_reader(){
  INFO("Medindo a distancia do animal");

  int _animal_distance = ultrasonic_animal.read(); //distance in CM

  return _animal_distance;
}



void door(bool status){
  // false  ->  open
  // true   ->  close

  control_door = status;

  digitalWrite(PIN_DOOR_1,control_door);
  digitalWrite(PIN_DOOR_2,!control_door);

  INFO("Porta " + control_door ? "Fechada":"Aberta"); //ver se compila
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

int soundPresent(uint8_t  _pin){
  int s=0;
  int aux=100000;
  for(int i=0;i<250;i++){
    if(aux >= s)
      aux = s; // pega o Valor Minimo

    s = analogRead(_pin);
  }

  return aux;
}


void loop() {
  
  // Serial.print(analogRead(0));
  // Serial.print("\t");
  // Serial.println(analogRead(1));
  // delay(250);
  
  // test_Speaker();
  float m1 = test_Microphone(PIN_MICROPHONE_1);
  // LOG(-500);  // To freeze the lower limit
  // LOG(" ");
  // LOG(500);  // To freeze the upper limit
  // LOG(" ");
  // LOGln(m1-500);  // To send all three 'data' points to the plotter
  // LOG(m1);
  // LOG(" ");  
  // m1 = (m1/800)*255;
  // if(m1 > 0){
  //   analogWrite(PIN_BUZZER, m1);
  //   analogWrite(PIN_BUZZER_2, 0); 
  // }
  // else{
  //   analogWrite(PIN_BUZZER, 0);
  //   analogWrite(PIN_BUZZER_2, m1); 
  // }

  tone(PIN_BUZZER,700);
  LOG(analogRead(A3));
  // delay(100);
  // delayMicroseconds(T);
  // delay(300);


  // LOG(m1);  
  // LOG("\t");
  // int m2 = test_Microphone(PIN_MICROPHONE_2);
  // LOG(m2);  
  // int a = animal_distance_reader();
  LOG("\n");
  // delay(1);


  animal_distance = animal_distance_reader();

  if(animal_distance > 100){

  }

  sound_extern = soundPresent(PIN_MICROPHONE_1);
  sound_intern = soundPresent(PIN_MICROPHONE_2);


}
