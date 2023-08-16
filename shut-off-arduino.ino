#define ppmOutputPin 0 // PPM signal output pin
#define ppmwidth 300
#define num_channels 2 // Number of PPM channels
#define PPM_FrLen 20000 // PPM frame length in microseconds (1ms = 1000us)
#define buttonPin 2 // Pin for the pushbutton

int PPM_PulseLen = ppmwidth; // Pulse length
volatile int ppm[num_channels] = {1000, 1500}; // Initial channel values (channel 1 is off)

void setup() {
  pinMode(ppmOutputPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as input with pull-up resistor
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as output
  digitalWrite(ppmOutputPin, 0); // Set the PPM signal pin to the default state (off)
  digitalWrite(LED_BUILTIN, HIGH); // Turn on the built-in LED on power up

  // Timer setup code
  cli();
  TCCR1A = 0; 
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1500; // compare match register, change this for 8MHz
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS10); // No prescaler for 8MHz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();
}

void loop() {
  static bool buttonState = false;
  static int lastButtonState = HIGH;

  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    delay(50); // Debounce
    if (reading == LOW) {
      buttonState = !buttonState;
      ppm[0] = buttonState ? 2000 : 1000; // Channel 1 is 2000 when on, 1000 when off
    }
  }
  lastButtonState = reading;
}

ISR(TIMER1_COMPA_vect) { // PPM generation
  static int cur_chan_numb = 0;
  static int calc_rest = 0;
  digitalWrite(ppmOutputPin, 1);
  OCR1A = PPM_PulseLen; // Adjusted for 8MHz
  digitalWrite(ppmOutputPin, 0);
  if (cur_chan_numb == num_channels) {
    OCR1A = (PPM_FrLen - calc_rest); // Adjusted for 8MHz
    cur_chan_numb = 0;
    calc_rest = 0;
  } else {
    OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen); // Adjusted for 8MHz
    calc_rest = calc_rest + ppm[cur_chan_numb];
    cur_chan_numb++;
  }
}
