#include <Wire.h>
#include <Bounce2.h>
#include "PCF8575.h"
#include "PCFStepper.h"

// #define SERIAL_DEBUG 1

uint8_t pcf_address = 0x20;

// ESP32 pins
uint8_t start_button_pin = 1;
uint8_t running_led_pin = 6;
uint8_t step_button_pin = 5;

const int stepsPerRevolution = 2048;
const int stepsPerNumber = 410;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 4, 6, 5, 7);

Bounce2::Button button_start = Bounce2::Button();
Bounce2::Button button_step = Bounce2::Button();

bool is_running = false;
int8_t direction = 1;

void setup() {
  // #ifdef SERIAL_DEBUG
  //   Serial.begin(115200);
  //   while(!Serial);  //  uncomment when needed
  // #endif

  Wire.begin(8, 9);
  
  if (!PCF.begin())
  {
    // #ifdef SERIAL_DEBUG
    //   Serial.println("could not initialize...");
    // #endif
  }
  if (!PCF.isConnected())
  {
    // #ifdef SERIAL_DEBUG
    //   Serial.println("=> not connected");
    // #endif
    while(1);
  }

  stepper.begin();
  stepper.setSpeed(10);

  button_start.attach( start_button_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_start.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_start.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  
  button_step.attach( step_button_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_step.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_step.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  pinMode(running_led_pin, OUTPUT);
  digitalWrite(running_led_pin, LOW);

  // #ifdef SERIAL_DEBUG
  //   int x = PCF.read16();
  //   Serial.print("Read ");
  //   Serial.println(x, HEX);
  //   delay(1000);
  // #endif
}

void loop() {
  button_start.update();
  button_step.update();

  if (button_start.pressed() && is_running == false)
  {
    digitalWrite(running_led_pin, HIGH);
    is_running = true;
    direction *= -1;
  }

  if (button_step.pressed() && is_running == true)
  {
    digitalWrite(running_led_pin, LOW);
    is_running = false;

    stepper.powerDown();
  }

  if (is_running)
  {
    stepper.step(direction*1);
  }
}