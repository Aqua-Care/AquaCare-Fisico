#include <WiFiManager.h>
#include <WiFi.h>

WiFiServer server(80);

void setup()
{
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
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available(); // listen for incoming clients
 int redes=WiFi.scanNetworks();

        if(redes==0){
         client.println("Não existem redes por perto");
        }else{
          client.print(redes);
          client.println(" redes encontradas");
          
            client.print("SSID: ");
            client.println(WiFi.SSID());
            client.print("Sinal: ");
            client.println(WiFi.RSSI());
            delay(100);
          
          client.println("\n---------------------------------------------------------------------------\n");
          delay(3000);
        }
      }
