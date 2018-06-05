// Speed of blink variables
int ledStepSize = 2;
int ledBlinkStepSize = 10;

// Button state variables
bool encoderButtonPressed = false;
bool nextButtonPressed = false;
bool calcButtonPressed = false;
int ledPower = 255;
bool ledGoingUp = false;

// Only trigger the button once it is let go
bool checkEncoderButton() {
  if (digitalRead(encoderButton) == HIGH) {
    encoderButtonPressed = true;
  }
  if (digitalRead(encoderButton) == LOW && encoderButtonPressed) {
    encoderButtonPressed = false;
    return true;
  } else {
    return false;
  }
}

// Only trigger the button once it is let go
bool checkCalcButton() {
  if (digitalRead(calcButton) == HIGH) {
    calcButtonPressed = true;
  }
  if (digitalRead(calcButton) == LOW && calcButtonPressed) {
    calcButtonPressed = false;
    return true;
  } else {
    return false;
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
