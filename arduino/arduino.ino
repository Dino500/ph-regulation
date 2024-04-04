
//pin activation 
int sensor1 = 0;//ph sensor

int sensor2 = 33;//ph minus level sensor  


int pump=8;//peristaltic pump 


long vrijeme;
long vrijeme2;  


void setup() {
  
  pinMode(sensor2, INPUT);

  pinMode(pump,  OUTPUT);


  Serial.begin(9600);

   
  Serial1.begin(9600);

  
  vrijeme=millis();
  vrijeme2=millis();
}
  String sensor_data;

void loop() {
  float ph = mapfloat(analogRead(sensor1), 0, 1023, 0, 14);

  ;

  bool v = digitalRead(sensor2);
 
 String sensorvalue = String(ph) + "," + (v ? "Full" : "Empty") +","+ millis()+"," +String(vrijeme); 

 bool Sr =false;
 
  while(Serial1.available()){
    
        //get sensor data from serial  in sensor_data
        sensor_data=Serial1.readString(); 
        Sr=true;       
    }

  if(Sr){
        Serial.println(sensor_data);
      if(sensor_data.toInt() == 1 ){
       digitalWrite(pump, HIGH);
      delay(100);
      digitalWrite(pump, LOW);
      delay(100);
      digitalWrite(pump, HIGH);
      delay(100);
      digitalWrite(pump, LOW);
      delay(100);
      digitalWrite(pump, HIGH);
      delay(100);
      digitalWrite(pump, LOW);
      delay(100);
      delay(1000);
      }
  }







  //seding data to ESP
  if( millis()-vrijeme > 10000 ){

    vrijeme = millis();
    Serial1.println(sensorvalue);
    Serial.println(sensorvalue);
  }




//checking ph value and startin pump 
  if(millis()-vrijeme2 > 30000){
    vrijeme2 = millis();
    
    if( digitalRead(sensor2)  && ph >7.6){

      digitalWrite(pump, HIGH);

        delay(1000);
      digitalWrite(pump, LOW);
    }
  }

}
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max){
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
