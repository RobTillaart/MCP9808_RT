# MCP9808_RT

Arduino library for I2C MCP9808 temperature sensor

## Description

Experimental version

The MCP9808 is a temperature sensor that measures typically in 1/16th of a degree celsius.


### MCP9808 breakout board
```
//
//  MCP9808 breakout board
//  +----------+
//  |0   ALERT |---------------+--[ 4K7 ]---- +5V
//  |       A2 |---- GND       |
//  |       A1 |---- GND       +--[ LED ]---- GND  // or other electronics.
//  |       A0 |---- GND
//  |      SDA |---- I2C MCU
//  |      SCL |---- I2C MCU
//  |      GND |---- GND MCU
//  |0     VCC |---- +5V
//  +----------+
//
//  address above is 24
//
```


## Interface

### Constructor
- **MCP9808(const uint8_t addr)** constructor for e.g. UNO
- **MCP9808(const uint8_t addr, const uint8_t dataPin = 255, const uint8_t clockPin = 255)** constructor for ESP32 and ESP8266
- **setAddress(const uint8_t address, TwoWire \*wire)** if multiple I2C busses are present one can choose.

**Address**
max 8 sensors on one I2C bus - 00011xxx where xxx = A2, A1, A0

| Address | HEX | A2 | A1 | A0 |
|:----:|:----:|:----:|:----:|:----:|
| 24 | 0x18 | 0 | 0 | 0 |
| 25 | 0x19 | 0 | 0 | 1 |
| 26 | 0x1A | 0 | 1 | 0 |
| 27 | 0x1B | 0 | 1 | 1 |
| 28 | 0x1C | 1 | 0 | 0 |
| 29 | 0x1D | 1 | 0 | 1 |
| 30 | 0x1E | 1 | 1 | 0 |
| 31 | 0x1F | 1 | 1 | 1 |



### Temperature and status
- **setOffset(float f)** set an offset to calibrate or to correct for self heating
- **getOffset()** return value of offset (default 0);
- **getTemperature()** read the ambient temperature.
- **getStatus()** returns the status bits of the last call to **getTemperature()**

The value returned by **getStatus()** is the last value read by the call to **GetTemperature()**.  There are three bits, see table below. A value of 6 means that TA is above the upper and above the critical temp.

| Bit  | Mask | Description | Notes |
|:----:|:----:|:----|:----|
| 0    | 0x01 | TA < TLOWER | lower  |
| 1    | 0x02 | TA > TUPPER | larger |
| 2    | 0x04 | TA ≥ TCRIT  | **larger or equal** |




### Resolution
- **setResolution(uint8_t res)** set the resolution, 
- **getResolution()** returns the resolution set

| Value  | Resolution | Conv time (ms) | Samples/s | Notes |
|:----:|:----|:----:|:----:|:----:|
| 0    | 0.5°C    | 30 | 33 ||
| 1    | 0.25°C   | 65 | 15 ||
| 2    | 0.125°C  | 130 | 7 ||
| 3    | 0.0625°C | 250 | 4 | **default** |



### Config
- **setConfigRegister(uint16_t cfg)**
- **getConfigRegister()**

| Bit  | Mask | Name | Description | Value |
|:----:|:----:|:----|:----|:----|
| 0    | 0x0001 | ALT MOD  | alert mode      | **0 = comparator output**, 1 = interrupt output |
| 1    | 0x0002 | ALT POL  | alert polarity  | **0 = active low**, 1 = active high|
| 2    | 0x0004 | ALT SEL  | alert select    | **0 = upper+lower+crit**,  1 = crit only|
| 3    | 0x0008 | ALT CNT  | alert control   | **0 = OFF**,  1 = ON |
| 4    | 0x0010 | ALT STAT | alert status    | **0 = OFF**,  1 = ON  (read!) |
| 5    | 0x0020 | INT CLR  | interrupt clear | **0 = none**, 1 = clear interrupt |
| 6    | 0x0040 | WIN LOC  | lock Tup + Tlow | **0 = unlocked**, 1 = Lock |
| 7    | 0x0080 | CRT LOC  | lock Tcritical  | **0 = unlocked**, 1 = Lock |
| 8    | 0x0100 | SHDN     | shutdown,       | **0 = continuous mode**,  1 = low power |
| 9-10 | 0x0600 | Hysteresis | Thysteresis   | **00 = 0°C**, 01 = 1.5°C, 10 = 3°C, 11 = 6°C |
| 11-15 |        | always 0 | |

Check datasheet for the details...



### Temperature limits / thresholds
- **setTupper(float f)** write upper register, accuracy 0.25°C
- **getTupper()** idem
- **setTlower(float f)** write lower register, accuracy 0.25°C
- **getTlower()** idem
- **setTcritical(float f)** write critical register, accuracy 0.25°C
- **getTcritical()** idem



### MCU alert pin (experimental)
- **setAlertPin(uint8_t pin)** set the (interrupt) pin of the MCU that is connected
to the Alertpin of the MCP9808
- **hasAlert()** read the alert pin above. initial minimalistic version that has no
control over polarity of the alert etc.



### Miscelaneous
- **getManufacturerID()** returns 84 (my version)
- **getDeviceID()** returns 0 (my version)
- **getRevision()** returns 84 (my version)



## Operation

See examples
