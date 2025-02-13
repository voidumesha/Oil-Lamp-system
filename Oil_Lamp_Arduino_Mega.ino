#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);  // RX=10, TX=11

// Pin assignments and states for 15 toggles (LEDs)
const int ledPins[] = {24, 25, 26, 27, 28, 29, 30, 31, 32};
bool ledStates[9] = {false};  // Array to store the states of the LEDs, initialized to false (OFF)

const int numToggles = 9;  // Number of toggles

// Variables to manage the incoming data
String inputString = "";  // A string to hold the incoming message
bool stringComplete = false;  // A flag to indicate when a complete message is received

// Function to blink the LED with a pattern
void blinkPattern(int pin, int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);  // Turn ON the LED
    delay(delayMs);           // Wait
    digitalWrite(pin, LOW);   // Turn OFF the LED
    delay(delayMs);           // Wait
  }
}

void setup() {
  // Start Serial communication with the PC
  Serial.begin(9600);

  // Start SoftwareSerial communication
  mySerial.begin(9600);

  // Set all LED pins as output
  for (int i = 0; i < numToggles; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);  // Initialize LEDs to OFF
  }

  // Initialize the inputString
  inputString.reserve(100);  // Reserve memory to prevent fragmentation
}

void loop() {
  // Check if a complete message has been received
  if (stringComplete) {
    Serial.print("Received from ESP32: ");
    Serial.println(inputString);  // Print the full message

    // Loop through the toggles to handle them
    for (int i = 0; i < numToggles; i++) {
      String toggleOn = String(i + 1) + ":ON";
      String toggleOff = String(i + 1) + ":OFF";

      // Check for "ON" and "OFF" states for each toggle
      if (inputString.indexOf(toggleOn) != -1) {
        if (!ledStates[i]) {  // Only blink if the LED was previously OFF
          blinkPattern(ledPins[i], 3, 150);  // Blink 3 times with 150ms delay
        }
        digitalWrite(ledPins[i], HIGH);  // Turn on the LED
        ledStates[i] = true;  // Update state
      } else if (inputString.indexOf(toggleOff) != -1) {
        if (ledStates[i]) {  // Only blink if the LED was previously ON
          blinkPattern(ledPins[i], 3, 150);  // Blink 3 times with 150ms delay
        }
        digitalWrite(ledPins[i], LOW);  // Turn off the LED
        ledStates[i] = false;  // Update state
      }
    }

    // Clear the string and reset the flag for the next message
    inputString = "";
    stringComplete = false;
  }

  // Handle incoming serial data
  serialEvent();
}

void serialEvent() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    inputString += inChar;  // Append the new character to the input string

    // If a newline character is received, the message is complete
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
