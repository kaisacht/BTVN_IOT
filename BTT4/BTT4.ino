#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"
LiquidCrystal_I2C lcd(0x27,20,4);
#define DHT_PIN 15
#define LED_PIN 26
#define PIR_PIN 13
DHTesp dhtSensor;

void setup() {
  // put your setup code here, to run once:
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
}
int value = 1 ;
void loop() {
  // put your main code here, to run repeatedly:
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  int temp = data.temperature;
  int humid = data.humidity;
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
