#include <Arduino.h>

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "esp32-hal.h"

// board https://www.tindie.com/products/lspoplove/d-duino-32esp32-and-096oled-display/
// graphic arduino lib for oled: https://github.com/squix78/esp8266-oled-ssd1306
// oled 0x3c device is connected via i2c through pins: pin 4 (CLK) and pin 5 (DATA)

SSD1306 display(0x3c, 5, 4);
int8_t pixelYs[100];
int8_t previousY;

uint8_t analogPin = 25;
float value;
float deltaValue=0.03;

float midVolt = 2.9;
float deltaVolt = 0.3;

float voltage;
char charBuf[12];

int8_t chartHeight = 45;

uint8_t x_offset = 10;
uint8_t y_offset = 60;

static inline int8_t sgn(float val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}

void setup() {

	Serial.begin(115200);

	display.init();
	display.flipScreenVertically();
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.setFont(ArialMT_Plain_16);
}

void loop() {

	// SINUS
	deltaValue=0.03;
 	voltage = midVolt + sin(value) * deltaVolt;

	// UP/DOWN RAMP
/*
	deltaValue=0.1;
	if(sgn(sin(value))>0)
		voltage = 3.3;
	else
		voltage = 0;
*/

	int8_t pixelY = voltage/3.3 * chartHeight;
	int8_t analogValue = voltage/3.3 * 255;


	for(int8_t i=80; i>=1; i--)
	{
		pixelYs[i] = pixelYs[i-1];
	}
	pixelYs[0] = pixelY;

	dtostrf(voltage, 5, 2, charBuf);
	display.clear();

	for (int8_t i = 0; i <= 80; i++)
	{
		display.setPixel(x_offset + i, y_offset - pixelYs[i]);
		//display.drawLine(x_offset + i - 1, y_offset - previousY, x_offset + i, y_offset - pixelYs[i]);
	}
	previousY = pixelYs[80];
	display.drawString(0, 0, charBuf);

	display.display();

	dacWrite(analogPin, analogValue);


	value = value + deltaValue;
}
