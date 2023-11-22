#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27,20,4);
#define DHT_PIN 15
#define LED_PIN 26
#define PIR_PIN 13

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_PIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_PIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/IOT/T6", "1");
      // ... and resubscribe
      client.subscribe("/IOT/T6");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
DHTesp dhtSensor;
void setup() {
  Serial.begin(11150);
  WiFi.begin("Wokwi-GUEST", "",6);
  // put your setup code here, to run once:
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lcd.init(); 
  lcd.backlight();
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  int temp = data.temperature;
  int humid = data.humidity;
  lcd.setCursor(0,0);
  lcd.print("Temperature:" + String(temp) +"C");
  lcd.setCursor(0,1);
  lcd.print("Humidity:" +  String(humid) + "%");
  String mg = "Temperature: " + String(temp) + "C  Humidity: " + String(humid) + "%";
  unsigned long now = millis();
  ++value;
  // Convert 'value' to a C-style string
  char valueStr[10]; // Assuming a maximum length for the number
  snprintf(valueStr, sizeof(valueStr), "%d", value);
  // Concatenate 'valueStr' to 'mg'
  mg += " Value: ";
  mg += valueStr;
  // Convert 'mg' (String) to a C-style string (char array)
  char msg[MSG_BUFFER_SIZE];
  mg.toCharArray(msg, MSG_BUFFER_SIZE);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("/IOT/T6", msg);
  delay(5000);
}
