//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESPDateTime.h>
#include <ArduinoJson.h>


// Set these to run example.
#define FIREBASE_HOST "" 
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""


String values,sensor_data;

void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone("CST-8");
  // DateTime.setServer("asia.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  // from
  /** changed from 0.2.x **/

  DateTime.setServer("time.pool.aliyun.com");
  DateTime.setTimeZone("CST+1");
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  } else {
    Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp is %ld\n", DateTime.now());
  }
   

}



void setup() {

  Serial.begin(9600);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  setupDateTime();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  int n = 0;

}



void loop() {
  DynamicJsonBuffer  jbuffer; 
  JsonObject& root = jbuffer.createObject();  

bool Sr =false;
 
  while(Serial.available()){
    
        //get sensor data from serial  in sensor_data
        sensor_data=Serial.readString(); 
        Sr=true;    
        
    }
  
delay(1000);

  if(Sr==true){  
    
  values=sensor_data;
  
  //get comma indexes from values variable
  int fristCommaIndex = values.indexOf(',');
  int secondCommaIndex = values.indexOf(',', fristCommaIndex+1);
  
  
  //get sensors data from values variable by  spliting by commas and put in to variables  
  String senzor1 = values.substring(0, fristCommaIndex);
  String senzor2 = values.substring(fristCommaIndex+1, secondCommaIndex);
 

  //store ultrasonic sensor data as string in firebase 
  Firebase.setString("Ph_vrijednost",senzor1);
   delay(10);
  //store IR sensor 1 data as string in firebase 
  Firebase.setString("stanje_spremnika",senzor2);
   delay(10);
 
  root["ph"] = senzor1; 
  root["vrijeme"] = DateTime.toString();

  //Firebase.pushString("ph_vrijednost_log",senzor1);
  //Firebase.pushString("vrijeme-log", DateTime.toString());

    Firebase.push("json",root);
   delay(10);

  
  delay(1000);
  
  if (Firebase.failed()) {  
      Serial.write("firebase bad");
  }
  
}   
}
