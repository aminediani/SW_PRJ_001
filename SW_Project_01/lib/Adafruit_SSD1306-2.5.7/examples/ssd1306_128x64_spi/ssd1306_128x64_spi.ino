/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using SPI to communicate
 4 or 5 pins are required to interface.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display_OLED(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display_OLED.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display_OLED.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display_OLED.clearDisplay();

  // Draw a single pixel in white
  display_OLED.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display_OLED.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  testdrawline();      // Draw many lines

  testdrawrect();      // Draw rectangles (outlines)

  testfillrect();      // Draw rectangles (filled)

  testdrawcircle();    // Draw circles (outlines)

  testfillcircle();    // Draw circles (filled)

  testdrawroundrect(); // Draw rounded rectangles (outlines)

  testfillroundrect(); // Draw rounded rectangles (filled)

  testdrawtriangle();  // Draw triangles (outlines)

  testfilltriangle();  // Draw triangles (filled)

  testdrawchar();      // Draw characters of the default font

  testdrawstyles();    // Draw 'stylized' characters

  testscrolltext();    // Draw scrolling text

  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display_OLED.invertDisplay(true);
  delay(1000);
  display_OLED.invertDisplay(false);
  delay(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void loop() {
}

void testdrawline() {
  int16_t i;

  display_OLED.clearDisplay(); // Clear display buffer

  for(i=0; i<display_OLED.width(); i+=4) {
    display_OLED.drawLine(0, 0, i, display_OLED.height()-1, SSD1306_WHITE);
    display_OLED.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display_OLED.height(); i+=4) {
    display_OLED.drawLine(0, 0, display_OLED.width()-1, i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  delay(250);

  display_OLED.clearDisplay();

  for(i=0; i<display_OLED.width(); i+=4) {
    display_OLED.drawLine(0, display_OLED.height()-1, i, 0, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  for(i=display_OLED.height()-1; i>=0; i-=4) {
    display_OLED.drawLine(0, display_OLED.height()-1, display_OLED.width()-1, i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  delay(250);

  display_OLED.clearDisplay();

  for(i=display_OLED.width()-1; i>=0; i-=4) {
    display_OLED.drawLine(display_OLED.width()-1, display_OLED.height()-1, i, 0, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  for(i=display_OLED.height()-1; i>=0; i-=4) {
    display_OLED.drawLine(display_OLED.width()-1, display_OLED.height()-1, 0, i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  delay(250);

  display_OLED.clearDisplay();

  for(i=0; i<display_OLED.height(); i+=4) {
    display_OLED.drawLine(display_OLED.width()-1, 0, 0, i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }
  for(i=0; i<display_OLED.width(); i+=4) {
    display_OLED.drawLine(display_OLED.width()-1, 0, i, display_OLED.height()-1, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<display_OLED.height()/2; i+=2) {
    display_OLED.drawRect(i, i, display_OLED.width()-2*i, display_OLED.height()-2*i, SSD1306_WHITE);
    display_OLED.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<display_OLED.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display_OLED.fillRect(i, i, display_OLED.width()-i*2, display_OLED.height()-i*2, SSD1306_INVERSE);
    display_OLED.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<max(display_OLED.width(),display_OLED.height())/2; i+=2) {
    display_OLED.drawCircle(display_OLED.width()/2, display_OLED.height()/2, i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display_OLED.clearDisplay();

  for(int16_t i=max(display_OLED.width(),display_OLED.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display_OLED.fillCircle(display_OLED.width() / 2, display_OLED.height() / 2, i, SSD1306_INVERSE);
    display_OLED.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<display_OLED.height()/2-2; i+=2) {
    display_OLED.drawRoundRect(i, i, display_OLED.width()-2*i, display_OLED.height()-2*i,
      display_OLED.height()/4, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<display_OLED.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display_OLED.fillRoundRect(i, i, display_OLED.width()-2*i, display_OLED.height()-2*i,
      display_OLED.height()/4, SSD1306_INVERSE);
    display_OLED.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display_OLED.clearDisplay();

  for(int16_t i=0; i<max(display_OLED.width(),display_OLED.height())/2; i+=5) {
    display_OLED.drawTriangle(
      display_OLED.width()/2  , display_OLED.height()/2-i,
      display_OLED.width()/2-i, display_OLED.height()/2+i,
      display_OLED.width()/2+i, display_OLED.height()/2+i, SSD1306_WHITE);
    display_OLED.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display_OLED.clearDisplay();

  for(int16_t i=max(display_OLED.width(),display_OLED.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display_OLED.fillTriangle(
      display_OLED.width()/2  , display_OLED.height()/2-i,
      display_OLED.width()/2-i, display_OLED.height()/2+i,
      display_OLED.width()/2+i, display_OLED.height()/2+i, SSD1306_INVERSE);
    display_OLED.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display_OLED.clearDisplay();

  display_OLED.setTextSize(1);      // Normal 1:1 pixel scale
  display_OLED.setTextColor(SSD1306_WHITE); // Draw white text
  display_OLED.setCursor(0, 0);     // Start at top-left corner
  display_OLED.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display_OLED.write(' ');
    else          display_OLED.write(i);
  }

  display_OLED.display();
  delay(2000);
}

void testdrawstyles(void) {
  display_OLED.clearDisplay();

  display_OLED.setTextSize(1);             // Normal 1:1 pixel scale
  display_OLED.setTextColor(SSD1306_WHITE);        // Draw white text
  display_OLED.setCursor(0,0);             // Start at top-left corner
  display_OLED.println(F("Hello, world!"));

  display_OLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display_OLED.println(3.141592);

  display_OLED.setTextSize(2);             // Draw 2X-scale text
  display_OLED.setTextColor(SSD1306_WHITE);
  display_OLED.print(F("0x")); display_OLED.println(0xDEADBEEF, HEX);

  display_OLED.display();
  delay(2000);
}

void testscrolltext(void) {
  display_OLED.clearDisplay();

  display_OLED.setTextSize(2); // Draw 2X-scale text
  display_OLED.setTextColor(SSD1306_WHITE);
  display_OLED.setCursor(10, 0);
  display_OLED.println(F("scroll"));
  display_OLED.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display_OLED.startscrollright(0x00, 0x0F);
  delay(2000);
  display_OLED.stopscroll();
  delay(1000);
  display_OLED.startscrollleft(0x00, 0x0F);
  delay(2000);
  display_OLED.stopscroll();
  delay(1000);
  display_OLED.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display_OLED.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display_OLED.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display_OLED.clearDisplay();

  display_OLED.drawBitmap(
    (display_OLED.width()  - LOGO_WIDTH ) / 2,
    (display_OLED.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display_OLED.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display_OLED.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display_OLED.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display_OLED.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display_OLED.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display_OLED.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display_OLED.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}
