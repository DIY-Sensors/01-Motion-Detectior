/*
 *     DIY-Sensors
 *     
 *     Project    : Motion detection
 *     Versie     : 1.0
 *     Datum      : 12-2023
 *     Schrijver  : Ap Matteman
 */    

#include <PubSubClient.h>
#include <ESP8266WiFi.h> 

/* WiFi en MQTT gegevens */
const char* YourWiFiSSID = "YourSSID";
const char* YourWiFiPassword = "YourWiFiPassword";
const char* mqtt_server = "YourMQTTserver";
const char* mqtt_user = "MQTTHome";
const char* mqtt_password = "YourMQTTPassword";
const char* Device_ID = "Motion";

int WiFiTry = 0;
int MQTTTry = 0;

WiFiClient espClient;
PubSubClient client(espClient); // MQTT Client



const int PIRPIN = D7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(PIRPIN, INPUT);

  Serial.println("Connecting to ");
  Serial.println(YourWiFiSSID);
  ConnectWiFi();
  Serial.println("Connecting to MQTT");
  client.setServer(mqtt_server, 1883);
  ConnectMQTT();
}

void ConnectWiFi() { 
  //Connect with WiFi network
  WiFi.begin(YourWiFiSSID, YourWiFiPassword);
  Serial.println("Connecting to WiFi");
  //Try to connect to WiFi for 5 times
  while (WiFi.status() != WL_CONNECTED && WiFiTry < 5) {
    ++WiFiTry;
    Serial.print("WiFi status: ");; Serial.println(WiFi.status());
    Serial.print("WiFiTry: ");; Serial.println(WiFiTry);
    delay(1000);
  }
  Serial.println("");
  Serial.print("WiFiTry: ");; Serial.println(WiFiTry);
  WiFiTry = 0;
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
}

void ConnectMQTT() {
  while (!client.connected() && MQTTTry < 5 ) {
      Serial.print("Attempting MQTT connection...");
      ++MQTTTry;
      // Create a random client ID
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
        Serial.println("connected");
        Serial.print("MQTT ClientID = "); Serial.println(clientId);
        delay(100);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
    }
  }
  MQTTTry = 0;
  Serial.println("MQTT end loop.");
}


void loop() {
  // put your main code here, to run repeatedly:
  int LightReading = analogRead(0);
  int pirValue = digitalRead(PIRPIN);
  Serial.print(LightReading); Serial.print("\t"); Serial.println(pirValue);

  // Send info to MQTT server
  client.publish("Office/Sensor/Light", String(LightReading).c_str());
  client.publish("Office/Sensor/motion", String(pirValue).c_str());
  
  delay(1000);
}
