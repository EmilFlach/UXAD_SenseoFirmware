#include <Timer.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

// Defining the different LED styles, used for setEncoderLed();
#define LED_OFF 0
#define LED_GLOW 1
#define LED_BLINK 2
#define LED_ON 3

// Hardware definitions, 10 LEDs and 96 steps on the encoder
#define NUMPIXELS 10
#define ENCODERSTEPS 96

// Defining all the timing that applies to the Senseo machine hardware
#define PRESSDELAY 500
#define MSPERCUP 67000
#define MSWARMUP 30000

// Defining the interface timeout and the point at which there is not enough water
#define MSTIMEOUT 15000
#define LED_CUTOFF 7

// Defining all pins
Timer t;
Encoder encoder(2, 3);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
int senseoPin = 4;
int encoderButton = 8;
int encoderLed = 9;
int calcButton = 10;
int calcLed = 11;

// State machine variables
byte state = 0;
byte prevState = 0;
long startMillis = 0;

// LED state variables
int ledsTurnedOn = 0;
int prevLedsTurnedOn = 0;

// Sleep timeout variables
long sleepMillis = 0;
bool isSleeping = false;

// Senseo state variables
bool senseoIsOn = false;
bool senseoIsBeingPressed = false;

void setup() {
  pixels.begin();
  pinMode(encoderLed, OUTPUT);
  pinMode(senseoPin, OUTPUT);
  pinMode(calcLed, OUTPUT);

  pinMode(encoderButton, INPUT);
  pinMode(calcButton, INPUT);
  setEncoderLed(LED_OFF);

  t.every(5, executeState); // Limiting amount of state machine executions, helps with lighting effects
  senseoPress(); // Used in boot sequence only, required to get the machine state and the arduino state in sync
}

void loop() {
  t.update();

  // Driving the pixels is left outside of the statemachine as it is a realtime input
  setPixels(mapEncoderToPixels(readEncoder()));
}

void executeState() {
  switch (state) {
    case 0: // Initial state, waiting for action or timeout
    
      // Things to do in this state
      updateLEDState();
      detectTimeout();
      setEncoderLed(LED_OFF);
      senseoDisable();

      // Detecting button presses, moving to other states
      if (checkEncoderButton()) {
        if (ledsTurnedOn <= 1) {
          setEncoderLed(LED_ON); //Should only cause a quick flash
        } else if (ledsTurnedOn <= LED_CUTOFF) {
          startMillis = millis(); //Save time at the start of brewing, used to time when the machine is turned off
          setState(1);
        } else {
          setState(2);
        }
      }
      if (checkCalcButton()) {
        setState(4);
      }
      break;
    case 1: // Machine heating, wait for heating to be done or instant stop
      setEncoderLed(LED_GLOW);
      senseoEnable();

      // Detecting changes, moving to other states
      if (millis() - startMillis > MSWARMUP) {
        setState(3);
      }
      if (checkEncoderButton()) {
        setState(0);
      }
      break;
    case 2: // Incorrect action state, machine blinks.
      setEncoderLed(LED_BLINK);

      // Go back to previous state by pressing the button again
      if (checkEncoderButton()) {
        setState(prevState);
      }
      break;
    case 3: // Machine brewing, wait for brewing to be done or instant stop 
      setEncoderLed(LED_ON);

      // Detecting changes, moving to other states
      if (millis() - startMillis > (MSWARMUP + getTimeFromLeds(ledsTurnedOn))) {
        setState(0);
      }
      if (checkEncoderButton()) {
        setState(0);
      }
      break;
    case 4: // Calc state, no actions possible until calc pressed again
      setEncoderLed(LED_OFF);
      digitalWrite(calcLed, HIGH);

      // Detecting changes, moving to other states
      if (checkCalcButton()) {
        digitalWrite(calcLed, LOW);
        setState(0);
      }
      if (checkEncoderButton()) {
        setState(2);
      }
      break;
  }
}

void setState(int newState) {
  if (!senseoIsBeingPressed) {
    prevState = state;
    state = newState;
  }
  if (state == 0 ) {
    sleepMillis = millis();
  }
  isSleeping = false;
}
