#include <A2a.h>

#define endereco 8

A2a arduinoMaster;


void setup() {
  // put your setup code here, to run once:
  //servo.attach(3);
  //pinMode(led, OUTPUT);
  arduinoMaster.begin(endereco);
  arduinoMaster.onReceive(receberDados);
  arduinoMaster.onRequest(enviarDados);

  Serial.begin(9600);
  
void loop() {
  // put your main code here, to run repeatedly:

  int valorSensor = digitalRead(sensor);
  arduinoMaster.varWireWrite(0, highByte(valorSensor));
  arduinoMaster.varWireWrite(1, lowByte(valorSensor));
  Serial.println(valorSensor);
}

void receberDados() {
  arduinoMaster.receiveData(); 
}

void enviarDados() {
  arduinoMaster.sendData(); 
}
