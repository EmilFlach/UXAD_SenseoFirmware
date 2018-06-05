int encoderValue = 0;
int pixelStepSize = 2;
int pixelPower = 255;
bool pixelGoingUp = false;

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

int mapEncoderToPixels(int val) {
  int mappedValue = map(val, 0, ENCODERSTEPS, 0, NUMPIXELS);
  if (mappedValue != 0) {
    ++mappedValue;
  }
  return constrain(mappedValue, 0, NUMPIXELS);
}

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
