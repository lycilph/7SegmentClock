/*
 * Modified from:
 * Stepper.h - Stepper library for Wiring/Arduino - Version 1.1.0
 *
 * The circuits can be found at
 * https://docs.arduino.cc/learn/electronics/stepper-motors#circuit
 */

// ensure this library description is only included once
#ifndef PCFStepper_h
#define PCFStepper_h

#include <PCF8575.h>

// library interface description
class PCFStepper {
  public:
    // constructors:
    PCFStepper(int number_of_steps, PCF8575& pcf8575, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4);

    void begin();

    // speed setter method:
    void setSpeed(long whatSpeed);

    // mover method:
    void step(int number_of_steps);

    void powerDown();

  private:
    void stepMotor(int this_step);

    PCF8575& pcf8575;  // Reference to PCF8575 instance

    int direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in us, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int step_number;          // which step the motor is on

    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;

    unsigned long last_step_time; // timestamp in us of when the last step was taken
};

#endif