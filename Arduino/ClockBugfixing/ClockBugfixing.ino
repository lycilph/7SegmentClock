#include <Wire.h>
#include <Bounce2.h>
#include "PCF8575.h"
#include "PCFStepper.h"
#include "RTClib.h"

#define SERIAL_DEBUG 1

uint8_t pcf_address = 0x20;

// ESP32 pins
uint8_t button1_pin = 1;
uint8_t button2_pin = 2;
uint8_t switch_pin = 3;
uint8_t led_yellow_pin = 6;
uint8_t led_green_pin = 7;

// Digit pins
uint8_t digit_led_red_pin = 0;
uint8_t digit_hall_sensor_pin = 1;

const int stepsPerRevolution = 2048;
const int stepsPerNumber = 410;

PCF8575 PCF(pcf_address, &Wire);
PCFStepper stepper(stepsPerRevolution, PCF, 4, 6, 5, 7);

Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();

RTC_DS1307 rtc;
DateTime last_time, now;
TimeSpan difference;

int8_t digit_led = HIGH;

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    while(!Serial);  //  uncomment when needed
  #endif

  Wire.begin(8, 9);
  
  #ifdef SERIAL_DEBUG
    Serial.println("Initializing PCF...");
  #endif

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

  if (! rtc.begin(&Wire)) {
    #ifdef SERIAL_DEBUG
      Serial.println("Couldn't find RTC");
    #endif
    while (1) delay(10);
  }
  
  #ifdef SERIAL_DEBUG
    Serial.println("Initializing Stepper...");
  #endif

  stepper.begin();
  stepper.setSpeed(10);
  stepper.powerDown();

  #ifdef SERIAL_DEBUG
    Serial.println("Initializing Buttons...");
  #endif

  button1.attach( button1_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button1.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button1.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  
  button2.attach( button2_pin ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button2.interval(10); // DEBOUNCE INTERVAL IN MILLISECONDS
  button2.setPressedState(LOW); // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  // Initialize digit pins
  PCF.write(digit_led_red_pin, digit_led);

  if (! rtc.isrunning()) {
    #ifdef SERIAL_DEBUG
      Serial.println("RTC is NOT running, let's set the time!");
    #endif
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  last_time = rtc.now();

  #ifdef SERIAL_DEBUG
    int x = PCF.read16();
    Serial.print("Read ");
    Serial.println(x, HEX);
    delay(1000);
  #endif
}

void loop() {
  now = rtc.now();
  difference = now - last_time;

  if (difference.seconds() > 1)
  {
    last_time = now;
    
    #ifdef SERIAL_DEBUG
      char buf2[] = "YYMMDD-hh:mm:ss";
      Serial.println(now.toString(buf2));
    #endif
  }

  button1.update();
  button2.update();

  if (button1.pressed())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Button 1 pressed");
    #endif
    stepper.step(stepsPerNumber);
    stepper.powerDown();
  }
  if (button2.pressed())
  {
    #ifdef SERIAL_DEBUG
      Serial.println("Button 2 pressed");
    #endif
    stepper.step(-stepsPerNumber);
    stepper.powerDown();
  }
}