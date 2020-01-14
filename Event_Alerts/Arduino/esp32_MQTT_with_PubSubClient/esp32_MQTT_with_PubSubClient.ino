// Sketch con el Public/Subscribe funcionando a 14/01
// Sketch designed for M5-Stick-C based on Esp32 boards.
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5StickC.h>
#include <ArduinoJson.h>
// WARNING: MQTT_MAX_PACKET_SIZE needs to be defined in PubSubClient.h library. MQTT Max message size is 128 by default.

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

  // Initialize the M5StickObject (for LCD notifications)
  M5.begin();
}

// ---- Function invoked every time a new message arrives to the topic. 
void callback(char* topic, byte* payload, unsigned int length) {
  // PubSub Tasks
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //Deserialize Json
  Serial.println("Deserializating JSON");
  DynamicJsonDocument doc(length);
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }
  Serial.print(F("Recource Type: "));
  const char* resource = doc["resource"];
  Serial.println(resource);
// TODO Probar resto de accesos a obetos/arrays anidados
//  Serial.println(F("Params -> Event: "));
//  Serial.println(doc["params"]["event"]["id"]);
//  Serial.println(doc["params"]["event"]["title"]);
//  Serial.println(doc["params"]["event"]["description"]);
//  Serial.println(F("Params -> Event --> Location: "));
//  Serial.println(doc["params"]["event"]["location"]["latitude"]);
//  Serial.println(doc["params"]["event"]["location"]["longitude"]);
//  Serial.println(doc["params"]["event"]["location"]["radius"]);

  //Resource selector. TODO: En caso de APIs diferentes métodos para una ruta, habría que hacer un selector para el tipo de método
  if (strcmp(resource,"Event")==0) {
    notify("New notification: Event");
    postEvent();
  }
  else if (strcmp(resource,"User")==0) {
    notify("New notification: User");
    getUsers();
  }
  else {  //Default
    Serial.println("API resource not supported.");
  }

  Serial.println("-----------------------");
}

void loop() {
  client.loop();
}


// ---- API Methods
void postEvent() {
  // Mocked response
  Serial.println("Method: postEvent");
}

void getUsers() {
  // Mocked response
  Serial.println("Method: getUsers");
}


// ---- Utilities
void notify(char* msg){
  // Show notification in LCD. M5 must be initialized.
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.printf(msg);
  M5.Lcd.fillCircle(40, 80, 30, RED);
}
