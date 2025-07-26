#include "Screen.h"
#include <LiquidCrystal_I2C.h>

void Screen::render() {
	if (initialRun) { renderWelcomeScreen(); return; }
	else { renderScene(); }
}

void Screen::renderWelcomeScreen() {
	lcd->clear();
	isPrezMode = true;
	sceneTitle = "Noonstar";
	sceneSubtitle = AUTO_VERSION;
	renderScene();
	initialRun = false;
	delay(3000);
	isPrezMode = false;
	lcd->clear();
}

void Screen::renderScene() {
	if (!isPrezMode) {
		lcd->setCursor(0, 0);
		lcd->print(topLeft + " ");
		lcd->print(topCenter + " ");
		lcd->print(topRight);
	}
	lcd->setCursor(0, 1);
	lcd->print(sceneTitle);
	lcd->setCursor(0, 2);
	lcd->print(sceneSubtitle);
	if (!isPrezMode) {
		lcd->setCursor(0, 3);
		if (bottomLeft == "") {
			lcd->print("Next ");
			lcd->write(0); // Up character
		} else {
			lcd->print(bottomLeft + " ");
		}
		if (bottomCenter == "") {
			lcd->print(" Prev ");
			lcd->write(1); // Down character
			lcd->print(" ");
		} else {
			lcd->print(bottomCenter + " ");
		}
		lcd->print(bottomRight);
	}
}

void Screen::resetTextContent() {
	topLeft = "";
	topCenter = "";
	topRight = "";
	bottomLeft = "";
	bottomCenter = "";
	bottomRight = "";
	sceneTitle = "";
	sceneSubtitle = "";
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

String Screen::makeLeftAlignedWhitespacePadding(const String& text) {
	int paddingSize = RENDERABLE_ACTION_TEXT_SIZE - text.length();
	if (paddingSize == 0) return text;
	String padding = "";
	for (int i = 0; i < paddingSize; i++) {
		padding += " ";
	}
	return text + padding;
}

String Screen::makeRightAlignedWhitespacePadding(const String& text) {
	int paddingSize = RENDERABLE_ACTION_TEXT_SIZE - text.length();
	if (paddingSize == 0) return text;
	String padding = "";
	for (int i = 0; i < paddingSize; i++) {
		padding += " ";
	}
	return padding + text;
}
