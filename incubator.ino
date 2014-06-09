#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT11   

#define dhtCnt 3 

DHT dht1(3, DHTTYPE);
DHT dht2(4, DHTTYPE);
DHT dht3(5, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,20,4);

int h[dhtCnt];
int t[dhtCnt];

int redLed  = 13;
int redLed2  = 12;

bool isDanger = false;
bool signal = false;

int maxTemp = 27;
int minTemp = 20;

long previousMillis = 0;   
int interval = 100;     


void setup() {
  Serial.begin(9600); 

  dht1.begin();
  dht2.begin();
  dht3.begin();

  lcd.init(); 
  lcd.backlight();

  pinMode(redLed, OUTPUT); 
}
void loop() {  
  isDanger = false;

  h[0] = dht1.readHumidity();
  t[0] = dht1.readTemperature();

  h[1] = dht2.readHumidity();
  t[1] = dht2.readTemperature();

  h[2] = dht3.readHumidity();
  t[2] = dht3.readTemperature();
  
  lcd.clear();

  for (int i = 0; i<dhtCnt; ++i){
  	if (isnan(t[i]) || isnan(h[i])) {
    Serial.println("Failed to read from DHT");
    } else {
	  	lcd.setCursor(0, i);
	    lcd.print(i+1);
	    lcd.print(": H=");
	    lcd.print(h[i]);
	    lcd.print("% ");

	    lcd.print("T=");
	    lcd.print(t[i]);
	    lcd.print("*C");

	    if (t[i] > maxTemp || t[i] < minTemp){
	    	if (t[i] != 0)
	    		isDanger = true;
	    }

    }

  }

 if (millis() - previousMillis > interval) {
 	previousMillis = millis();   
 	signal = !signal;


 }

  digitalWrite(redLed, isDanger && signal);
  digitalWrite(redLed2, isDanger && signal);

 
}