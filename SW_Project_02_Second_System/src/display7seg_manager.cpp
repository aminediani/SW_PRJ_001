#include "display7seg_manager.h"

static TM1637Display display(CLK_7seg,DIO_7seg);

void showValue7seg( float num)
{
    display.showNumberDecEx((int)(num*100), (0x80 >> 1), true);
}

void clear7seg()
{
    display.clear();
}

void init_7seg(){
    display.setBrightness(0x0f);
}

