#include "Screen.h"
#include <LiquidCrystal_I2C.h>

void Screen::render() {
	if (initialRun) { renderWelcomeScreen(); return; }
	else { renderScene(); }
}

void Screen::renderWelcomeScreen() {
	lcd->clear();
	lcd->setCursor(0, 1);
	lcd->print("Noonstar");
	lcd->setCursor(0, 2);
	lcd->print(AUTO_VERSION);
	initialRun = false;
	delay(3000);
	lcd->clear();
}

void Screen::renderScene() {
	lcd->setCursor(0, 0);
	lcd->print(topLeft + " ");
	lcd->print(topCenter + " ");
	lcd->print(topRight);
	lcd->setCursor(0, 1);
	lcd->print(sceneTitle);
	lcd->setCursor(0, 2);
	lcd->print(sceneSubtitle);
	lcd->setCursor(0, 3);
	lcd->print("Next ");
	lcd->write(0); // Up character
	lcd->print(" Prev ");
	lcd->write(1); // Down character
	lcd->print(" " + bottomRight);
}

String Screen::makeWhitespacePadding(const String& text) {
	int paddingSize = RENDERABLE_ACTION_TEXT_SIZE - text.length();
	if (paddingSize == 0) return text;
	String padding = "";
	for (int i = 0; i < paddingSize/2; i++) {
		padding += " ";
	}
	return padding + text + (paddingSize % 2 == 0 ? padding : padding + " ");
}
