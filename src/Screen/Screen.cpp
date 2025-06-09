#include "Screen.h"
#include <LiquidCrystal_I2C.h>

void Screen::render() {
	lcd->setCursor(0, 0);
	lcd->print("Freeze   Tap    Loop");
	lcd->setCursor(0, 1);
	lcd->print("Mark III Solo");
	lcd->setCursor(0, 2);
	lcd->print("DLY + RVB");
	lcd->setCursor(0, 3);
	lcd->print("Next ");
	lcd->write(0); // Up character
	lcd->print("  Prev ");
	lcd->write(1); // Down character
	lcd->print("  Tune");
}
