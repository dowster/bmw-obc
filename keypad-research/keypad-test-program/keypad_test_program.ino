// test program to figure out which pins are connected when testing
// a 4x4 keypad matrix
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  for(int pin = 2; pin < 11; pin++) {
    pinMode(pin, INPUT_PULLUP);
    digitalWrite(pin, HIGH);
  }
}

// the loop routine runs over and over again forever:
void loop() {

  int potatos[] = { -1, -1, 0, 1, 2, 3, -1, 4, 5, 6, 7 };
  Serial.println("                 0, 1, 2, 3, 4, 5, 6, 7");
  
  for(int activePin = 2; activePin < 11; activePin++) {
    if(activePin != 6) {
      pinMode(activePin, OUTPUT);
      digitalWrite(activePin, LOW);
  
      Serial.print("Checking pin ");
      Serial.print(potatos[activePin]);
      Serial.print(" - ");
      
      for(int checkPin = 2; checkPin < 11; checkPin++) {
        if(checkPin != 6) {
          if(checkPin != activePin) {
            Serial.print(digitalRead(checkPin));
          } else {
            Serial.print("X");
          }
      
          if(checkPin != 10) {
            Serial.print(", ");
          } else {
            Serial.println(" ");
          }
        }
      }
  
      digitalWrite(activePin, HIGH);
      pinMode(activePin, INPUT_PULLUP);
    }
  }

  delay(1000);
}
