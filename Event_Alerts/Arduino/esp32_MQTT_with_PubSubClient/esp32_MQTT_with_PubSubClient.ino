// Sketch con el Public/Subscribe funcionando a 14/01
// Sketch designed for M5-Stick-C based on Esp32 boards.
#include <WiFi.h>
#include <PubSubClient.h>
// WARNING: MQTT_MAX_PACKET_SIZE needs to be defined in PubSubClient.h library. MQTT Max message size is 128B by default.
#include <M5StickC.h>
#include <ArduinoJson.h>

const char* ssid = "Gloin";
const char* password = "Gloin2014";
const char* mqttServer = "192.168.0.107";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

// TODO Creación de structs para almacenar los datos entrantes
// TODO Modularizar el parseo desde un JSON a un struct con el objeto (Evento o User) 
 
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
  const char* method_ = doc["method"];
  Serial.println(method_);
// ---- All data accesses
//  const char* sender = doc["sender"];
//  Serial.println(sender);
//  Serial.println(F("Params -> Event:"));
//  int eventId = doc["params"]["event"]["id"];
//  Serial.println(eventId);
//  const char* title = doc["params"]["event"]["title"];
//  Serial.println(title);
//  const char* description = doc["params"]["event"]["description"];
//  Serial.println(description);
//  Serial.println(F("Event -> Location: "));
//  double latitude = doc["params"]["event"]["location"]["latitude"];
//  Serial.println(latitude,6);
//  double longitude = doc["params"]["event"]["location"]["longitude"];
//  Serial.println(longitude,6);
//  int radius = doc["params"]["event"]["location"]["radius"];
//  Serial.println(radius);

  //Resource selector. TODO: En caso de APIs diferentes métodos para una ruta, habría que hacer un selector para el tipo de método
  if (strcmp(resource,"Event")==0) {
    notify("New notification: Event");
    // Method selector: GET, POST, PUT, DELETE
    if (strcmp(method_,"getEvent")==0) { 
      getEvent();
    }
    else if (strcmp(method_,"postEvent")==0){
      postEvent();
    }
    else if (strcmp(method_,"putEvent")==0){
      putEvent();
    }
    else if (strcmp(method_,"deleteEvent")==0){
      deleteEvent();
    }
    else { // Default
      Serial.println("Resource method not supported.");
    }
  }
  else if (strcmp(resource,"User")==0) {
    notify("New notification: User");
    if (strcmp(method_,"getUser")==0) { 
      getUser();
    }
    else if (strcmp(method_,"postUser")==0){
      postUser();
    }
    else if (strcmp(method_,"putUser")==0){
      putUser();
    }
    else if (strcmp(method_,"deleteUser")==0){
      deleteUser();
    }
    else { // Default
      Serial.println("Resource method not supported.");
    }
  }
  else {  //Default
    Serial.println("API resource not supported.");
  }

  Serial.println("-----------------------");
}

void loop() {
  client.loop();
}


// --------------- API Methods
// ---- Event
void getEvent() {
  // Mocked response
  Serial.println("Method: getEvent");
}
void postEvent() {
  // Mocked response
  Serial.println("Method: postEvent");
}
void putEvent() {
  // Mocked response
  Serial.println("Method: putEvent");
}
void deleteEvent() {
  // Mocked response
  Serial.println("Method: deleteEvent");
}

// ---- User
void getUser() {
  // Mocked response
  Serial.println("Method: getUser");
}
void postUser() {
  // Mocked response
  Serial.println("Method: postUser");
}
void putUser() {
  // Mocked response
  Serial.println("Method: putUser");
}
void deleteUser() {
  // Mocked response
  Serial.println("Method: deleteUser");
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
