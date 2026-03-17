#include "MicroBit.h"
#include "samples/Tests.h"
#include "robotBoard.h"

MicroBit uBit;
KitronikRoboticsBoard theBoard;

uint32_t lastMotorStart = 0;

void buttonA(MicroBitEvent e)
{
    theBoard.motorOff(1);
    for (int servo = 1; servo <= 8; servo++) {
        theBoard.servoWrite(servo, 0);
    }
    uBit.serial.printf("Button A Pressed\r\n");
    uBit.io.speaker.setAnalogValue( 256 );
    uBit.io.speaker.setAnalogPeriodUs( 1136 );
    uBit.sleep( 250 );
    uBit.io.speaker.setAnalogValue( 0 );
    uBit.sleep( 50 );
    uBit.io.speaker.setAnalogValue( 512 );
    uBit.io.speaker.setAnalogPeriodUs( 1136 );
    uBit.sleep( 250 );
    uBit.io.speaker.setAnalogValue( 0 );
    uBit.sleep( 50 );
    uBit.io.speaker.setAnalogValue( 512 );
    uBit.io.speaker.setAnalogPeriodUs( 1136 );
    uBit.sleep( 500 );
    uBit.io.speaker.setAnalogValue( 0 );
}

void buttonB(MicroBitEvent e)
{
    uBit.serial.printf("Button B Pressed\r\n");
    lastMotorStart = system_timer_current_time();
    uBit.serial.printf("Last motor start: ");
    uBit.serial.printf(ManagedString((int)lastMotorStart).toCharArray());
    uBit.serial.printf("\r\n");
    for (int servo = 1; servo <= 8; servo++) {
        theBoard.servoWrite(servo, 90);
    }
    theBoard.motorOn(1, FORWARD, 50);
    uBit.serial.printf("Motor 1 on forward at speed 50\r\n");
}

void onButtonPress(MicroBitEvent e) {
    uBit.serial.printf("P15 Pressed\r\n");
    if (system_timer_current_time() - lastMotorStart < 5000) return;
    uBit.serial.printf("Turning motor off\r\n");
    theBoard.motorOff(1);
    uBit.serial.printf("Motor 1 off\r\n");
}

void serialDisplay() {
    while(1) {
        ManagedString text = uBit.serial.readUntil("\n");
        uBit.serial.printf("Received: %s\r\n", text.toCharArray());
        uBit.display.scroll(text);
    }
}

void pinMonitor() {
    while(1) {
        uBit.serial.printf("P15: %d\r\n", uBit.io.P15.getDigitalValue());
        uBit.sleep(400);
    }
}

int main()
{
    uBit.init();
    uBit.serial.printf("Successfully initialized\r\n");
    theBoard = KitronikRoboticsBoard(); // reinitialise here
    uBit.serial.printf("Kitronik Robotics Board Initialized\r\n");
    theBoard.allOff();
    uBit.serial.printf("All motors and servos off\r\n");
    uBit.io.P15.setPull(PullMode::Up);
    uBit.io.P15.eventOn(MICROBIT_PIN_EVENT_ON_EDGE);
    uBit.messageBus.listen(MICROBIT_ID_IO_P15, MICROBIT_PIN_EVT_FALL, onButtonPress);
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, buttonA );
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, buttonB );

    create_fiber(serialDisplay);
    create_fiber(pinMonitor);

    release_fiber();
}