#include <ArduinoJson.h>

#include <Wire.h>

#include <ESP8266HTTPClient.h>

#include <WiFiServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>  
#include "defines.h"


void setup() {

 

  WiFiManager wifimanager;

  Serial.begin(115200);

  delay(5000);

  Serial.println("Connectig to ");
  Serial.println(NETWORK_SSID);
  
  wifimanager.resetSettings();
  WiFi.mode(WIFI_STA);
  WiFi.begin(NETWORK_SSID,NETWORK_PWD);

  while (WiFi.status() != WL_CONNECTED){

    Serial.print(".");
    delay(500);
    
    }

    Serial.println("Connected!");
    Serial.println("The Esp8266 local ip is ");
    Serial.println(WiFi.localIP());

    //Start ESP8266 as I2C master
    Wire.begin();

        
   
 
}

void loop() {

 

  HTTPClient dGardenHttpClient;
  dGardenHttpClient.begin("http://104.236.209.29:8000/");

  int httpRequestStatus = dGardenHttpClient.GET();

  if(httpRequestStatus>0){
    //Serial.println("GET request DONE!");
    String payload = dGardenHttpClient.getString();
    //Serial.println(payload);
     

    DynamicJsonBuffer jsonBuffer;
    char rawJson[1024];
    strcpy(rawJson,payload.c_str());
    JsonArray& root = jsonBuffer.parseArray(rawJson);

    for(int i=0;i<root.size();i++){
        const char * myPlant;
        const char * myInterval;
        const char * myDuration;
    
      myPlant = root[i]["plantId"];
      myInterval = root[i]["interval"];
      myDuration = root[i]["duration"];
     

      
      Serial.print("The plant is: ");
      Serial.println(myPlant);
      Serial.println();
      Serial.print("The interval is: ");
      Serial.println(myInterval);
      Serial.println();
      Serial.print("The duration is: ");
      Serial.println(myDuration);
      Serial.println();
      //send all necessary information of one single plant to arduino board
      sendPlantData(myPlant,myInterval,myDuration);
      //necessary delay for data sync
      delay(1000);    
    }

      
          
      
      }

  dGardenHttpClient.end();
  delay (10000);

 

}

//function to send plant information via I2C to arduino slave
void sendPlantData(const char * plantName, const char * interval,  const char * duration){

  Wire.beginTransmission(8);
  Wire.write(plantName);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(8);
  Wire.write(interval);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(8);
  Wire.write(duration);
  Wire.endTransmission(); 
  
  
  }


