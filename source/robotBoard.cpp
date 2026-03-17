#include "MicroBit.h"
#include "robotBoard.h"

KitronikRoboticsBoard::KitronikRoboticsBoard() {
    uint8_t buf[2];
    buf[0] = PRESCALE_REG;
    buf[1] = 0x7D;
    uBit.i2c.write(chipAddress << 1, buf, 2);

    for (int blockReg = 0xFA; blockReg < 0xFE; blockReg++) {
    buf[0] = blockReg;
    buf[1] = 0x00;
    uBit.i2c.write(chipAddress << 1, buf, 2);
    }
}

void KitronikRoboticsBoard::servoWrite(int servo, int degrees) {
    uint8_t buf[2];
    int calcServo = SRV_REG_BASE + ((servo - 1) *REG_OFFSET);
    bool HighByte = false;
    int PWMVal = degrees * 100 * SERVO_MULTI;
    PWMVal /= 10000;
    PWMVal += SERVO_ZERO_OFFSET;

    if (PWMVal > 0xFF)
    {
        HighByte = true;
    }

    buf[0] = calcServo;
    buf[1] = int(PWMVal);
    uBit.i2c.write(chipAddress << 1, buf, 2);
    buf[0] = calcServo + 1;

    if (HighByte)
    {
        buf[1] = 0x01;
    }
    else
    {
        buf[1] = 0x00;
    }

    uBit.i2c.write(chipAddress << 1, buf, 2);
}

void KitronikRoboticsBoard::motorOn(int motor, Direction direction, int speed) {
    uint8_t buf[2];
    int motorReg = MOTOR_REG_BASE + (2 * (motor - 1) * REG_OFFSET);
    bool HighByte = false;
    int OutputVal = speed * 40;
    int HighOutputVal = 0;

    if (direction == FORWARD)
    {
        if (OutputVal > 0xFF)
        {
            HighByte = true;
            HighOutputVal = (OutputVal / 256);
            uBit.serial.printf("High output val: ");
            uBit.serial.printf(ManagedString((int)HighOutputVal).toCharArray());
            uBit.serial.printf("\r\n");
        }

        buf[0] = motorReg;
        buf[1] = int(OutputVal);
        uBit.i2c.write(chipAddress << 1, buf, 2);
        buf[0] = motorReg + 1;

        if (HighByte)
        {
            buf[1] = int(HighOutputVal);
            uBit.serial.printf("Output val: ");
            uBit.serial.printf(ManagedString((int)buf[1]).toCharArray());
            uBit.serial.printf("\r\n");
        }
        else
        {
            buf[1] = 0x00;
        }

        uBit.i2c.write(chipAddress << 1, buf, 2);

        for (int offset = 4; offset < 6; offset++) {
            buf[0] = motorReg + offset;
            buf[1] = 0x00;
            uBit.i2c.write(chipAddress << 1, buf, 2);
        }
    }
    else if (direction == REVERSE)
    {
            if (OutputVal > 0xFF)
            {
                HighByte = true;
                HighOutputVal = (OutputVal / 256);
            }

            buf[0] = motorReg + 4;
            buf[1] = int(OutputVal);
            uBit.i2c.write(chipAddress << 1, buf, 2);
            buf[0] = motorReg + 5;

            if (HighByte)
            {
                buf[1] = int(HighOutputVal);
            }
            else
            {
                buf[1] = 0x00;
            }

            uBit.i2c.write(chipAddress << 1, buf, 2);

            for (int offset2 = 0; offset2 < 2; offset2++) {
                buf[0] = motorReg + offset2;
                buf[1] = 0x00;
                uBit.i2c.write(chipAddress << 1, buf, 2);
            }
        }
    }

void KitronikRoboticsBoard::motorOff(int motor) {
    uint8_t buf[2];
    int motorReg = MOTOR_REG_BASE + (2 * (motor - 1) * REG_OFFSET);

    for (int offset3 = 0; offset3 < 2; offset3++) {
        buf[0] = motorReg + offset3;
        buf[1] = 0x00;
        uBit.i2c.write(chipAddress << 1, buf, 2);
    }

    for (int offset4 = 4; offset4 < 6; offset4++) {
        buf[0] = motorReg + offset4;
        buf[1] = 0x00;
        uBit.i2c.write(chipAddress << 1, buf, 2);
    }
}

void KitronikRoboticsBoard::allOff() {
    uint8_t buf[2];
    int servoOffCount = 0;
    int servoRegCount = 0;

    for (int motors = 1; motors < 5; motors++) {
        motorOff(motors);
    }

    while (servoOffCount < 8)
    {
        for (int offset5 = 0; offset5 < 2; offset5++) {
            buf[0] = SRV_REG_BASE + servoRegCount + offset5;
            buf[1] = 0x00;
            uBit.i2c.write(chipAddress << 1, buf, 2);
        }
        servoRegCount += 4;
        servoOffCount += 1;

    }
}

void KitronikRoboticsBoard::stepperMotorTurnAngle(int stepper, Direction direction, int angle) {
    int angleToSteps = 0;

    if ( stepper == 1)
    {
        int angleToSteps = ((angle - 1) * stepper1Steps) / ( 360 - 1) + 1;
    }
    else 
    {
        int angleToSteps = ((angle - 1) * stepper2Steps) / ( 360 - 1) + 1;
    }

    turnStepperMotor(stepper, direction, angleToSteps);
}

void KitronikRoboticsBoard::stepperMotorTurnSteps(int stepper, Direction direction, int steps) {
    turnStepperMotor(stepper, direction, steps);
}

void KitronikRoboticsBoard::turnStepperMotor(int stepper, Direction direction, int steps) {
    int stepCounter = 0;

    if (stepInit == false)
    {
        stepStage = 1;
        stepInit = true;
    }

    while (stepCounter < steps)
    {
        if (stepper == 1 || stepper == 3)
        {
            if (stepper == 1)
            {
                currentMotor = 1;
            }
            else
            {
                currentMotor = 3;
            };
        }
        else
        {
            if (stepper == 1)
            {
                currentMotor = 2;
            }
            else
            {
                currentMotor = 4;
            }
        }

        Direction currentDirection;

        if (stepStage == 1 || stepStage ==4)
        {
            currentDirection = FORWARD;
        }

        else
        {
            currentDirection = REVERSE;
        }

        motorOn(currentMotor, currentDirection, 100);
        uBit.sleep(20);

        if (direction == FORWARD){
            if (stepStage == 4)
            {
                stepStage = 1;
            }
            else
            {
                stepStage++;
            }
        }
            
        else if (direction == REVERSE)
        {
            if (stepStage == 1)
            {
                stepStage = 4;
            }
            else
            {
                stepStage--;
            }
        }

        stepCounter++;
    }
}