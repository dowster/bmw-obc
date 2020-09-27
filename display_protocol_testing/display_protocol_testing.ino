#define DATA_PIN 3  // GREEN WIRE
#define CLOCK_PIN 4 //  BLUE WIRE

void setup() {
  // put your setup code here, to run once:
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, HIGH);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendStartMessage();
  for(int i = 0; i < 24; i++) {
    sendNibble(0x02, i == 23);
  }
  delay(200);
}

void sendStartMessage() {
  digitalWrite(DATA_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 1 START
  delayMicroseconds(15);
  digitalWrite(DATA_PIN, LOW);
  delayMicroseconds(15);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 1 END
  delayMicroseconds(15);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 2 START
  delayMicroseconds(15);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 2 END
  delayMicroseconds(15);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 3 START
  delayMicroseconds(15);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 3 END
  delayMicroseconds(100);
}

void sendNibble(byte data, bool lastNibble) {
  for(byte i = 0; i < 4; i++) {
    digitalWrite(DATA_PIN, HIGH);
    delayMicroseconds(15);
    if(!(data & (1 << i))) { // Are we sending a low bit? If so we set DATA low here
      digitalWrite(DATA_PIN, LOW);
    }
    delayMicroseconds(15);
    digitalWrite(CLOCK_PIN, HIGH); // Clock in that sweet bit
    
    delayMicroseconds(15);
    // The last bit of each nibble seems to have a delay in between clock rising edge and data falling edge
    // Pretty sure the original micro is loading the next nibble during this delay or checking if it's the last nibble.
    if(i == 3) {
      delayMicroseconds(85); 
    }

    // On the last nibble of the 24 nibble message it seems like the data line has to leave high.
    // This will make sure that the data line is left high on the last bit of the last nibble
    digitalWrite(DATA_PIN, i == 3 && lastNibble);
    
    delayMicroseconds(15);
    digitalWrite(CLOCK_PIN, LOW);
  }
}

void sendLastNibble(byte data) {
  
}
