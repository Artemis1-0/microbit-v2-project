#include "MicroBit.h"
#include "samples/Tests.h"

MicroBit uBit;

void buttonA(MicroBitEvent e)
{
    uBit.io.speaker.setAnalogValue( 512 );
    uBit.io.speaker.setAnalogPeriod( 1000 );
    uBit.sleep( 1000 );
    uBit.io.speaker.setAnalogValue( 0 );
}

int main()
{
    uBit.init();
    uBit.messageBus.listen( MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, buttonA );
}

