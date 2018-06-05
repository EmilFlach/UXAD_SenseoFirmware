// Encoder and pixel variables
int pixelStepSize = 2;
int pixelPower = 255;
bool pixelGoingUp = false;

// Encoder led variables
int ledPower = 255;
bool ledGoingUp = false;
int ledStepSize = 2;
int ledBlinkStepSize = 10;

// Saving the encoder state, for internal use
int encoderValue = 0;

void updateLEDState() {
  prevLedsTurnedOn = ledsTurnedOn;
  ledsTurnedOn = mapEncoderToPixels(readEncoder());
}

void detectTimeout() {
  if (prevLedsTurnedOn != ledsTurnedOn) {
    sleepMillis = millis();
  }
  if (millis() - sleepMillis > MSTIMEOUT) {
    isSleeping = true;
  } else {
    isSleeping = false;
  }
}

// Read the encoder and make it loop at 360 degree rotation
int readEncoder() {
  int tempValue = encoder.read();
  if (tempValue < 0) {
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
  return encoderValue;
}

// Convert encoder value to pixels that should be on
int mapEncoderToPixels(int val) {
  int mappedValue = map(val, 0, ENCODERSTEPS, 0, NUMPIXELS);
  if (mappedValue != 0) {
    ++mappedValue;
  }
  return constrain(mappedValue, 0, NUMPIXELS);
}

// Setting the pixels based on the mapped encoder value
void setPixels(int val) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (isSleeping) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    } else {
      if (i < val) {
        if (i < LED_CUTOFF) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 150));
        } else {
          pixels.setPixelColor(i, pixels.Color(0, 0, pixelPower));
        }
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
    }
    pixels.show();
  }

  if (pixelGoingUp) {
    pixelPower += pixelStepSize;
  } else {
    pixelPower -= pixelStepSize;
  }
  if (pixelPower > 150) {
    pixelGoingUp = false;
  }
  if (pixelPower < 2) {
    pixelGoingUp = true;
  }
}

// State machine to set the encoderLed to different blink types
void setEncoderLed(int type) {
  switch (type) {
    case LED_OFF:
      analogWrite(encoderLed, 255);
      break;
    case LED_GLOW:
      analogWrite(encoderLed, 255 - ledPower);
      if (ledGoingUp) {
        ledPower += ledStepSize;
      } else {
        ledPower -= ledStepSize;
      }
      if (ledPower > 220) {
        ledGoingUp = false;
      }
      if (ledPower < 20) {
        ledGoingUp = true;
      }
      break;
    case LED_BLINK:
      analogWrite(encoderLed, 255 - ledPower);
      if (ledGoingUp) {
        ledPower += ledBlinkStepSize;
      } else {
        ledPower -= ledBlinkStepSize;
      }
      if (ledPower > 220) {
        ledGoingUp = false;
      }
      if (ledPower < 20) {
        ledGoingUp = true;
      }
      break;
    case LED_ON:
      analogWrite(encoderLed, 0);
      break;
  }
}
