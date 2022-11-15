#include <A2a.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <Servo.h>

// Set these to run example.
#define FIREBASE_HOST "aquacarebd-fa5c0-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "0mwBCOKLQ0Y4runPUgIbR1cga5ZDmbfH85LlYQ0O"

#define endereco 8

#define SDA 0 
#define SCL 2

#define sensor 4 
#define led 5


Servo servo;


WiFiServer server(80);

A2a arduinoSlave;

void setup() {

  //conexão wifi
  WiFi.mode(WIFI_STA);
  
    Serial.begin(115200);

   WiFiManager wm;

    wm.resetSettings();


    bool res;
  
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        
    } 
    else {
        
        Serial.println("connected :)");
    }


    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    //fim conexão wifi

    
    server.begin();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.set("LUZ", 0);
    Firebase.setInt("Temperatura", 0);
    Firebase.set("Servo", 0);

    arduinoSlave.begin(SDA, SCL);

    arduinoSlave.pinWireMode(endereco, led, OUTPUT);
}

int n = 0;

void loop() {

  n = Firebase.getInt("LED");
  if(n == 0){
    arduinoSlave.digitalWireWrite(endereco, luz, LOW);
    return;
    delay(10);
    }
    else{
      arduinoSlave.digitalWireWrite(endereco, luz, HIGH);
      return;  
      }

  int valorSensor = digitalRead(sensor);
  arduinoMaster.varWireWrite(0, highByte(valorSensor));
  arduinoMaster.varWireWrite(1, lowByte(valorSensor));
  Serial.println(valorSensor);
  
  // set value
 Firebase.setInt("Temperatura", arduinoSlave.digitalWireRead(valorSensor));
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /Temperatura failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  

  // get value 
  Serial.print("Servo: ");
  Serial.println(Firebase.getBool("Servo"));
  delay(1000);

   Serial.print("Luz: ");
  Serial.println(Firebase.getBool("Luz"));
  delay(1000);

}
