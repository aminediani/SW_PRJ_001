#include "display7seg_manager.h"

// int CLK_7seg, DIO_7seg;
// //int8_t data[] = { 0xff, 0xff, 0xff, 0xff };
// TM1637Display display(CLK_7seg, DIO_7seg);

void showValue7seg(TM1637Display display, float num)
{
    
    display.showNumberDecEx((int)(num*100), (0x80 >> 1), true);

    //     //display.setSegments(data);
}

void clear7seg(TM1637Display display)
{
     //TM1637Display display(CLK_7seg, DIO_7seg);
    //uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
    
    // All segments off
    display.setSegments(blank);
}
void init7seg( TM1637Display display)
{

}
