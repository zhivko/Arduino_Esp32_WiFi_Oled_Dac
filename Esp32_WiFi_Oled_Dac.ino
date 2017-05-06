#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "esp32-hal.h"



// board https://www.tindie.com/products/lspoplove/d-duino-32esp32-and-096oled-display/
// graphic arduino lib for oled: https://github.com/squix78/esp8266-oled-ssd1306

//CL ... 6
//SO ... 7
//CM ... 11
//S1 ... 8

// oled 0x3c device is connected via i2c through pins: pin 4 (CLK) and pin 5 (DATA)

SSD1306 display(0x3c, 5, 4);

uint8_t analogPin = 25;
uint8_t value;
int8_t sign=1;

float voltage;
char charBuf[12];

void setup() {

  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);

}

void loop() {
  voltage = 3.3 * value/255.0;

  dtostrf(voltage, 5, 2, charBuf);
  display.clear();
  display.drawString(0, 10, charBuf);
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
