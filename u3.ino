int rotary[4] = { 13, 12, 14, 27 };

int led[8] = { 15, 16, 5, 18, 19, 21, 22, 23 };

void setup() {
  Serial.begin(115200);
  for (int i = 0; i <= 7; i++) {
    if (i < 4) {
      pinMode(rotary[i], INPUT_PULLUP);
    }
    pinMode(led[i], OUTPUT);
  }
}

int knob[4];
int key;
bool ledstate = HIGH;
int last = 0;
void loop() {
  for (int i = 0; i <= 3; i++) {
    knob[i] = !digitalRead(rotary[i]);
  }
  int dec = ((knob[3]) * 8) + ((knob[2]) * 4) + ((knob[1]) * 2) + (knob[0]);
  if (dec < 8) {
    for (int i = 0; i <= 8; i++) {
      if(i<=dec){
        digitalWrite(led[i], HIGH);
      }else{
        digitalWrite(led[i], LOW);
      }
      
    }
  } else {
    if (millis() - last >= 500) {
      for (int i = 0; i <= 7; i++) {
        digitalWrite(led[i], ledstate);
      }
      ledstate = !ledstate;
      last = millis();
    }
  }

  Serial.println(dec);
}
