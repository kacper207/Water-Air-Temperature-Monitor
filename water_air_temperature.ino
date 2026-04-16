#include <SPI.h>
#include <DMD2.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "fonts/Arial14.h"

#define ONE_WIRE_BUS 3
#define NTC_PIN A0
#define NTC_NOMINAL 10000
#define SERIES_RESISTOR 10000
#define TEMP_NOMINAL 25
#define B_COEFFICIENT 3950

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

SPIDMD dmd(2, 1);

unsigned long lastRead = 0;
float lastTempWater = -999.0;
float lastTempAir = -999.0;

#define PANEL_W 32
#define DMD_H 16

void drawStringMirrored(const char* str, int x, int y, int panelOffsetX) {
  bool buf[PANEL_W][DMD_H];

  for (int px = 0; px < PANEL_W; px++)
    for (int py = 0; py < DMD_H; py++)
      buf[px][py] = false;

  for (int px = 0; px < PANEL_W; px++)
    for (int py = 0; py < DMD_H; py++)
      dmd.setPixel(px + panelOffsetX, py, GRAPHICS_OFF);

  dmd.drawString(x + panelOffsetX, y, str);

  for (int px = 0; px < PANEL_W; px++)
    for (int py = 0; py < DMD_H; py++)
      buf[px][py] = dmd.getPixel(px + panelOffsetX, py);

  for (int px = 0; px < PANEL_W; px++)
    for (int py = 0; py < DMD_H; py++)
      dmd.setPixel(px + panelOffsetX, py, GRAPHICS_OFF);

  for (int px = 0; px < PANEL_W; px++)
    for (int py = 0; py < DMD_H; py++)
      if (buf[px][py])
        dmd.setPixel(panelOffsetX + (PANEL_W - 1) - px, (DMD_H - 1) - py, GRAPHICS_ON);
}

float readNTCTemperature() {
  int raw = analogRead(NTC_PIN);
  float resistance = SERIES_RESISTOR * (1023.0 / raw - 1.0);
  float steinhart = resistance / NTC_NOMINAL;
  steinhart = log(steinhart);
  steinhart /= B_COEFFICIENT;
  steinhart += 1.0 / (TEMP_NOMINAL + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;
  return steinhart;
}

void setup() {
  sensors.begin();
  dmd.setBrightness(200);
  dmd.begin();
  dmd.selectFont(Arial14);
}

void loop() {
  if (millis() - lastRead > 2000) {
    lastRead = millis();

    char tmp[10];
    char buf[15];

    sensors.requestTemperatures();
    float tempWater = sensors.getTempCByIndex(0);
    float tempAir = readNTCTemperature();

    // lewy panel — powietrze
    if (abs(tempAir - lastTempAir) >= 0.1) {
      lastTempAir = tempAir;
      dtostrf(tempAir, 4, 1, tmp);
      sprintf(buf, "%s\xB0", tmp);
      drawStringMirrored(buf, 2, 3, 32);
    }

    // prawy panel — woda
    if (abs(tempWater - lastTempWater) >= 0.1) {
      lastTempWater = tempWater;
      dtostrf(tempWater, 4, 1, tmp);
      sprintf(buf, "%s\xB0", tmp);
      drawStringMirrored(buf, 2, 3, 0);
    }
  }
}