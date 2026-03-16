#ifndef KITRONIK_MOTORS_H
#define KITRONIK_MOTORS_H

#include "MicroBit.h"

extern MicroBit uBit;

// Register addresses
#define PRESCALE_REG 0xFE
#define MODE1_REG 0x00
#define SRV_REG_BASE 0x08
#define MOTOR_REG_BASE 0x28
#define REG_OFFSET 4
#define SERVO_MULTI 226
#define SERVO_ZERO_OFFSET 0x66

// Define Variables
int chipAddress = 0x6C;
bool initialised = false;
bool stepInit = false;
int stepStage = 0;
int stepper1Steps = 200;
int stepper2Steps = 200;

enum Direction { FORWARD, REVERSE };

class KitronikRoboticsBoard {
    public:
        KitronikRoboticsBoard();         // constructor
        void servoWrite(int servo, int degrees);
        void motorOn(int motor, Direction direction, int speed);
        void motorOff(int motor);
        void allOff();
        void stepperMotorTurnAngle(int stepper, Direction direction, int angle);
        void stepperMotorTurnSteps(int stepper, Direction direction, int steps);

    private:
        void turnStepperMotor(int stepper, Direction direction, int steps);
        int stepStage;
        bool stepInit;
        int stepper1Steps;
        int stepper2Steps;
};

#endif