#include "Button.h"
#include <Arduino.h>

Button::Status Button::read() {
	// if not busy AND state is same as last time
	if ((bitRead(status, 0) == false) && (digitalRead(pin) == last)) {
		if (isLongPressed()) return LONG_PRESSED;
		return NONE;																												// do nothing -- the button is still pressed
	}

	// If NEW Bit set - Key just pressed, record time
	if (bitRead(status, 1) == true) {																		// if it's a new value
		bitSet(status, 0);																								// set busy
		bitClear(status, 1);																							// set 'is is recently pressed? bit' to false
		time = millis();
		return NONE;																											// do nothing -- the button is still pressed
	}

	if (isDebounceOver() == false) return NONE;

	if (digitalRead(pin) == last) {
		bitClear(status, 0);
		bitSet(status, 1);
		return NONE;
	} else {
		bitClear(status, 0);
		bitSet(status, 1);
		last = ((~last) & 0b00000001); // invert last
    return last == 0 ? PRESSED : NOT_PRESSED;
	}
}

bool Button::isDebounceOver() {
	return millis() - time > debounce; 
}

bool Button::isLongPressed() {
	return millis() - time > LONG_PRESS_TIME && last == 0;
}
