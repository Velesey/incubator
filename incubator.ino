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

int maxTemp = 39;
int minTemp = 33;

int maxHum = 65;
int humPin = 10;

int minTempPin = A0;
int maxTempPin = A1;

long previousMillis = 0;   
int interval = 100;     

volatile long millisSleepDanger = 0;
long timerSleepDanger;
bool isMute = false;

void setup() {
  Serial.begin(9600); 

  dht1.begin();
  dht2.begin();
  dht3.begin();

  lcd.init(); 
  lcd.backlight();

  
  pinMode(redLed, OUTPUT);
  pinMode(redLed2, OUTPUT); 
  pinMode(humPin, OUTPUT); 
  pinMode(minTempPin, INPUT);

  attachInterrupt(0, mute, FALLING);
  timerSleepDanger = 300000;
}
void loop() {  
  setMinMaxTemp();
  isDanger = false;
  isMute = millis() < millisSleepDanger;
  Serial.println(millis());
  Serial.println(millisSleepDanger);
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

      Serial.print(i+1);
      Serial.print(": H=");
      Serial.print(h[i]);
      Serial.print("% ");

      Serial.print("T=");
      Serial.print(t[i]);
      Serial.println("*C");


	    if (t[i] >= maxTemp || t[i] <= minTemp){
	    	if (t[i] != 0 && t[i] != 255){
          if (!isMute){
            isDanger = true;
            Serial.println("danger");
          }
        }
	    	
	    }

    } 

  }
  lcd.setCursor(0, 3);
  lcd.print("min:");
  lcd.print(minTemp);
  lcd.print("*C");
  
  lcd.print(" max:");
  lcd.print(maxTemp);
  lcd.print("*C");
  if (isMute)
  lcd.print("(-)");

  Serial.print("min:");
  Serial.print(minTemp);
  Serial.print("*C");
  
  Serial.print(" max:");
  Serial.print(maxTemp);
  Serial.println("*C");


	    

	 

 if (millis() - previousMillis > interval) {
 	previousMillis = millis();   
 	signal = !signal;


 }
  digitalWrite(humPin,  h[0] >= maxHum);
  digitalWrite(redLed, isDanger && signal);
  digitalWrite(redLed2,  isDanger && signal);

}
void mute()
{
  millisSleepDanger = millis() + timerSleepDanger;
  Serial.println("pushed");
}

void setMinMaxTemp(){
minTemp = map(analogRead(minTempPin), 0, 1020, 30, 35);
maxTemp = map(analogRead(maxTempPin), 0, 1020, 35, 40);
}