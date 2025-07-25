#ifndef Button_h
#define Button_h
#include <Arduino.h>

class Button {
	public:
		Button(byte pin) {
			pinMode(pin, INPUT_PULLUP);
			this->pin = pin;
		};
		enum Status {
			NOT_PRESSED = 1, // 0b00000001
			PRESSED = 0,     // 0b00000000
			LONG_PRESSED = 3, // 0b00000011
			NONE = 255
		};
		Status read();

	private:
		const int LONG_PRESS_TIME = 1500;
		byte pin;
		Status lastValue = Status::NOT_PRESSED;
		unsigned long lastValueRead;
		bool isPressed = false;
		bool longPressTriggered = false;
};

#endif
