/*
 *     Maker      : DIYSensors                
 *     Project    : Build your own Smart Light Sensor
 *     Version    : 0.4
 *     Date       : 02-2025
 *     Programmer : Ap Matteman
 *     
 */ 

const int iMinLux = 200;    // Minimal lux level for lights on
const long tTimeOn = 30000;   // Time delay in milliseconds
long tTime;
int iBlink = 0;


const int iPin_LedRed = D6;
const int iPin_PIR = D7;



void setup() {
  Serial.begin(115200);
  pinMode(iPin_LedRed, OUTPUT);
  pinMode(iPin_PIR, INPUT);
  
}


void loop() {
  int iPIRvalue = digitalRead(iPin_PIR);
  int iLuxReading = analogRead(0);

  if((iPIRvalue == 1) && (iLuxReading <iMinLux)) {  
    Serial.print("Light: "); Serial.println(iLuxReading);
    tTime = millis();
    Serial.print("millis() - tTime = "); Serial.println(millis() - tTime);
    
    iBlink = 0;
  } 

  if(millis() - tTime < tTimeOn){
    // Light on
    Serial.print("LIGHT ON millis() - tTime = "); Serial.println(millis() - tTime);


    // let the LED blink
    iBlink++;
    if (iBlink <= 10) {
      digitalWrite(iPin_LedRed, HIGH);
    } else {
      digitalWrite(iPin_LedRed, LOW); 
    }
    if (iBlink > 20) { iBlink = 0;}  

    // Checkif there is some motion detect while the light is on
    if(iPIRvalue == 1) {
      	tTime = millis();
        iBlink = 0;
    }

  } else {
    digitalWrite(iPin_LedRed, LOW); 
  }

  delay(100);
  
}
