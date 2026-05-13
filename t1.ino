int button[8] = { 13, 12, 14, 27, 15, 4, 33, 32 };

int led[8] = { 16, 17, 5, 18, 19, 21, 22, 23 };

int ledState[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
int buttonState[8];
int lastButtonState[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned long lastDebounceTime[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long debounceDelay = 50;

void setup() {
  for (int i = 0; i <= 7; i++) {
    pinMode(button[i], INPUT_PULLUP);
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i], ledState[i]);
  }
}

int reading[8];

void loop() {
  for (int i = 0; i <= 7; i++) {
    reading[i] = digitalRead(button[i]);

    if (reading[i] != lastButtonState[i]) {

      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (reading[i] != buttonState[i]) {
        buttonState[i] = reading[i];

        if (buttonState[i] == HIGH) {
          ledState[i] = !ledState[i];
        }
      }
    }

    digitalWrite(led[i], ledState[i]);

    lastButtonState[i] = reading[i];
  }
}
