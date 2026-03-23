#include <LiquidCrystal.h>
#include <max6675.h>

#define CONTRAST_PIN 9

#define thermoDO 12  
#define thermoCS 10  
#define thermoCLK 13 

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

void setup() {
  analogWrite(CONTRAST_PIN, 95);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.noCursor();
  lcd.noBlink();

  lcd.print("Temp. wody:");
  delay(2000);
  lcd.clear();
  
  delay(500);
}

void loop() {
  lcd.noDisplay();
  
  delay(250);
  
  float tempC = thermocouple.readCelsius();
  
  delay(250);
  float tempC2 = thermocouple.readCelsius();
  
  float avgTemp = (tempC + tempC2) / 2.0;
  
  lcd.display();
  delay(50);
  
  if (isnan(avgTemp) || avgTemp < -50 || avgTemp > 1000) {
    lcd.clear();
    lcd.print("Blad czujnika!");
    lcd.setCursor(0, 1);
    lcd.print("Sprawdz polacz.");
    delay(1000);
    return;
  }
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");
  
  lcd.setCursor(0, 1);
  lcd.print(avgTemp, 1);
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  
  delay(1000);
}