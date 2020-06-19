//
//    FILE: mcp9808.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: Arduino Library for I2C mcp9808 temperature sensor
//    DATE: 2020-05-03
//     URL: https://github.com/RobTillaart/MCP9808_RT
//
// HISTORY:
// 0.1.0    2020-05-03
//

#include "mcp9808.h"

#define MCP9808_RFU     0x00
#define MCP9808_CONFIG  0x01
#define MCP9808_TUPPER  0x02
#define MCP9808_TLOWER  0x03
#define MCP9808_TCRIT   0x04
#define MCP9808_TA      0x05
#define MCP9808_MID     0x06
#define MCP9808_DID     0x07
#define MCP9808_RES     0x08

/*
  0000 = RFU, Reserved for Future Use (Read-Only register)
  0001 = Configuration register (CONFIG)
  0010 = Alert Temperature Upper Boundary Trip register (TUPPER)
  0011 = Alert Temperature Lower Boundary Trip register (TLOWER)
  0100 = Critical Temperature Trip register (TCRIT)
  0101 = Temperature register (TA)
  0110 = Manufacturer ID register
  0111 = Device ID/Revision register
  1000 = Resolution register
  1xxx = Reserved(1)
*/

#if defined(ESP8266) || defined(ESP32)
MCP9808::MCP9808(uint8_t addr, const uint8_t dataPin, const uint8_t clockPin)
{
  _address = address;
  _wire = &Wire;

  if ((dataPin < 255) && (clockPin < 255))
  {
    _wire->begin(dataPin, clockPin);
  } else {
    _wire->begin();
  }
}
#endif

MCP9808::MCP9808(uint8_t addr)
{
  MCP9808::setAddress(addr, &Wire);
}

void MCP9808::setAddress(const uint8_t address, TwoWire *wire)
{
  _address = address;
  _wire = wire;
  _wire->begin();
}

void MCP9808::setConfigRegister(uint16_t cfg)
{
  writeReg16(MCP9808_CONFIG, cfg);
}

uint16_t MCP9808::getConfigRegister()
{
  return readReg16(MCP9808_CONFIG);
}

void  MCP9808::setTupper(float f)
{
  writeFloat(MCP9808_TUPPER, f);
}

float MCP9808::getTupper()
{
  return readFloat(MCP9808_TUPPER);
}

void  MCP9808::setTlower(float f)
{
  writeFloat(MCP9808_TLOWER, f);
}

float MCP9808::getTlower()
{
  return readFloat(MCP9808_TLOWER);
}

void  MCP9808::setTcritical(float f)
{
  writeFloat(MCP9808_TCRIT, f);
}

float MCP9808::getTcritical()
{
  return readFloat(MCP9808_TCRIT);
}

void MCP9808::setAlertPin(uint8_t pin)
{
  _alertPin = pin;
  pinMode(pin, INPUT);
}

bool MCP9808::hasAlert()
{
  return (digitalRead(_alertPin) != LOW);  // alert polarity? HIGH LOW
}

float MCP9808::getTemperature()
{
  return readFloat(MCP9808_TA);
}

uint8_t MCP9808::getStatus()
{
  // readFloat(MCP9808_TA);
  return _status;
}

uint16_t MCP9808::getManufacturerID()
{
  return readReg16(MCP9808_MID);
}

uint8_t MCP9808::getDeviceID()
{
  return readReg16(MCP9808_MID) >> 8;
}

uint8_t MCP9808::getRevision()
{
  return readReg16(MCP9808_MID) & 0xFF;
}

void MCP9808::setResolution(uint8_t res)
{
  if (res < 4) writeReg8(MCP9808_RES, res);
}

uint8_t MCP9808::getResolution()
{
  return readReg8(MCP9808_RES);
}

////////////////////////////////////////////////////////////////
//
// PRIVATE
//
void MCP9808::writeFloat(uint8_t reg, float f)
{
  // no range checking for f.
  bool neg = (f < 0.0);
  if (neg) f = -f;
  uint16_t val = uint16_t(f * 4 + 0.5) * 4;
  if (neg) val |= 0x1000;
  writeReg16(reg, val);
}

float MCP9808::readFloat(uint8_t reg)
{
  uint16_t val = readReg16(reg);
  if (reg == MCP9808_TA)
  {
    _status = (val & 0xE000) >> 8;          // _status = ...x xxxx
  }
  if (val & 0x1000)
  {
    return 256 - (val & 0x0FFF) * 0.0625;
  }
  return (val & 0x0FFF) * 0.0625;         // todo optimize same part?
}


void MCP9808::writeReg8(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t MCP9808::readReg8(uint8_t reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, (uint8_t)1);
  return Wire.read();
}

void MCP9808::writeReg16(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value >> 8);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

uint16_t MCP9808::readReg16(uint8_t reg)
{
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(_address, (uint8_t)2);
  uint16_t val = Wire.read() << 8;
  val += Wire.read();
  return val;
}

// -- END OF FILE --
