#include <Wire.h>
#include <Bounce2.h>
#include "PCF8575.h"
#include "PCFStepper.h"
#include "RTClib.h"

// #define SERIAL_DEBUG 1

uint8_t pcf_address = 0x20;

// ESP32 pins
uint8_t start_button_pin = 7;
uint8_t running_led_pin = 6;
uint8_t step_button_pin = 5;

// PCF pins
uint8_t hall_sensor_pin = 8;

const int stepsPerRevolution = 2048;
const int stepsPerNumber = 410;

RTC_DS1307 rtc;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 0, 2, 1, 3);

Bounce2::Button button_start = Bounce2::Button();
Bounce2::Button button_step = Bounce2::Button();

bool is_running = false;
DateTime last_time;
TimeSpan difference;

void setup() {
  // #ifdef SERIAL_DEBUG
  //   Serial.begin(115200);
  //   while(!Serial);  //  uncomment when needed
  // #endif

  Wire.begin(8, 9);

  if (! rtc.begin(&Wire)) {
    // Serial.println("Couldn't find RTC");
    // Serial.flush();
    // while (1) delay(10);
  }
  
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
  
  if (! rtc.isrunning()) {
    // Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  last_time = rtc.now();

  // #ifdef SERIAL_DEBUG
  //   int x = PCF.read16();
  //   Serial.print("Read ");
  //   Serial.println(x, HEX);
  //   delay(1000);
  // #endif
}

void loop() {
  DateTime now = rtc.now();
  difference = now - last_time;

  button_start.update();
  button_step.update();

  if (button_start.pressed() && is_running == false)
  {
    digitalWrite(running_led_pin, HIGH);
    is_running = true;

    // #ifdef SERIAL_DEBUG
    //   Serial.println("Button pressed...");
    // #endif
  }

  if (button_step.pressed())
  {
    digitalWrite(running_led_pin, HIGH);

    stepper.step(stepsPerNumber);
    stepper.powerDown();

    digitalWrite(running_led_pin, LOW);
  }

  if (PCF.read(hall_sensor_pin) == LOW && is_running == true)
  {
    digitalWrite(running_led_pin, LOW);
    is_running = false;
    stepper.powerDown();

    // #ifdef SERIAL_DEBUG
    //   Serial.println("Hall sensor triggered...");
    // #endif
  }

  if (is_running)
  {
    stepper.step(1);
  }

  if (!is_running && difference.seconds() > 10)
  {
    last_time = now;

    digitalWrite(running_led_pin, HIGH);

    stepper.step(stepsPerNumber);
    stepper.powerDown();

    digitalWrite(running_led_pin, LOW);
  }
}