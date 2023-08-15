// Snippet to be used with DSMP DIY MODULE
// Binding is handled by the module, so the button supposed to be pressed ON for the correct value of failsafe


// Pin for PPM output
const int ppmPin = 9;

// Pin for pushbutton input
const int buttonPin = 2;

// Frame length in microseconds (22ms)
const unsigned long frameLength = 22000;

void setup() {
  pinMode(ppmPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ppmPin, LOW); // Start with shut-off OFF
}

void loop() {
  // Read the pushbutton state
  int buttonState = digitalRead(buttonPin);

  // Determine the pulse width for the shut-off channel based on the button state
  unsigned int shutOffPulseWidth;
  if (buttonState == LOW) { // Button is pressed (ON)
    shutOffPulseWidth = 2000; // Shut-off ON (Max value)
  } else { // Button is not pressed (OFF)
    shutOffPulseWidth = 1000; // Shut-off OFF (Min value)
  }

  // Generate the PPM signal for the shut-off channel
  digitalWrite(ppmPin, HIGH);
  delayMicroseconds(shutOffPulseWidth);
  digitalWrite(ppmPin, LOW);

  // Generate the PPM signal for the blank channel (1500µs pulse width)
  digitalWrite(ppmPin, HIGH);
  delayMicroseconds(1500); // Blank channel pulse width
  digitalWrite(ppmPin, LOW);

  // Fill the rest of the frame with shut-off OFF
  delayMicroseconds(frameLength - shutOffPulseWidth - 1500);

  // Repeat to generate the continuous PPM signal
}
