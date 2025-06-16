# Control LED state and brightness from the Serial Monitor
This code demonstrates how to control the on/off functionality of LEDs, as well
as the brightness.

## Code
I use CLion and the following code is used to ensure
CLion's serial monitor behaves neatly and as expected.
```c++
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
```
This ensures that prompts and answers are cleanly displayed and that backspace
functionality works as it should. It also takes care of any newline or return 
character issues.

The `handleInput()` function processes the user's inputs string to control LED states
and brightness. It converts the input string to lowercase to ensure case-insensitive comparison.

If the input matches one of the predefined colors, it prompts the user to specify a brightness level (1-10).
The brightness level entered by the user is then constrained to this valid range and mapped to a PWM value
suitable for `analogWrite`, effectively controlling the LED brightness.

if the input is `off`, it turns off all LEDs. Any invalid input triggers an error message prompting the user
to try again.

```c++
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
```
## Wiring
![](https://i.postimg.cc/TYT8Sxn5/rgn-brightness-toggle.png)