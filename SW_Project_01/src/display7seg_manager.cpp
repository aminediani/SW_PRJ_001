#include "display7seg_manager.h"

void showValue7seg(TM1637Display display, float num)
{
    display.showNumberDecEx((int)(num*100), (0x80 >> 1), true);

}

void clear7seg(TM1637Display display)
{
    display.clear();
}

void init7seg( TM1637Display display)
{

}
