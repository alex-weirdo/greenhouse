#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define LIGHT D5
#define RED_LED D0
#define GREEN_LED D1

const char* ssid="";
const char* key="";

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
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://example.com/testJson.json");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      String lamp_state = root["light"];

		if (lamp_state.equals("On")){
		  digitalWrite(LIGHT, HIGH);
		} else if (lamp_state.equals("SOS")){
			  // just for fun
			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(500);

			  digitalWrite(LIGHT, HIGH);
			  delay(500);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(500);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(500);
			  digitalWrite(LIGHT, LOW);
			  delay(500);

			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
			  digitalWrite(LIGHT, HIGH);
			  delay(250);
			  digitalWrite(LIGHT, LOW);
			  delay(250);
		} else {
		  digitalWrite(LIGHT, LOW);
		}
    
      // Output to serial monitor
      Serial.print("Lamp is ");
      Serial.println(lamp_state);
    }
    http.end();   //Close connection
  } else {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  delay(1000);
}
