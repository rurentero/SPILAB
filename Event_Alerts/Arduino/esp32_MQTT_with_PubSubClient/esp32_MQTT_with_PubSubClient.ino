// Sketch con el Public/Subscribe funcionando a 10/01
// 
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5StickC.h>

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

  // Subscribe to topic
  client.subscribe("mytopic/test");

  // initialize the M5StickObject (for LCD notifications)
  M5.begin();
}

// ---- Function invoked every time a new message arrives to the topic. 
void callback(char* topic, byte* payload, unsigned int length) {

  // Show notification in LCD
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf("New Notification!");
  M5.Lcd.fillCircle(40, 80, 30, RED);

  // PubSub Tasks
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
  //client.publish("mytopic/test", "This is a message sent 5 seconds later");
  client.loop();
}
