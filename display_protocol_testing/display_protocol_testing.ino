#include <Shell.h>

#define DATA_PIN 3  // GREEN WIRE
#define CLOCK_PIN 4 //  BLUE WIRE

#define TICK_DELAY 5

byte displayBuffer[12];

void setup() {
  // Wait after reset or power on...
  delay(1000);
  
  // put your setup code here, to run once:
  pinMode(DATA_PIN, OUTPUT);
  digitalWrite(DATA_PIN, HIGH);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);

  // Not sure if I need to clear the buffer contents but fuck it
  // better safe than sorry.
  for(int i = 0; i < 12; i++) {
    displayBuffer[i] = 0;
  }
  
  // Prepare serial communication
  Serial.begin(9600);

  shell_println("Starting display prototype");
  
  // Initialize command line interface (CLI)
  // We pass the function pointers to the read and write functions that we implement below
  // We can also pass a char pointer to display a custom start message
  shell_init(shell_reader, shell_writer, 0);
  
  // Add commands to the shell
  shell_register(set_int, PSTR("setint"));
  
  // Add commands to the shell
  shell_register(set_all_bits, PSTR("setall"));
  
  // Add commands to the shell
  shell_register(set_bits, PSTR("setb"));

  
  // Add commands to the shell
  shell_register(set_bit, PSTR("set"));
  
  // Add commands to the shell
  shell_register(clear_bit, PSTR("clr"));
  
  // Add commands to the shell
  shell_register(get_display_buffer, PSTR("get"));

}
int loopCount = 0;
void loop() {
  if(0 == (loopCount %= 40)) {
    // put your main code here, to run repeatedly:
    sendStartMessage();
    for(int i = 0; i < 12; i++) {
      sendByte(displayBuffer[i], i == 11);
    }
  }
  loopCount++;
  delay(5);
    
  // This should always be called to process user input
  shell_task();
}

/**
 * Send the start message to the display. 
 * This is different than shifting the data in because it sets data high, clock high, data low, clock low, 
 * and then sends two clock pulses while keeping the data line low. 
 */
