#ifndef Screen_h
#define Screen_h
#include <LiquidCrystal_I2C.h>

#include <Arduino.h>

class Screen {
	public:
		Screen(LiquidCrystal_I2C& lcd) {
			this->lcd = &lcd;
			lcd.init();
			lcd.backlight();
			lcd.clear();
			lcd.createChar(0, upCharacter);
			lcd.createChar(1, downCharacter);
		}
		void render();
	private:
		static const int CHAR_SIZE = 8;
		byte downCharacter[CHAR_SIZE] = { B00000, B00000, B11111, B11111, B01110, B00100, B00000, B00000 };
		byte upCharacter[CHAR_SIZE] = { B00000, B00000, B00100, B01110, B11111, B11111, B00000, B00000 };
		LiquidCrystal_I2C* lcd;
};

#endif
