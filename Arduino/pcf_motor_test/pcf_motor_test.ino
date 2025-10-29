#include <Wire.h>
#include <Bounce2.h>
#include "PCF8575.h"
#include "PCFStepper.h"

//#define SERIAL_DEBUG 1

uint8_t pcf_address = 0x20;
uint8_t led_pin = 8;
uint8_t ok_led_pin = 11;
uint8_t home_10_pin = 5;
uint8_t home_50_pin = 6;
uint8_t step_pin = 7;
uint8_t switch_pin = 4;
const int stepsPerRevolution = 2048;
const int stepsPerNumber = 410;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 0, 2, 1, 3);

Bounce2::Button button_home_10 = Bounce2::Button();
Bounce2::Button button_home_50 = Bounce2::Button();
Bounce2::Button button_step = Bounce2::Button();

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    while(!Serial);  //  uncomment when needed
  #endif
  
  pinMode(ok_led_pin, OUTPUT);
  digitalWrite(ok_led_pin, LOW);
  delay(1000);

  Wire.begin(9, 8);
  
  if (!PCF.begin())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("could not initialize...");
    #endif
    digitalWrite(ok_led_pin, HIGH);
  }
  if (!PCF.isConnected())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("=> not connected");
    #endif
    digitalWrite(ok_led_pin, HIGH);
    while(1);
  }

  stepper.begin();
  stepper.setSpeed(10);

  button_home_10.attach( home_10_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_home_50.attach( home_50_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button_step.attach( step_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  
  button_home_10.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_home_50.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button_step.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  
  button_home_10.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button_home_50.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button_step.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  pinMode(switch_pin, INPUT);

  #ifdef SERIAL_DEBUG
    int x = PCF.read16();
    Serial.print("Read ");
    Serial.println(x, HEX);
    delay(1000);
  #endif
}

void loop() {
  button_home_10.update();
  button_home_50.update();
  button_step.update();

  int switchState = digitalRead(switch_pin); // HIGH = 3.3V side, LOW = GND side
  int direction = 1;

  if (switchState == HIGH)
  {
    direction = -1;
  }
  else
  {
    direction = 1;
  }

  if (button_step.pressed())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Stepping 1 number");
    #endif
    PCF.write(led_pin, LOW);
    
    stepper.step(direction*stepsPerNumber);
    stepper.powerDown();

    PCF.write(led_pin, HIGH);
  }

  if (button_home_10.pressed())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Homing 10 steps");
    #endif

    stepper.step(direction*10);
    stepper.powerDown();
  }
  
  if (button_home_50.pressed())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Homing 50 steps");
    #endif

    stepper.step(direction*50);
    stepper.powerDown();
  }
}