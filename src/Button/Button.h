#ifndef Button_h
#define Button_h
#include <Arduino.h>

class Button {
	public:
		Button(byte pin, int debounce) {
			pinMode(pin, INPUT_PULLUP);
			this->pin = pin;
			this->debounce = debounce;
			this->last = 1;
			this->status = 0b00000010;
			this->time = 0;
		};
		enum Status {
			NOT_PRESSED = 1, // 0b00000001
			PRESSED = 0,     // 0b00000000
			LONG_PRESSED = 3, // 0b00000011
			NONE = 255
		};
		Status read();

	private:
		bool isDebounceOver();
		bool isLongPressed();

		const int LONG_PRESS_TIME = 1500;
		unsigned long time;
		int debounce;
		byte pin;
		byte status;
		byte last;
};

#endif
