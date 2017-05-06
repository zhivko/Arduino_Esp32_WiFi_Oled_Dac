#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "esp32-hal.h"
#include "StackArray.h"




// board https://www.tindie.com/products/lspoplove/d-duino-32esp32-and-096oled-display/
// graphic arduino lib for oled: https://github.com/squix78/esp8266-oled-ssd1306

//CL ... 6
//SO ... 7
//CM ... 11
//S1 ... 8

// oled 0x3c device is connected via i2c through pins: pin 4 (CLK) and pin 5 (DATA)

SSD1306 display(0x3c, 5, 4);
int8_t pixelYs[100];

uint8_t analogPin = 25;
uint8_t value;
int8_t sign=1;

float voltage;
char charBuf[12];

int8_t chartHeight = 45;

uint8_t x_offset = 10;
uint8_t y_offset = 60;

void setup() {

  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
}

void loop() {
  voltage = 3.3 * value/255.0;
  int8_t pixelY = chartHeight * value/255;
  for(int i=80; i>=1; i--)
  {
    pixelYs[i] = pixelYs[i-1]; 
  }
  pixelYs[0] = pixelY;


  dtostrf(voltage, 5, 2, charBuf);
  display.clear();

  
  for (int i = 0; i <= 80; i++)
  {
    display.setPixel(x_offset + i, y_offset - pixelYs[i]);
  }
  display.drawString(0, 0, charBuf);
  
  display.display();
  
  dacWrite(analogPin, value);
  if(value == 255)
  {
    sign = -1;
  }
  if(value == 0)
  {
    sign = 1;
  }
  
  value = value + sign;
}
