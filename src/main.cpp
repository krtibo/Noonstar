#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Screen/Screen.h"
#include "Button/Button.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
Screen* screen;
Button buttonA(14, 5);
Button buttonB(27, 5);
Button buttonC(26, 5);
Button buttonD(25, 5);
Button buttonE(33, 5);
Button buttonF(32, 5);

void setup() {
	Serial.begin(9600);
	screen = new Screen(lcd);
}

void loop() {
	delay(32);
	byte buttonStateA = buttonA.read();
	if (buttonStateA == 0) Serial.println("button A pressed");
	if (buttonStateA == 1) Serial.println("button A NOT pressed");
	if (buttonStateA == 3) Serial.println("button A LONG pressed");
	byte buttonStateB = buttonB.read();
	if (buttonStateB == 0) Serial.println("button B pressed");
	if (buttonStateB == 1) Serial.println("button B NOT pressed");
	if (buttonStateB == 3) Serial.println("button B LONG pressed");
	byte buttonStateC = buttonC.read();
	if (buttonStateC == 0) Serial.println("button C pressed");
	if (buttonStateC == 1) Serial.println("button C NOT pressed");
	if (buttonStateC == 3) Serial.println("button C LONG pressed");
	byte buttonStateD = buttonD.read();
	if (buttonStateD == 0) Serial.println("button D pressed");
	if (buttonStateD == 1) Serial.println("button D NOT pressed");
	if (buttonStateD == 3) Serial.println("button D LONG pressed");
	byte buttonStateE = buttonE.read();
	if (buttonStateE == 0) Serial.println("button E pressed");
	if (buttonStateE == 1) Serial.println("button E NOT pressed");
	if (buttonStateE == 3) Serial.println("button E LONG pressed");
	byte buttonStateF = buttonF.read();
	if (buttonStateF == 0) Serial.println("button F pressed");
	if (buttonStateF == 1) Serial.println("button F NOT pressed");
	if (buttonStateF == 3) Serial.println("button F LONG pressed");
	// screen->render();
}
