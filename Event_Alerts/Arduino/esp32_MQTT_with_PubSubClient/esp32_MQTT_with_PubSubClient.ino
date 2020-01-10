// Sketch con el Public/Subscribe funcionando a 10/01
// 
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Gloin";
const char* password = "Gloin2014";
const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
 
WiFiClient espClient;
PubSubClient client(espClient);
  
void setup() {
  Serial.begin(115200);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }

  client.subscribe("mytopic/test");

}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void loop() {
  // put your main code here, to run repeatedly:
  //client.publish("mytopic/test", "This is a message sent 5 seconds later");
  client.loop();
}
