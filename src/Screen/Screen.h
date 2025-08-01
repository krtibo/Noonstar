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
			lcd.createChar(1, upCharacter);
			lcd.createChar(2, downCharacter);
			lcd.createChar(3, noteCharacter);
			initialRun = true;
			isPrezMode = false;
		}
		void render();
		void setTopLeft(const String& text) { topLeft = makeLeftAlignedWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setTopCenter(const String& text) { topCenter = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setTopRight(const String& text) { topRight = makeRightAlignedWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setBottomLeft(const String& text) { bottomLeft = makeLeftAlignedWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setBottomCenter(const String& text) { bottomCenter = makeWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setBottomRight(const String& text) { bottomRight = makeRightAlignedWhitespacePadding(text.substring(0, RENDERABLE_ACTION_TEXT_SIZE)); }
		void setSceneTitle(const String& text) { sceneTitle = text.substring(0, RENDERABLE_TITLE_SIZE); }
		void setSceneSubtitle(const String& text) { sceneSubtitle = text.substring(0, RENDERABLE_TITLE_SIZE); }
		void setPrezMode(const bool& mode) { isPrezMode = mode; }
		void resetTextContent();
		void clear() { lcd->clear(); }

	private:
		byte downCharacter[CUSTOM_CHAR_SIZE] = { B00000, B00000, B00000, B11111, B01110, B00100, B00000, B00000 };
		byte upCharacter[CUSTOM_CHAR_SIZE] = { B00000, B00000, B00000, B00100, B01110, B11111, B00000, B00000 };
		byte noteCharacter[CUSTOM_CHAR_SIZE] = { B00011, B00011, B00010, B00010, B00010, B01110, B11110, B01100 };
		LiquidCrystal_I2C* lcd;
		String topLeft, topCenter, topRight, bottomLeft, bottomCenter, bottomRight;
		String sceneTitle, sceneSubtitle;
		bool initialRun;
		bool isPrezMode;

		String makeWhitespacePadding(const String& text);
		String makeLeftAlignedWhitespacePadding(const String& text);
		String makeRightAlignedWhitespacePadding(const String& text);
		void renderWelcomeScreen();
		void renderScene();
};

#endif
