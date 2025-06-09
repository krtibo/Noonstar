#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
	// Initialize serial communication at 9600 baud rate
	Serial.begin(9600);
	Serial.println("Hello, World!");
	lcd.init();
	lcd.backlight();
	lcd.clear();
}

int count = 0;
void loop() {
	delay(100);
	lcd.setCursor(0, 0);
	lcd.print("test");
	lcd.setCursor(0, 1);
}
