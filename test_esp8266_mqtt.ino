#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define LIGHT D5
#define RED_LED D0
#define GREEN_LED D1
#define TEMP_SENSOR A0

const char* ssid="";
const char* key="";

const char *mqtt_server = ""; // Имя сервера MQTT
const int mqtt_port = 1883; // Порт для подключения к серверу MQTT
const char *mqtt_user = ""; // Логин от сервер
const char *mqtt_pass = ""; // Пароль от сервера

int temp_timer = 0;

void callback(const MQTT::Publish& pub)
{
    Serial.print(pub.topic());
    Serial.print(" => ");
    Serial.println(pub.payload_string());
    
    String payload = pub.payload_string();
    
    if(String(pub.topic()) == "home/light")
    {
        int stlight = payload.toInt();
        digitalWrite(LIGHT, stlight);
    }
}


WiFiClient wclient;      
PubSubClient client(wclient, mqtt_server, mqtt_port);


void setup() {
  Serial.begin(115200);
  
  pinMode(LIGHT, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  WiFi.begin(ssid, key);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  Serial.println("WiFi connected"); 
}

void loop() {
    // подключаемся к MQTT серверу
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            digitalWrite(RED_LED, HIGH);
            digitalWrite(GREEN_LED, LOW);
            Serial.println("Connecting to MQTT server");
            if (client.connect(MQTT::Connect("NodeMCU_client")
                                 .set_auth(mqtt_user, mqtt_pass))) {
                Serial.println("Connected to MQTT server");
                client.set_callback(callback);
                client.subscribe("home/light");
            } else {
                Serial.println("Could not connect to MQTT server");   
            }
        }
        
        if (client.connected()){
            client.loop();
            sendTemp();
            digitalWrite(RED_LED, LOW);
            digitalWrite(GREEN_LED, HIGH);
        }
    }
}

void sendTemp() {
  if (temp_timer >= 50000) {
    int reading = analogRead(TEMP_SENSOR);
    float voltage = reading * 3.3;
    voltage /= 1024.0;
    float temerature = (voltage - 0.5) * 100;
    client.publish("home/temp", String(temerature));
    Serial.println(String(temerature));
    temp_timer = 0;
  } else {
    temp_timer++;
  }
}