// Button state variables
bool encoderButtonPressed = false;
bool nextButtonPressed = false;
bool calcButtonPressed = false;

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
