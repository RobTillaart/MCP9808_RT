//
//    FILE: mcp9808_test.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: demo
//    DATE: 2020-05-03
//    (c) : MIT
//

#include "mcp9808.h"

MCP9808 ts(0x24);

uint32_t lastTemp = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);

  Serial.print("CONFIG:   ");
  Serial.println(ts.getConfigRegister());
  Serial.print("TUPPER:   ");
  Serial.println(ts.getTupper());
  Serial.print("WLOWER:   ");
  Serial.println(ts.getTlower());
  Serial.print("TCRIT:    ");
  Serial.println(ts.getTcritical());
  Serial.print("TAMBIENT: ");
  Serial.println(ts.getTemperature());
  Serial.print("Resolut:  ");
  Serial.println(ts.getResolution());
  Serial.print("Manufact: ");
  Serial.println(ts.getManufacturerID());
  Serial.print("DeviceID: ");
  Serial.println(ts.getDeviceID());
  Serial.print("Revision: ");
  Serial.println(ts.getRevision());

}

void loop()
{
  uint32_t now = millis();
  if (now - lastTemp > 2000)
  {
    lastTemp = now;
    Serial.print("TAMBIENT: ");
    Serial.println(ts.getTemperature());   
  }
  // ...
}