void sendStartMessage() {
  digitalWrite(DATA_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 1 START
  delayMicroseconds(TICK_DELAY);
  digitalWrite(DATA_PIN, LOW);
  delayMicroseconds(TICK_DELAY);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 1 END
  delayMicroseconds(TICK_DELAY);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 2 START
  delayMicroseconds(TICK_DELAY);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 2 END
  delayMicroseconds(TICK_DELAY);
  digitalWrite(CLOCK_PIN, HIGH); // PULSE 3 START
  delayMicroseconds(TICK_DELAY);
  digitalWrite(CLOCK_PIN, LOW); // PULSE 3 END
  delayMicroseconds(100);
}

void sendByte(byte data, bool lastByte) {
  for(byte i = 0; i < 8; i++) {
    digitalWrite(DATA_PIN, HIGH);
    delayMicroseconds(TICK_DELAY);
    if(!(data >> i & 1)) { // Are we sending a low bit? If so we set DATA low here
      digitalWrite(DATA_PIN, LOW);
    }
    delayMicroseconds(TICK_DELAY);
    digitalWrite(CLOCK_PIN, HIGH); // Clock in that sweet bit
    
    delayMicroseconds(TICK_DELAY);
    // The last bit of each nibble seems to have a delay in between clock rising edge and data falling edge
    // Pretty sure the original micro is loading the next nibble during this delay or checking if it's the last nibble.
    if(i == 3 || i == 7) {
      //delayMicroseconds(85); 
    }

    // On the last nibble of the 24 nibble message it seems like the data line has to leave high.
    // This will make sure that the data line is left high on the last bit of the last nibble
    digitalWrite(DATA_PIN, i == 7 && lastByte);
    
    delayMicroseconds(TICK_DELAY);
    digitalWrite(CLOCK_PIN, LOW);
  }
}

void format_bits(byte val, char* charbuffer) {
  for(int i = 0; i < 8; i++) {
    charbuffer[i] = ((val >> (7 - i)) & 0x01) ? '1' : '0';
  }
}

/**
 * Print the display buffer to serial.
 */
int get_display_buffer(int argc, char** argv)
{
  
  shell_print("Display Buffer: ");
  for(int i = 0; i < 12; i++) {
    char binaryString[9] = {};
    format_bits(displayBuffer[i], binaryString);
    shell_printf("%s ", binaryString);
  }
  shell_println("");
  
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to change the bytes in the display buffer. 
 */
int set_int(int argc, char** argv)
{
  shell_println("Running \"set\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s, arg3=%s\n\r", argc, argv[0], argv[1], argv[2]);
  if(argc != 3) {
    shell_println("Error: two arguments \"position\" and \"value\" were not provided.");
    shell_println("       Command Syntax is \"set [position] [value]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  int bytePos = atoi(argv[1]);
  byte byteVal = (byte)atoi(argv[2]);

  displayBuffer[bytePos] = byteVal;
  
  shell_println("Byte set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to change the bytes in the display buffer. 
 */
int set_bit(int argc, char** argv)
{
  shell_println("Running \"set\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s, arg3=%s\n\r", argc, argv[0], argv[1], argv[2]);
  if(argc != 3) {
    shell_println("Error: two arguments \"position\" and \"bit\" were not provided.");
    shell_println("       Command Syntax is \"set [position] [bit]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  displayBuffer[atoi(argv[1])] |= (1 << atoi(argv[2]));
  
  shell_println("Bit set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to change the bytes in the display buffer. 
 */
int clear_bit(int argc, char** argv)
{
  shell_println("Running \"clr\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s, arg3=%s\n\r", argc, argv[0], argv[1], argv[2]);
  if(argc != 3) {
    shell_println("Error: two arguments \"position\" and \"bit\" were not provided.");
    shell_println("       Command Syntax is \"set [position] [bit]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  displayBuffer[atoi(argv[1])] &= ~(1 << atoi(argv[2]));
  
  shell_println("Bit cleared!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to change the bytes in the display buffer. 
 */
int set_bits(int argc, char** argv)
{
  shell_println("Running \"setbits\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s, arg3=%s\n\r", argc, argv[0], argv[1], argv[2]);
  if(argc != 3) {
    shell_println("Error: two arguments \"position\" and \"value\" were not provided.");
    shell_println("       Command Syntax is \"set [position] [value]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  int bytePos = atoi(argv[1]);
  byte value = 0;
  for (int i=0; i< strlen(argv[2]); i++)  // for every character in the string  strlen(s) returns the length of a char array
  {
    value *= 2; // double the result so far
    if (argv[2][i] == '1') value++;  //add 1 if needed
  }

  displayBuffer[bytePos] = value;
  
  shell_println("Byte set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to change the bytes in the display buffer. 
 */
int set_all_bits(int argc, char** argv)
{
  shell_println("Running \"setbits\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s\n\r", argc, argv[0], argv[1]);
  if(argc != 2) {
    shell_println("Error: argument \"value\" was not provided.");
    shell_println("       Command Syntax is \"setall [value]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  byte value = 0;
  
  for (int i=0; i< strlen(argv[1]); i++)  // for every character in the string  strlen(s) returns the length of a char array
  {
    value *= 2; // double the result so far
    if (argv[1][i] == '1') value++;  //add 1 if needed
  }
  
  for(int i = 0; i < 12; i++) {
    displayBuffer[i] = value;
  }
  
  shell_println("bytes set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Function to read data from serial port
 * Functions to read from physical media should use this prototype:
 * int my_reader_function(char * data)
 */
int shell_reader(char * data)
{
  // Wrapper for Serial.read() method
  if (Serial.available()) {
    *data = Serial.read();
    return 1;
  }
  return 0;
}

/**
 * Function to write data to serial port
 * Functions to write to physical media should use this prototype:
 * void my_writer_function(char data)
 */
void shell_writer(char data)
{
  // Wrapper for Serial.write() method
  Serial.write(data);
}
