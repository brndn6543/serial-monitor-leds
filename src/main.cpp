#include <Arduino.h>
#include <map>
#include <sstream>

int redLED = 13;
int greenLED = 8;
int yellowLED = 4;
int baudRate = 9600;

String inputString = "";
String prompt = "[+] Please provide a color (red, green, yellow), or off: ";
char receivedChar;

std::map<String, int> ledMap = {
	{"red", redLED},
	{"green", greenLED},
	{"yellow", yellowLED}
};

void handleInput(String& inputString) {
	// Normalize input.
	inputString.toLowerCase();

	if (ledMap.count(inputString)) {
		int pin = ledMap[inputString];
		digitalWrite(pin, !digitalRead(pin)); // Toggle the LED state
	} else if (inputString == "off") {
		for (const auto& pair : ledMap) {
			digitalWrite(pair.second, LOW);
		}
	} else {
		Serial.println("[-] You entered an invalid value, try again.");
	}
}

void setup() {
	Serial.begin(baudRate);

	pinMode(redLED, OUTPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);

	while (!Serial) {};
}

void loop() {
	int ledArray[] = {redLED, greenLED, yellowLED};
	Serial.print(prompt);

	/// Empty buffer on loop start.
	inputString = "";

	while (true) {
		if (Serial.available()) {
			receivedChar = Serial.read();

			if (receivedChar == '\n' || receivedChar == '\r') {
				if (inputString.length() > 0) {
					break;
				} else {
					continue;
				}
			}

			// Handle backspace.
			else if (receivedChar == 8 || receivedChar == 127) {
				if (inputString.length() > 0) {
					inputString.remove(inputString.length() - 1);
					Serial.print("\b \b"); // Erases characters visually.
				}
			}
			// Regular printable characters.
			else if (receivedChar >= 32 && receivedChar < 126) {
				inputString += receivedChar;
				Serial.print(receivedChar);
			}
		}
	}
	Serial.println();
	handleInput(inputString);

}