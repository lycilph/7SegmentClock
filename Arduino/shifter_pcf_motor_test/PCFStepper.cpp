#include "Arduino.h"
#include "PCFStepper.h"

/*
 *   constructor for four-pin version
 *   Sets which wires should control the motor.
 */
PCFStepper::PCFStepper(int number_of_steps, PCF8575& pcf8575,
                 int motor_pin_1, int motor_pin_2,
                 int motor_pin_3, int motor_pin_4)
  : pcf8575(pcf8575)
{
  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // timestamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;
}

void PCFStepper::begin()
{
  // Initialize stepper pins to LOW (outputs, coils off)
  pcf8575.write(this->motor_pin_1, LOW);
  pcf8575.write(this->motor_pin_2, LOW);
  pcf8575.write(this->motor_pin_3, LOW);
  pcf8575.write(this->motor_pin_4, LOW);
}

/*
 * Sets the speed in revs per minute
 */
void PCFStepper::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void PCFStepper::step(int steps_to_move)
{
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
      stepMotor(this->step_number % 4);
    } else {
      yield();
    }
  }
}

void PCFStepper::powerDown()
{
  // Turn off coils to save power
  pcf8575.write(motor_pin_1, LOW);
  pcf8575.write(motor_pin_2, LOW);
  pcf8575.write(motor_pin_3, LOW);
  pcf8575.write(motor_pin_4, LOW);
}

/*
 * Moves the motor forward or backwards.
 */
void PCFStepper::stepMotor(int thisStep)
{
  switch (thisStep) {
    case 0:  // 1010
      pcf8575.write(motor_pin_1, HIGH);
      pcf8575.write(motor_pin_2, LOW);
      pcf8575.write(motor_pin_3, HIGH);
      pcf8575.write(motor_pin_4, LOW);
    break;
    case 1:  // 0110
      pcf8575.write(motor_pin_1, LOW);
      pcf8575.write(motor_pin_2, HIGH);
      pcf8575.write(motor_pin_3, HIGH);
      pcf8575.write(motor_pin_4, LOW);
    break;
    case 2:  //0101
      pcf8575.write(motor_pin_1, LOW);
      pcf8575.write(motor_pin_2, HIGH);
      pcf8575.write(motor_pin_3, LOW);
      pcf8575.write(motor_pin_4, HIGH);
    break;
    case 3:  //1001
      pcf8575.write(motor_pin_1, HIGH);
      pcf8575.write(motor_pin_2, LOW);
      pcf8575.write(motor_pin_3, LOW);
      pcf8575.write(motor_pin_4, HIGH);
    break;
  }
}