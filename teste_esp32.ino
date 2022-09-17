#include "WiFi.h"


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

}

void loop() {
  int redes=WiFi.scanNetworks();

  if(redes==0){
    Serial.println("Não existem redes por perto");
  }else{
    Serial.print(redes);
    Serial.println(" redes encontradas");
    for(int i=0;i<redes;i++){
      Serial.print("SSID: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Sinal: ");
      Serial.println(WiFi.RSSI(i));
      delay(100);
    }
    Serial.println("\n---------------------------------------------------------------------------\n");
    delay(3000);
  }
}
