#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Screen/Screen.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
Screen* screen;

void setup() {
	Serial.begin(9600);
	Serial.println("Hello, World!");
	screen = new Screen(lcd);
}

void loop() {
	delay(1000);
	screen->render();
}
