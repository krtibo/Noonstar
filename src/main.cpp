// compile with Piorun neovim command
// to generate compile_commands.json: pio run --target compiledb
// should open project from noonstar directory with nvim command, otherwise clang misbehaves
// if clangd cries for a library, make sure to add its path to .clangd

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "Screen/Screen.h"
#include "Button/Button.h"
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <MIDI.h>
#include <ArduinoOTA.h>

#define DEBUG_MODE true
#define BUTTON_A_PIN 14
#define BUTTON_B_PIN 27
#define BUTTON_C_PIN 26
#define BUTTON_D_PIN 25
#define BUTTON_E_PIN 33
#define BUTTON_F_PIN 32
#define MIDI_CHANNEL 1
#define COMMA ,
#define CC_HIGH 127
#define CC_LOW 0
#define CC_LOOP_REC 60 COMMA CC_HIGH
#define CC_LOOP_OVERDUB 60 COMMA CC_LOW
#define CC_LOOP_PLAY 61 COMMA CC_HIGH
#define CC_LOOP_STOP 61 COMMA CC_LOW
#define CC_LOOP_UNDO 63 COMMA CC_HIGH
#define CC_TAP 64 COMMA CC_HIGH
#define CC_TUNER 68 COMMA CC_LOW
#define CC_RVB 4
#define CC_DLY 5

MIDI_CREATE_DEFAULT_INSTANCE();

LiquidCrystal_I2C lcd(0x27, 20, 4);
Screen* screen;

enum Buttons {
	A, B, C, D, E, F
};
Button buttons[6] = {
	Button(BUTTON_A_PIN),
	Button(BUTTON_B_PIN),
	Button(BUTTON_C_PIN),
	Button(BUTTON_D_PIN),
	Button(BUTTON_E_PIN),
	Button(BUTTON_F_PIN)
};
Button::Status buttonValues[6];

bool isAnyButtonPressed() {
	return
		buttonValues[Buttons::A] == Button::PRESSED ||
		buttonValues[Buttons::B] == Button::PRESSED ||
		buttonValues[Buttons::C] == Button::PRESSED ||
		buttonValues[Buttons::D] == Button::PRESSED ||
		buttonValues[Buttons::E] == Button::PRESSED ||
		buttonValues[Buttons::F] == Button::PRESSED;
}

const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

WebServer server(80);
File uploadFile;  // File object for the uploaded file

bool isTunerOn = false;
bool isReverbOn = false;
bool isDelayOn = false;
bool isLoopOn = false;
char sceneTitle[21];
int currentPreset = 0;
int totalPresets = 127;

int taps = 0;
unsigned long lastTap;
unsigned long tapsSum;
unsigned long currentTap;

void updateSceneTitle() {
	float tapsSumFloat = static_cast<float>(tapsSum) / taps;
	if (tapsSumFloat > 0) {
		std::sprintf(sceneTitle, "#%03d            t%3.0f", currentPreset, 60000 / tapsSumFloat);
	} else {
		std::sprintf(sceneTitle, "#%03d", currentPreset);
	}
}

// Serve the upload form page
void handleRoot() {
	const char* html =
	"<html lang=\"en\">\n"
	"<head>\n"
	"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
	"<style>\n"
	"  html, body {\n"
	"    height: 100%;\n"
	"    margin: 0;\n"
	"    display: flex;\n"
	"    align-items: center;\n"
	"    justify-content: center;\n"
	"    font-family: sans-serif;\n"
	"    padding: 1em;\n"
	"    box-sizing: border-box;\n"
	"  }\n"
	"  form {\n"
	"    display: flex;\n"
	"    flex-direction: column;\n"
	"    gap: 1em;\n"
	"    width: 100%;\n"
	"    max-width: 320px;\n"
	"  }\n"
	"  input[type=\"file\"], input[type=\"submit\"] {\n"
	"    font-size: 1em;\n"
	"  }\n"
	"</style>\n"
	"</head>\n"
	"<body>\n"
	"  <form method=\"POST\" action=\"/upload\" enctype=\"multipart/form-data\">\n"
	"    <h1 style=\"text-align:center; margin:0 0 1em;\">Upload Config File</h1>\n"
	"    <input type=\"file\" name=\"configfile\" required>\n"
	"    <input type=\"submit\" value=\"Upload\">\n"
	"  </form>\n"
	"</body>\n"
	"</html>\n";
	server.send(200, "text/html", html);
}

