#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 23
#define ENCODERSTEPS 96

Encoder encoder(2, 3);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
int switchPin = 8;
int machineButtonPin = 4;
int ledPin = 9;

int cutOffValue = 15;
int encoderValue = 0;

bool ledOn = false;
int ledPower = 255;
bool ledGoingUp = false;

void setup() {
  pixels.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(machineButtonPin, OUTPUT);
}

void loop() {
  readEncoder();
  setPixels();
  checkAndSetLed();
  delay(30);
}

void buttonPassThrough() {
  if(digitalRead(switchPin)) {
    digitalWrite(machineButtonPin, HIGH);
    delay(300);
  } else {
    digitalWrite(machineButtonPin, LOW);
  }
}

void readEncoder() {
  int tempValue = encoder.read();
  if(tempValue < 0) {
    int val = ENCODERSTEPS + tempValue;
    encoder.write(val);
    encoderValue = val;
  } else if (tempValue > ENCODERSTEPS) {
    int val = tempValue - ENCODERSTEPS; 
    encoder.write(val);
    encoderValue = val;
  } else {
    encoderValue = tempValue;
  }
}

void setPixels() {
  int mappedValue = map(encoderValue, 0, ENCODERSTEPS, 0, NUMPIXELS);
  int val = constrain(mappedValue, 0, NUMPIXELS);
  for (int i = 0; i < NUMPIXELS; i++) {
    if (i < val) {
      if (i < cutOffValue) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 150));
      } else {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }
    } else {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
  }
}

void checkAndSetLed() {
  if(digitalRead(switchPin)) {
    ledOn = !ledOn;
    delay(300);
  }
  if(ledOn) {
    analogWrite(ledPin, 255 - ledPower);
    updateLedPower(8);
  } else {
    analogWrite(ledPin, 255);
  }
}

void updateLedPower(byte stepSize) {
  if (ledGoingUp) {
    ledPower += stepSize;
  } else {
    ledPower -= stepSize;
  }
  if (ledPower > 240) {
    ledGoingUp = false;
  }
  if (ledPower < 20) {
    ledGoingUp = true;
  }
}

