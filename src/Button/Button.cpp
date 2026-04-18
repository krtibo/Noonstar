#include "Button.h"
#include <Arduino.h>

Button::Status Button::read() {
    Status newValue = digitalRead(pin) ? Status::NOT_PRESSED : Status::PRESSED;
    unsigned long currentTime = millis();

    // On state change:
    if (newValue != lastValue) {
        lastValue = newValue;
        lastValueRead = currentTime;

			if (newValue == Status::PRESSED) {
				isPressed = true;
				longPressTriggered = false;  // Reset for new press
				return Status::NONE;  // Wait for hold or release
			} else {  // Released
				if (isPressed && !longPressTriggered) {
					isPressed = false;
					return Status::PRESSED;  // Short press confirmed
				}
				isPressed = false;
				return Status::NONE;
			}
		}
    // No state change; button still pressed?
    else if (isPressed && !longPressTriggered) {
        if ((currentTime - lastValueRead) >= LONG_PRESS_TIME) {
            longPressTriggered = true;
            return Status::LONG_PRESSED;
        }
    }

    return Status::NONE;
}
