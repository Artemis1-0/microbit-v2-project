#include "MicroBit.h"
#include "samples/Tests.h"
#include "robotBoard.h"

MicroBit uBit;
KitronikRoboticsBoard theBoard;

void buttonA(MicroBitEvent e)
{
    theBoard.motorOff(1);
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
    theBoard.motorOn(1, FORWARD, 50);
}

void onButtonPress(MicroBitEvent e) {
    uBit.serial.printf("P15 Pressed\r\n");
    uBit.display.scroll("Hello World!");
    uBit.serial.printf("Button Pressed\r\n");
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

    while(1) {
    uBit.serial.printf("P15: %d\r\n", uBit.io.P15.getDigitalValue());
    uBit.sleep(100);
    ManagedString text = uBit.serial.readUntil("\n");
    uBit.display.scroll(text);
    }

    release_fiber();
}