// Handle file upload
void handleFileUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Upload Start: %s\n", upload.filename.c_str());
    // Open file for writing (create or overwrite)
    if (SPIFFS.exists("/config.txt")) {
      SPIFFS.remove("/config.txt");  // Remove old file if exists
    }
    uploadFile = SPIFFS.open("/config.txt", FILE_WRITE);
    if (!uploadFile) {
      Serial.println("Failed to open file for writing");
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    // Write received bytes to file
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.printf("Upload End: %s, %u bytes\n", upload.filename.c_str(), upload.totalSize);
      server.send(200, "text/plain", "File Uploaded Successfully");
    } else {
      server.send(500, "text/plain", "Failed to save file");
    }
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    if (uploadFile) {
      uploadFile.close();
      SPIFFS.remove("/config.txt");  // Remove incomplete file
    }
    Serial.println("Upload Aborted");
    server.send(500, "text/plain", "Upload Aborted");
  }
}

void onOTAStart() {
	screen->clear();
	screen->setSceneTitle("OTA update started.");
	screen->setSceneSubtitle("");
	screen->render();
}

void otaMode() {
	if (buttonValues[Buttons::A] == Button::LONG_PRESSED && buttonValues[Buttons::C] == Button::LONG_PRESSED) {
		screen->clear();
		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssid, password);
		ArduinoOTA.setPassword("12345678");
		ArduinoOTA.begin();
		while(true) {
			screen->setSceneTitle("OTA mode...");
			screen->setSceneSubtitle("Connect to WiFi");
			screen->setPrezMode(true);
			screen->render();
			ArduinoOTA.handle();
			ArduinoOTA.onStart(onOTAStart);
			if (isAnyButtonPressed()) {
				screen->setPrezMode(false);
				screen->clear();
				WiFi.softAPdisconnect(true);
				ArduinoOTA.end();
				break;
			}
		}
	}
}

void handleTap() {
	currentTap = millis();
	if (lastTap > 0 && currentTap - lastTap > 5000) {
		taps = 0;
		tapsSum = 0;
		currentTap = 0;
		lastTap = 0;
	}
	if (lastTap > 0) {
		tapsSum += currentTap - lastTap;
		taps++;
	}
	lastTap = currentTap;
}

void otaDebug() {
	if (DEBUG_MODE) {
		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssid, password);
		ArduinoOTA.setPassword("12345678");
		ArduinoOTA.begin();
	}
}

void readButtonValues() {
	buttonValues[Buttons::A] = buttons[Buttons::A].read();
	buttonValues[Buttons::B] = buttons[Buttons::B].read();
	buttonValues[Buttons::C] = buttons[Buttons::C].read();
	buttonValues[Buttons::D] = buttons[Buttons::D].read();
	buttonValues[Buttons::E] = buttons[Buttons::E].read();
	buttonValues[Buttons::F] = buttons[Buttons::F].read();
}

void sendMIDIProgramChange(int programNumber) {
	MIDI.sendProgramChange(programNumber, MIDI_CHANNEL);
}

void sendMIDIControlChange(int controlNumber, int controlValue) {
	MIDI.sendControlChange(controlNumber, controlValue, MIDI_CHANNEL);
}

