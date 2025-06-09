#include "Button.h"
#include <Arduino.h>

byte Button::read() {
	// if not busy AND state is same as last time
	if ((bitRead(status, 0) == false) && (digitalRead(pin) == last)) {
		if (isLongPressed()) return 3;
		return 255;																												// do nothing -- the button is still pressed
	}

	// If NEW Bit set - Key just pressed, record time
	if (bitRead(status, 1) == true) {																		// if it's a new value
		bitSet(status, 0);																								// set busy
		bitClear(status, 1);																							// set 'is is recently pressed? bit' to false
		time = millis();
		return 255;
	}

	if (isDebounceOver() == false) return 255;

	if (digitalRead(pin) == last) {
		bitClear(status, 0);
		bitSet(status, 1);
		return 255;
	} else {
		bitClear(status, 0);
		bitSet(status, 1);
		last = ((~last) & 0b00000001); // invert last
    return last;
	}
}

bool Button::isDebounceOver() {
	return millis() - time > debounce; 
}

bool Button::isLongPressed() {
	return millis() - time > LONG_PRESS_TIME && last == 0;
}
