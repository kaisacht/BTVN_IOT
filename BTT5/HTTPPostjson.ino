#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27,20,4);
#define DHT_PIN 15
#define LED_PIN 26
#define PIR_PIN 13
DHTesp dhtSensor;
const String serverName = "http://postman-echo.com/post";
void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "",6);
  // put your setup code here, to run once:
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
}
int value = 1 ;
void loop() {
  HTTPClient http;
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  int temp = data.temperature;
  int humid = data.humidity;
  if (WiFi.status() ==  WL_CONNECTED){
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    DynamicJsonDocument doc(1024);
    String jsonstr;
    JsonObject root = doc.to<JsonObject>();
    root["temperature"] = temp;
    root["humidity"] = humid;
    serializeJson(doc, jsonstr);

String httpRequestData = jsonstr;
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode == 200){
      String payload  = http.getString();
      Serial.println(payload);
    } else{
      Serial.println("error");
      Serial.println(httpResponseCode);
    }
    delay(3000);
  }
  // put your main code here, to run repeatedly:
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Temperature:" + String(temp) +"C");
  lcd.setCursor(0,1);
  lcd.print("Humidity:" +  String(humid) + "%");
  
  value = digitalRead(PIR_PIN);
  if(value == 1){
    digitalWrite(LED_PIN, HIGH);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
