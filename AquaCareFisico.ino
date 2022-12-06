  
#include <WiFiManager.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif


#include <Firebase_ESP_Client.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define   relay1   D0
#define ONE_WIRE_BUS D2


// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 2. Define the API Key */
#define API_KEY "AIzaSyCib82NkCyfwXVdczmLFSCfeDesJOI8a5Y"

/* 3. Define the RTDB URL */
#define DATABASE_URL "aquacarebd-fa5c0-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "lucasfantunes2005@gmail.com"
#define USER_PASSWORD "123456"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

Servo myservo;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

String luz;
String rac;

float temp;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

void setup(void)
{
  WiFi.mode(WIFI_STA);
    
    
    Serial.begin(115200);
    
    
    WiFiManager wm;

   

    bool res;
   
    res = wm.autoConnect("AquaCare","password"); 

    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {
      
        Serial.println("connected...yeey :)");
    }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h


#if defined(ESP8266)
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;


  myservo.attach(D1);//servo
  pinMode(relay1, OUTPUT);//relé

//ds18b20
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  
  Serial.print("Device 0 Address: ");
  //printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
}


void printTemperature(DeviceAddress deviceAddress)
{
  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }

  temp = tempC;
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print("Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}
 

void loop(void)
{
  int pos;
  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

    Serial.printf("Valor Luz... %s\n", Firebase.RTDB.getInt(&fbdo, F("/Aquario/Luz/LED")) ? luz = String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    Serial.printf("Valor Racao... %s\n", Firebase.RTDB.getInt(&fbdo, F("/Aquario/Racao/Alimentar")) ? rac = String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    Serial.printf("Valor Temperatura... %s\n", Firebase.RTDB.setString(&fbdo, F("/Aquario/Agua/Temperatura"), F("")) ? "ok" : fbdo.errorReason().c_str());
    Serial.printf("O valor da temperatura... %s\n", Firebase.RTDB.getString(&fbdo, F("/Aquario/Agua/Temperatura")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

    Serial.println(temp);


//luz
   if (luz == "1"){
    digitalWrite(relay1, LOW);
    }else if(luz == "0") {
      digitalWrite(relay1, HIGH);
      }

//alimentação
     if (rac == "1"){
      int pos;

      for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      }
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      }
      
      }

//temperatura

   }

  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  // It responds almost immediately. Let's print out the data
  printTemperature(insideThermometer);

   
}


void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