void loopMode() {
	if (!isLoopOn) return;
	screen->resetTextContent();
	screen->clear();
	unsigned long loopStartTime = 0;
	unsigned long loopLength = 0;
	char loopLengthStr[20] = "";
	while(true) {
		screen->setBottomLeft(loopStartTime > 0 ? "REC" : "rec");
		screen->setBottomCenter("play");
		screen->setBottomRight("stop");
		screen->setTopLeft("o.dub");
		screen->setTopCenter("undo");
		screen->setTopRight("LOOP");
		screen->setSceneTitle("Loop mode");

		if (loopStartTime > 0) {
			for (int i=0; i<20; i++) { loopLengthStr[i] = ' '; }
			loopLength = millis() - loopStartTime;
			float progressChars = ((float)loopLength/1000)/30*20;
			float limit = progressChars > 20 ? 20 : progressChars;
			for (int i=0; i<limit; i++) {
				loopLengthStr[i] = (char)255;
			}
		}

		screen->setSceneSubtitle(loopLengthStr);
		screen->render();
		readButtonValues();

		if (buttonValues[Buttons::A] == Button::PRESSED) {
			sendMIDIControlChange(CC_LOOP_REC);
			screen->clear();
			if (loopStartTime == 0) {
				loopStartTime = millis();
			}
		}
		if (buttonValues[Buttons::B] == Button::PRESSED) {
			sendMIDIControlChange(CC_LOOP_PLAY);
			if (loopStartTime > 0) {
				loopLength = millis() - loopStartTime;
				loopStartTime = 0;
				for (int i=0; i<20; i++) { loopLengthStr[i] = ' '; }
				std::sprintf(loopLengthStr, "* recorded %.1fs", (float)loopLength/1000);
				screen->clear();
			}
		}
		if (buttonValues[Buttons::C] == Button::PRESSED) {
			sendMIDIControlChange(CC_LOOP_STOP);
			if (loopStartTime > 0) {
				loopLength = millis() - loopStartTime;
				loopStartTime = 0;
				for (int i=0; i<20; i++) { loopLengthStr[i] = ' '; }
				std::sprintf(loopLengthStr, "* recorded %.1fs", (float)loopLength/1000);
				screen->clear();
			}
		}
		if (buttonValues[Buttons::D] == Button::PRESSED) {
			sendMIDIControlChange(CC_LOOP_OVERDUB);
		}
		if (buttonValues[Buttons::E] == Button::PRESSED) {
			sendMIDIControlChange(CC_LOOP_UNDO);
		}
		if (buttonValues[Buttons::F] == Button::PRESSED) {
			isLoopOn = false;
			screen->resetTextContent();
			screen->clear();
			break;
		}
	}
}

void setup() {
	Serial.begin(9600);
  MIDI.begin(MIDI_CHANNEL_OMNI);
	sendMIDIProgramChange(0);
	screen = new Screen(lcd);
	updateSceneTitle();
	otaDebug();
  // // Initialize SPIFFS
  // if (!SPIFFS.begin(true)) {
  //   Serial.println("An Error has occurred while mounting SPIFFS");
  //   return;
  // }
  //
  // // Start WiFi Access Point
  //
  // // Start mDNS responder for esp32.local
  // if (!MDNS.begin("esp32")) {
  //   Serial.println("Error setting up MDNS responder!");
  // } else {
  //   Serial.println("mDNS responder started");
  // }
  //
  // // Define server routes
  // server.on("/", HTTP_GET, handleRoot);
  // server.on("/upload", HTTP_POST, []() {
  //   // Empty handler for POST completion (required by WebServer)
  //   server.send(200);
  // }, handleFileUpload);
  //
  // server.begin();
  // Serial.println("HTTP server started");
}

