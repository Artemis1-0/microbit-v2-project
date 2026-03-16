#include "MicroBit.h"
#include "samples/Tests.h"
#include "robotBoard.h"

MicroBit uBit;

void buttonA(MicroBitEvent e)
{
    uBit.io.speaker.setAnalogValue( 512 );
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
    KitronikRoboticsBoard theBoard;
    
    theBoard.motorOn(1, FORWARD, 50);
}

int main()
{
    uBit.init();
    buttonA(MicroBitEvent());
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, buttonA );
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, buttonB );

    release_fiber();
}

