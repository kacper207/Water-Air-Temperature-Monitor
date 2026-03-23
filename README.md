# Water-Air-Temperature-Monitor
Arduino-based dual temperature monitoring system with LCD displays for water and air temperature.


## Features
Real-time water and air temperature measurement
Dual 16x2 LCD displays (one for water, one for air)
Automatic error detection
Dual reading averaging for stability
Independent sensor monitoring

## Hardware Requirements

### Components
Arduino Uno / Seeeduino v4.3
2x LCD 16x2 (HD44780 compatible)
MAX6675 temperature sensor module with K-type thermocouple (water)
DS18B20 waterproof temperature sensor (water - alternative)
DHT11/DHT22 or DS18B20 temperature sensor (air)
Breadboard(s)
Jumper wires (male-to-male, female-to-female)
2x Potentiometer 10kΩ (optional, for contrast adjustment)
Resistor 4.7kΩ (for DS18B20 pull-up)

## Software Requirements
### Libraries
LiquidCrystal (built-in)
max6675 by Adafruit (for water sensor)
DHT by Adafruit (if using DHT11/DHT22 for air)
OneWire by Paul Stoffregen (if using DS18B20 for air)
DallasTemperature by Miles Burton (if using DS18B20 for air)
