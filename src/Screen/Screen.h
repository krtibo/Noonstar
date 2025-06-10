#ifndef Screen_h
#define Screen_h
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

#define RENDERABLE_TITLE_SIZE 20
#define RENDERABLE_ACTION_TEXT_SIZE 6
#define CUSTOM_CHAR_SIZE 8

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
		void setTopLeft(const String& text) { topLeft = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setTopCenter(const String& text) { topCenter = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setTopRight(const String& text) { topRight = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setBottomRight(const String& text) { bottomRight = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setSceneTitle(const String& text) { sceneTitle = text.substring(0, RENDERABLE_TITLE_SIZE); }
		void setSceneSubtitle(const String& text) { sceneSubtitle = text.substring(0, RENDERABLE_TITLE_SIZE); }

	private:
		byte downCharacter[CUSTOM_CHAR_SIZE] = { B00000, B00000, B11111, B11111, B01110, B00100, B00000, B00000 };
		byte upCharacter[CUSTOM_CHAR_SIZE] = { B00000, B00000, B00100, B01110, B11111, B11111, B00000, B00000 };
		LiquidCrystal_I2C* lcd;
		String topLeft, topCenter, topRight, bottomRight;
		String sceneTitle, sceneSubtitle;
		String makeWhitespacePadding(const String& text);
};

#endif
