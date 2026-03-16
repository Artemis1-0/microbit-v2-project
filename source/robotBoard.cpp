#include "MicroBit.h"
#include "robotBoard.h"

KitronikRoboticsBoard::KitronikRoboticsBoard() {
    uint8_t buf[2];
    buf[0] = PRESCALE_REG;
    buf[1] = 0x7D;
    uBit.i2c.write(chipAddress, buf, false);

    for (int blockReg = 0xFA; blockReg < 0xFE; blockReg++) {
    buf[0] = blockReg;
    buf[1] = 0x00;
    uBit.i2c.write(chipAddress, buf, false);
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
    uBit.i2c.write(chipAddress, buf, false);
    buf[0] = calcServo + 1;

    if (HighByte)
    {
        buf[1] = 0x01;
    }
    else
    {
        buf[1] = 0x00;
    }

    uBit.i2c.write(chipAddress, buf, false);
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
        }

        buf[0] = motorReg;
        buf[1] = int(OutputVal);
        uBit.i2c.write(chipAddress << 1, buf, 2);
        buf[0] = motorReg + 1;

        if (HighByte)
        {
            buf[1] = int(HighOutputVal);
        }
        else
        {
            buf[1] = 0x00;
        }

        uBit.i2c.write(chipAddress << 1, buf, 2);

        for (int offset = 4; offset < 6; offset++) {
            buf[0] = motorReg + offset;
            buf[1] = 0x00;
            uBit.i2c.write(chipAddress << 1, buf, false);
        }
    }
    else if (direction == REVERSE)
    {
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
                uBit.i2c.write(chipAddress << 1, buf, false);
            }
        }
    }
}

void KitronikRoboticsBoard::motorOff(int motor) {
    uint8_t buf[2];
    int motorReg = MOTOR_REG_BASE + (2 * (motor - 1) * REG_OFFSET);

    for (int offset3 = 0; offset3 < 6; offset3++) {
        buf[0] = motorReg + offset3;
        buf[1] = 0x00;
        uBit.i2c.write(chipAddress << 1, buf, false);
    }

    for (int offset4 = 4; offset4 < 6; offset4++) {
        buf[0] = motorReg + offset4;
        buf[1] = 0x00;
        uBit.i2c.write(chipAddress << 1, buf, false);
    }
}

void KitronikRoboticsBoard::allOff() {
    uint8_t buf[2];
    int servoOffCount = 0;
    int servoRegCount = 0;

    for (int motors = 1; motors <= 5; motors++) {
        motorOff(motors);
    }

    while (servoOffCount < 8)
    {
        for (int offset5 = 0; offset5 < 2; offset5++) {
            buf[0] = SRV_REG_BASE + servoRegCount + offset5;
            buf[1] = 0x00;
            uBit.i2c.write(chipAddress << 1, buf, false);
        }
        servoRegCount += 4;
        servoOffCount += 1;

    }
}