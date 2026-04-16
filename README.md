# LED Temperature Display

Dual-panel LED matrix display showing water and air temperature in real time, built on Arduino.

## Hardware

| Component | Description |
|---|---|
| LED Moduł Panel P10 Czerwony HUB12 DIP 32x16 | Water temperature display panel |
| LED Dot Matrix Panel 32x16cm P10 HUB12 Green | Air temperature display panel |
| Przewód IDC 16-pin żeńsko-żeński 30cm (x2) | Panel data cables |
| Zasilacz impulsowy LRS-75-5 75W 5V 14A | 5V power supply for panels |
| Sonda wodoodporna DS18B20 Okystar 5m | Waterproof water temperature sensor |
| Termistor NTC 110 100K z przewodem 1m | Air temperature sensor |
| Wtyk IDC 16-pin prosty (5 szt.) | IDC connectors |
| Zestaw rezystorów JustPi CF THT 1/4W 200szt. | Resistors (10kΩ for NTC voltage divider) |
| Zestaw szybkozłączek Wago 221-412/413/415 | WAGO connectors for wiring |

## Wiring

### DS18B20 (Water Temperature)
- Data pin → Arduino **pin 3** (with 4.7kΩ pull-up resistor to 5V)
- VCC → 5V
- GND → GND

### NTC Thermistor 100K (Air Temperature)
Connected as a voltage divider with a 10kΩ series resistor:

```
5V ──── [NTC 100K] ──┬── [10kΩ resistor] ──── GND
                     │
                    A0 (Arduino)
```

Using WAGO connectors:
- **WAGO 1**: 5V rail + red NTC wire
- **WAGO 2**: black NTC wire + 10kΩ resistor leg 1 + wire to A0
- **WAGO 3**: GND rail + 10kΩ resistor leg 2

### LED Panels
Both panels are connected in daisy-chain via IDC cable. The first panel connects to Arduino via SPI (DMD2 library default pins). Both panels share the same data/clock lines.

## Software

### Dependencies
- [DMD2](https://github.com/freetronics/DMD2) — LED matrix driver
- [OneWire](https://github.com/PaulStoffregen/OneWire) — DS18B20 communication
- [DallasTemperature](https://github.com/milesburton/Arduino-Temperature-Control-Library) — DS18B20 temperature reading
- Font: `Arial14` (included with DMD2)

### Configuration

```cpp
#define ONE_WIRE_BUS 3        // DS18B20 data pin
#define NTC_PIN A0            // NTC analog input pin
#define NTC_NOMINAL 10000     // NTC resistance at 25°C (10kΩ)
#define SERIES_RESISTOR 10000 // Series resistor value (10kΩ)
#define TEMP_NOMINAL 25       // Reference temperature (°C)
#define B_COEFFICIENT 3950    // NTC B coefficient — adjust for your sensor
```


## How It Works

- **Left panel** — air temperature (NTC thermistor), read via analog pin and Steinhart-Hart equation
- **Right panel** — water temperature (DS18B20), read via OneWire protocol
- Both temperatures are read every **2 seconds**
- Display only updates when temperature changes by **≥ 0.1°C** to minimize flicker
- Text is rendered mirrored (180° rotated) to correct for physical panel orientation
- Temperature is shown with one decimal place followed by a degree symbol (e.g. `23.4°`)

## Notes

- Panels are physically mounted upside-down, hence the pixel mirroring in `drawStringMirrored()`
- `noInterrupts()` / `interrupts()` is used during pixel updates to prevent partial-frame flicker caused by DMD2's background refresh timer
- Power the LED panels from the external 5V supply, not from the Arduino's 5V pin
