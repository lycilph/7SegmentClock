#include <Wire.h>
#include <Bounce2.h>
#include "PCF8575.h"
#include "PCFStepper.h"

//#define SERIAL_DEBUG 1

uint8_t pcf_address = 0x20;

uint8_t next_pin = 7;
uint8_t prev_pin = 6;

const int stepsPerRevolution = 2048;
const int stepsPerNumber = 410;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 0, 2, 1, 3);

Bounce2::Button button_next = Bounce2::Button();
Bounce2::Button button_prev = Bounce2::Button();

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    while(!Serial);  //  uncomment when needed
  #endif

  Wire.begin(8, 9);
  
  if (!PCF.begin())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("could not initialize...");
    #endif
  }
  if (!PCF.isConnected())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("=> not connected");
    #endif
    while(1);
  }

  stepper.begin();
  stepper.setSpeed(10);

  button_next.attach( next_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_next.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_next.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  button_prev.attach( prev_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_prev.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_prev.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  #ifdef SERIAL_DEBUG
    int x = PCF.read16();
    Serial.print("Read ");
    Serial.println(x, HEX);
    delay(1000);
  #endif
}

void loop() {
  button_next.update();
  button_prev.update();

  if (button_next.pressed())
  {
    stepper.step(stepsPerNumber);
    stepper.powerDown();
  }
  
  if (button_prev.pressed())
  {
    stepper.step(-stepsPerNumber);
    stepper.powerDown();
  }
}