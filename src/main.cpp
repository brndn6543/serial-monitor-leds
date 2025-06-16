#include <Arduino.h>
#include <map>

int redLED = 11;
int greenLED = 9;
int yellowLED = 3;
int baudRate = 9600;

String inputString = "";
String promptColor = "[+] Please provide a color (red, green, yellow), or off: ";
String promptBrightness = "[+] Please input your desired brightness (1-10): ";
char receivedChar;

std::map<String, int> ledMap = {
	{"red", redLED},
	{"green", greenLED},
	{"yellow", yellowLED}
};

void handleInput(String& inputString) {
	inputString.toLowerCase();

	if (ledMap.count(inputString)) {
		int pin = ledMap[inputString];
		Serial.print(promptBrightness);
		String brightnessInput = "";

		while (true) {
			if (Serial.available()) {
				receivedChar = Serial.read();

				if (receivedChar == '\n' || receivedChar == '\r') {
					if (brightnessInput.length() > 0) break;
				}
				else if (receivedChar >= '0' && receivedChar <= '9') {
					brightnessInput += receivedChar;
					Serial.print(receivedChar);
				}
			}
		}
		Serial.println();

		int brightness = brightnessInput.toInt();
		brightness = constrain(brightness, 1, 10);
		int pwmValue = map(brightness, 1, 10, 26, 255); // Map brightness from 1-10 to PWM (approx 10%-100%)
		analogWrite(pin, pwmValue);

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
	Serial.print(promptColor);
	inputString = "";

	while (true) {
		if (Serial.available()) {
			receivedChar = Serial.read();

			if (receivedChar == '\n' || receivedChar == '\r') {
				if (inputString.length() > 0) break;
			}
			else if (receivedChar == 8 || receivedChar == 127) {
				if (inputString.length() > 0) {
					inputString.remove(inputString.length() - 1);
					Serial.print("\b \b");
				}
			}
			else if (receivedChar >= 32 && receivedChar < 126) {
				inputString += receivedChar;
				Serial.print(receivedChar);
			}
		}
	}
	Serial.println();
	handleInput(inputString);
}