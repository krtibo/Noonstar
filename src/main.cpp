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
	screen->setTopLeft("D");
	screen->setTopCenter("E");
	screen->setTopRight("F");
	screen->setBottomRight("C");
}

void loop() {
	delay(1000);
	screen->render();
	// byte buttonStateA = buttonA.read();
	// if (buttonStateA == Button::PRESSED) {
	// 	Serial.println("button A pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(2, 3);
	// 	lcd.print("A");
	// }
	// if (buttonStateA == Button::NOT_PRESSED) {
	// 	Serial.println("button A NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateA == Button::LONG_PRESSED) {
	// 	Serial.println("button A LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(1, 3);
	// 	lcd.print("AAA");
	// }
	//
	//
	// byte buttonStateB = buttonB.read();
	// if (buttonStateB == Button::PRESSED) {
	// 	Serial.println("button B pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(10, 3);
	// 	lcd.print("B");
	// }
	// if (buttonStateB == Button::NOT_PRESSED) {
	// 	Serial.println("button B NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateB == Button::LONG_PRESSED) {
	// 	Serial.println("button B LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(9, 3);
	// 	lcd.print("BBB");
	// }
	//
	// byte buttonStateC = buttonC.read();
	// if (buttonStateC == Button::PRESSED) {
	// 	Serial.println("button C pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(18, 3);
	// 	lcd.print("C");
	// }
	// if (buttonStateC == Button::NOT_PRESSED) {
	// 	Serial.println("button C NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateC == Button::LONG_PRESSED) {
	// 	Serial.println("button C LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(17, 3);
	// 	lcd.print("CCC");
	// }
	//
	//
	// byte buttonStateD = buttonD.read();
	// if (buttonStateD == Button::PRESSED) {
	// 	Serial.println("button D pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(2, 0);
	// 	lcd.print("D");
	// }
	// if (buttonStateD == Button::NOT_PRESSED) {
	// 	Serial.println("button D NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateD == Button::LONG_PRESSED) {
	// 	Serial.println("button D LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(1, 0);
	// 	lcd.print("DDD");
	// }
	//
	//
	// byte buttonStateE = buttonE.read();
	// if (buttonStateE == Button::PRESSED) {
	// 	Serial.println("button E pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(10, 0);
	// 	lcd.print("E");
	// }
	// if (buttonStateE == Button::NOT_PRESSED) {
	// 	Serial.println("button E NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateE == Button::LONG_PRESSED) {
	// 	Serial.println("button E LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(9, 0);
	// 	lcd.print("EEE");
	// }
	//
	// byte buttonStateF = buttonF.read();
	// if (buttonStateF == Button::PRESSED) {
	// 	Serial.println("button F pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(18, 0);
	// 	lcd.print("F");
	// }
	// if (buttonStateF == Button::NOT_PRESSED) {
	// 	Serial.println("button C NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateF == Button::LONG_PRESSED) {
	// 	Serial.println("button F LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(17, 0);
	// 	lcd.print("FFF");
	// }
}
