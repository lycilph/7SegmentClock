#include <Wire.h>
#include "PCF8575.h"

uint8_t pcf_address = 0x20;
uint8_t led_pin = 8;
PCF8575 PCF(pcf_address, &Wire);

void setup() {
  byte error;

  Serial.begin(115200);
  while(!Serial);  //  uncomment when needed

  Serial.println(__FILE__);
  Serial.print("PCF8575_LIB_VERSION:\t");
  Serial.println(PCF8575_LIB_VERSION);
  Serial.println();
  
  Wire.begin(9, 8);
  
  // Wire.beginTransmission(pcf_address);
  // error = Wire.endTransmission();
  // if (error == 0)
  // {
  //   Serial.println("I2C device found at address 0x20");
  // }
  // else
  // {
  //   Serial.print("Got error: ");
  //   Serial.println(error);
  // }
  // delay(200);
  
  if (!PCF.begin())
  {
    Serial.println("could not initialize...");
  }
  if (!PCF.isConnected())
  {
    Serial.println("=> not connected");
    while(1);
  }

  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
  delay(1000);
}

void loop() {
  // for(int i = 0; i < 16; i++)
  // {
  //   Serial.print("Trying address: ");
  //   Serial.println(i);

  //   PCF.write(i, LOW);
  //   delay(500);

  //   PCF.write(i, HIGH);
  //   delay(500);
  // }

  Serial.println("HLT");
  while (Serial.available() == 0);
  switch (Serial.read())
  {
    case 'H': doHigh(); break;
    case 'L': doLow(); break;
    case 'T': doToggle(); break;
  }
}

void doHigh()
{
  Serial.println("Writing HIGH");
  PCF.write(led_pin, HIGH);
  // PCF.write16(0xFFFF);
  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
}


void doLow()
{
  Serial.println("Writing LOW");
  PCF.write(led_pin, LOW);
  // PCF.write16(0);
  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
}


void doToggle()
{
  Serial.println("Toggling");
  PCF.toggle(led_pin);
  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
}
