#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST “esri-eea51.com“
#define FIREBASE_AUTH “qULRQbnuz————-Q1FxHvM“
#define WIFI_SSID “kavin“
#define WIFI_PASSWORD “123456345“
void setup() {
    Serial.begin(9600);
    pinMode(D1, OUTPUT);
  // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(“connecting“);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(“.“);
      delay(500);
    }
    Serial.println();
    Serial.print(“connected: “);
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.set(“LED_STATUS“, 0);
  }
int n = 0;
void loop() {
// get value
  n = Firebase.getInt(“LED_STATUS“);
// handle error
  if (n==1) {
    Serial.println(“LED ON“);
    digitalWrite(D1,HIGH);
    return;
    delay(10);
  }
  else {
    Serial.println(“LED OFF“);
    digitalWrite(D1,LOW);
  return;
  }
}