void loop() {
	if (DEBUG_MODE) ArduinoOTA.handle();
	updateSceneTitle();
	screen->setTopLeft(isReverbOn ? "RVB   " : "rvb   ");
	screen->setTopCenter(isDelayOn ? "DLY" : "dly");
	screen->setTopRight(isLoopOn ? "  LOOP" : "  loop");
	screen->setBottomRight(isTunerOn ? " t/TUN" : " t/tun");
	screen->setSceneTitle(sceneTitle);
	screen->setSceneSubtitle("Mark V - Post-rock");
	screen->render();

	readButtonValues();

	if (buttonValues[Buttons::A] == Button::PRESSED) {
		currentPreset = (currentPreset + 1) % totalPresets;
		sendMIDIProgramChange(currentPreset);
		updateSceneTitle();
	}
	if (buttonValues[Buttons::B] == Button::PRESSED) {
		currentPreset = (currentPreset - 1 + totalPresets) % totalPresets;
		sendMIDIProgramChange(currentPreset);
		updateSceneTitle();
	}
	if (buttonValues[Buttons::C] == Button::PRESSED) {
		if (!isTunerOn) {
			handleTap();
			sendMIDIControlChange(CC_TAP);
		}
	}
	if (buttonValues[Buttons::C] == Button::LONG_PRESSED) {
		sendMIDIControlChange(CC_TUNER);
		isTunerOn = !isTunerOn;
	}
	if (buttonValues[Buttons::D] == Button::PRESSED) {
		sendMIDIControlChange(CC_RVB, isReverbOn ? CC_LOW : CC_HIGH);
		isReverbOn = !isReverbOn;
	}
	if (buttonValues[Buttons::E] == Button::PRESSED) {
		sendMIDIControlChange(CC_DLY, isDelayOn ? CC_LOW : CC_HIGH);
		isDelayOn = !isDelayOn;
	}
	if (buttonValues[Buttons::F] == Button::PRESSED) {
		isLoopOn = true;
	}

	otaMode();
	loopMode();

	// server.handleClient();
	// byte buttonStateA = buttonA.read();
	// if (buttonStateA == Button::PRESSED) {
	// 	Serial.println("button A pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(2, 3);
	// 	lcd.print("A");
	// }
	// if (buttonStateA == Button::NOT_PRESSED) {
	// 	Serial.println("button A NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateA == Button::LONG_PRESSED) {
	// 	Serial.println("button A LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(1, 3);
	// 	lcd.print("AAA");
	// }
	//
	//
	// byte buttonStateB = buttonB.read();
	// if (buttonStateB == Button::PRESSED) {
	// 	Serial.println("button B pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(10, 3);
	// 	lcd.print("B");
	// }
	// if (buttonStateB == Button::NOT_PRESSED) {
	// 	Serial.println("button B NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateB == Button::LONG_PRESSED) {
	// 	Serial.println("button B LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(9, 3);
	// 	lcd.print("BBB");
	// }
	//
	// byte buttonStateC = buttonC.read();
	// if (buttonStateC == Button::PRESSED) {
	// 	Serial.println("button C pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(18, 3);
	// 	lcd.print("C");
	// }
	// if (buttonStateC == Button::NOT_PRESSED) {
	// 	Serial.println("button C NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateC == Button::LONG_PRESSED) {
	// 	Serial.println("button C LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(17, 3);
	// 	lcd.print("CCC");
	// }
	//
	//
	// byte buttonStateD = buttonD.read();
	// if (buttonStateD == Button::PRESSED) {
	// 	Serial.println("button D pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(2, 0);
	// 	lcd.print("D");
	// }
	// if (buttonStateD == Button::NOT_PRESSED) {
	// 	Serial.println("button D NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateD == Button::LONG_PRESSED) {
	// 	Serial.println("button D LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(1, 0);
	// 	lcd.print("DDD");
	// }
	//
	//
	// byte buttonStateE = buttonE.read();
	// if (buttonStateE == Button::PRESSED) {
	// 	Serial.println("button E pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(10, 0);
	// 	lcd.print("E");
	// }
	// if (buttonStateE == Button::NOT_PRESSED) {
	// 	Serial.println("button E NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateE == Button::LONG_PRESSED) {
	// 	Serial.println("button E LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(9, 0);
	// 	lcd.print("EEE");
	// }
	//
	// byte buttonStateF = buttonF.read();
	// if (buttonStateF == Button::PRESSED) {
	// 	Serial.println("button F pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(18, 0);
	// 	lcd.print("F");
	// }
	// if (buttonStateF == Button::NOT_PRESSED) {
	// 	Serial.println("button C NOT pressed");
	// 	lcd.clear();
	// }
	// if (buttonStateF == Button::LONG_PRESSED) {
	// 	Serial.println("button F LONG pressed");
	// 	lcd.clear();
	// 	lcd.setCursor(17, 0);
	// 	lcd.print("FFF");
	// }
}
