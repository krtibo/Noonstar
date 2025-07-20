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

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

LiquidCrystal_I2C lcd(0x27, 20, 4);
Screen* screen;
Button buttonA(14, 5);
Button buttonB(27, 5);
Button buttonC(26, 5);
Button buttonD(25, 5);
Button buttonE(33, 5);
Button buttonF(32, 5);

const char* ssid = "ESP32-Access-Point";
const char* password = "12345678";

WebServer server(80);
File uploadFile;  // File object for the uploaded file

bool isAnyButtonPressed() {
	return
		buttonA.read() == Button::PRESSED ||
		buttonB.read() == Button::PRESSED ||
		buttonC.read() == Button::PRESSED ||
		buttonD.read() == Button::PRESSED ||
		buttonE.read() == Button::PRESSED ||
		buttonF.read() == Button::PRESSED;
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
	if (buttonA.read() == Button::LONG_PRESSED && buttonC.read() == Button::LONG_PRESSED) {
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

void setup() {
	Serial.begin(9600);          // Debug output on UART0
  // Serial2.begin(31250);          // MIDI baud rate on UART2
  // MIDI.begin(MIDI_CHANNEL_OMNI);
	screen = new Screen(lcd);

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
	screen->setTopLeft("RVB   ");
	screen->setTopCenter("DLY");
	screen->setTopRight("Freeze");
	screen->setBottomRight("Loop");
	screen->setSceneTitle("#02");
	screen->setSceneSubtitle("Mark V - Post-rock");
	screen->render();
	otaMode();
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
