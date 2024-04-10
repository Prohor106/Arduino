#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 11
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);

const int buzpin = 3;   //BUZZER
const int ledpin1 = 6;  //RED
const int ledpin2 = 5;  //GREEN
const int flamepin = A2;

const int threshold =350;  // sets threshold value for flame sensor
int flamesensvalue = 0;     // initialize flamesensor reading
int ck = 0;

void setup() {
  Serial.begin(9600);

  lcd.init();       // initialize the lcd
  lcd.backlight();  // Turn on the LCD screen backlight

  dht.begin();      // initialize the dht

  pinMode(ledpin1, OUTPUT);
  pinMode(ledpin2, OUTPUT);
  pinMode(flamepin, INPUT);
  pinMode(buzpin, OUTPUT);
}

void loop() {
  lcd.setCursor(1, 0);

            //Temperature & Humidity
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Tempar: ");
  lcd.print(t);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(h);
  lcd.print("%");

  

            //Flame

  flamesensvalue = analogRead(flamepin);  // reads analog data from flame sensor
  if (flamesensvalue <= threshold) {      // compares reading from flame sensor with the threshold value
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire!!!!");

    digitalWrite(ledpin1, HIGH); //turns on red led
    digitalWrite(ledpin2, LOW);  //turns off green led
    digitalWrite(buzpin, HIGH);  //turns on buzzer
    ck = 1;
    delay(1000);  //stops program for 1 second

  } else if(ck==1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saved...");

    digitalWrite(ledpin1, LOW);   //turns off red led
    digitalWrite(ledpin2, HIGH);  //turns on green led
    digitalWrite(buzpin, LOW);    //turns off buzzer

    ck = 0;
    delay(3000);  //stops program for 3 second
    lcd.clear();
  }


}
