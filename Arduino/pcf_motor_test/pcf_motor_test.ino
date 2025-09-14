#include <Wire.h>
#include "PCF8575.h"
#include "PCFStepper.h"

uint8_t pcf_address = 0x20;
uint8_t led_pin = 8;
const int stepsPerRevolution = 2048;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 0, 2, 1, 3);

void setup() {
  Serial.begin(115200);
  while(!Serial);  //  uncomment when needed

  Wire.begin(9, 8);
  
  if (!PCF.begin())
  {
    Serial.println("could not initialize...");
  }
  if (!PCF.isConnected())
  {
    Serial.println("=> not connected");
    while(1);
  }

  stepper.begin();
  stepper.setSpeed(3);

  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
  delay(1000);
}

void loop() {
  Serial.println("1 = one rotation clockwise, 2 = one rotation counter-clockwise, 3 = power down, t = toggle led");
  while (Serial.available() == 0);
  switch (Serial.read())
  {
    case '1': doClockwise(); break;
    case '2': doCounterClockwise(); break;
    case '3': doPowerDown(); break;
    case 't': doToggle(); break;
  }
}

void doClockwise()
{
  // step one revolution  in one direction:
  Serial.println("clockwise");
  stepper.step(stepsPerRevolution);
}

void doCounterClockwise()
{
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  stepper.step(-stepsPerRevolution);
}

void doPowerDown()
{
  // De-energize coils
  Serial.println("powerDown");
  stepper.powerDown();
}

void doToggle()
{
  Serial.println("Toggling");
  PCF.toggle(led_pin);
  int x = PCF.read16();
  Serial.print("Read ");
  Serial.println(x, HEX);
}