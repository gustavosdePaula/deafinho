// Requisitos

/* 
  ler microfone
  emitir som
  abrir porta
  fechar porta
   
  converter a amplitude (defasar) // tratamento

   

*/
 
#include "Ultrasonic.h"


#define LOG(x) Serial.print(x)

// OUTPUTS
#define PIN_BUZZER        A2
#define PIN_DOOR          A4

// INPUTS
#define PIN_MICROPHONE    A1

#define PIN_ANIMAL_SENSOR_TRIGGER A3
#define PIN_ANIMAL_SENSOR_ECHO    A3



Ultrasonic ultrasonic_animal(PIN_ANIMAL_SENSOR_TRIGGER, PIN_ANIMAL_SENSOR_ECHO);

//Variables

float frequency = 0.0;
int microphone = 0;
int animal_distance = 0;
bool control_door = false;


int microphone_reader(){
  LOG("Ouvindo ..." + "\n");
  
  int _microphone;
  
  return _microphone;
}

int animal_distance_reader(){
  LOG("Medindo a distancia do animal" + "\n");

  int _animal_distance = ultrasonic_animal.read(); //distance in CM

  return _animal_distance;
}

void sound_output(float f){
  LOG("Ligando o som" + "\n");
  float _frequency = f;


}

void door(bool status){
  // false  ->  open
  // true   ->  close

  control_door = status;

  digitalWrite(PIN_DOOR,control_door);

  LOG("Porta " + control_door ?"Fechada":"Aberta"); //ver se compila
  LOG("\n");
}



void setup() {
  // configs

  Serial.begin(9600);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_DOOR, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);


}

void loop() {
  
  // Serial.print(analogRead(0));
  // Serial.print("\t");
  // Serial.println(analogRead(1));
  // delay(250);
  
  door(1);
  delay(500);
}
