void senseoPress() {
  if (!senseoIsBeingPressed) {
    digitalWrite(senseoPin, HIGH);
    t.after(PRESSDELAY, senseoEndPress);
    senseoIsBeingPressed = true;
  }
}

void senseoEndPress() {
  digitalWrite(senseoPin, LOW);
  senseoIsBeingPressed = false;
}

void senseoEnable() {
  if (!senseoIsOn) {
    senseoPress();
    senseoIsOn = true;
  }
}

void senseoDisable() {
  if (senseoIsOn) {
    senseoPress();
    senseoIsOn = false;
  }
}

long getTimeFromLeds(int leds) {
  long value;
  switch (leds) {
    case 0:
      return 0;
      break;
    case 1:
      return 0;
      break;  
    case 2:
      value = (0.5 * MSPERCUP);
      break;
    case 3:
      value = (1 * MSPERCUP);
      break;
    case 4:
      value = (1.5 * MSPERCUP);
      break;
    default:
      value = ((leds - 2) * MSPERCUP);
      break;
  }
  return value;
}
