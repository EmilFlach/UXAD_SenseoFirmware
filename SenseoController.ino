// Enable senseo relais and disable after delay
void senseoPress() {
  if (!senseoIsBeingPressed) {
    digitalWrite(senseoPin, HIGH);
    t.after(PRESSDELAY, senseoEndPress);
    senseoIsBeingPressed = true;
  }
}

// Called by timer in senseoPress
void senseoEndPress() {
  digitalWrite(senseoPin, LOW);
  senseoIsBeingPressed = false;
}

// Sync presses with senseo state
void senseoEnable() {
  if (!senseoIsOn) {
    senseoPress();
    senseoIsOn = true;
  }
}

// Sync presses with senseo state
void senseoDisable() {
  if (senseoIsOn) {
    senseoPress();
    senseoIsOn = false;
  }
}

// Convert the amount of LEDs to the time the senseo should be on
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
