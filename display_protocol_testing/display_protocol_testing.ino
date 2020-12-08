/**
 * Including the GeekFactory Shell Library, found in "Sketch -> Include Library -> Manage Libraries"
 */
#include <Shell.h>

#include "bmw_obc_display_lib_converted.h"

/**
 * This is the pin on the arduino that will be connected to the data pin on the ribbon cable. 
 * This doesn't need to be pin 3 on the arduino but it does need to be pin 3 on the ribbon cable.
 */
#define DATA_PIN 3

/**
 * This is the pin on the arduino that will be connected to the clock pin on the ribbon cable. 
 * This doesn't need to be pin 4 on the arduino but it does need to be pin 4 on the ribbon cable.
 */
#define CLOCK_PIN 4 

/**
 * TICK_DELAY is a constant that controls how many microseconds the arduino will wait before changing
 * the state of the data or clock lines. This is time is 15microseconds on the original PCB but it
 * seems to work just fine being scaled down to 5. The 15 on the original PCB may have just been a
 * restriction of the micro controller used.
 */
#define TICK_DELAY 5

/**
 * The display operates on a 12 byte / 96 bit buffer which allows
 * 
 * The specification for which bit controls which LCD segment is included
 * in the "docs/display_buffer_mapping.md" file (Not yet available).
 */

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
  //for(int i = 0; i < 12; i++) {
  //  displayBuffer[i] = 0;
  //}
  
  // Prepare serial communication
  Serial.begin(9600);

  shell_println("Starting display prototype");
  
  // Initialize command line interface (CLI)
  // We pass the function pointers to the read and write functions that we implement below
  // We can also pass a char pointer to display a custom start message
  shell_init(shell_reader, shell_writer, 0);
  
  // Add commands to the shell
  shell_register(set_all_bits, PSTR("setall"));
  
  // Add commands to the shell
  shell_register(set_bits, PSTR("setb"));

  
  // Add commands to the shell
  shell_register(set_str, PSTR("set"));
  
  // Add commands to the shell
  shell_register(clear_bit, PSTR("clr"));
  
  // Add commands to the shell
  shell_register(get_display_buffer, PSTR("get"));
  char testStr[] = {'1','2','3','4','T','E','S','T',0};
  obc_writestr(testStr);

  update_buffer();
  update_buffer();
  update_buffer();
}
int loopCount = 0;
void loop() {
  if(0 == (loopCount %= 40)) {
    // put your main code here, to run repeatedly:
    sendStartMessage();
    for(int i = 0; i < 12; i++) {
      sendByte(obc_buffer[obc_active_buffer][i], i == 11);
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

/**
 * Send a byte to the display
 * 
 * @param data to send
 * @param lastByte is true if we're sending byte 11, 
 *        the data line should be held high during the 
 *        falling edge of the last bit's clock signal. 
 */
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
      // Every 4th bit the stock micro has an extra 85 microseconds
      // of waiting / clock time. I suspect this is so it can move
      // the next four bits into a buffer or do some other work.
      // The display seems to work just fine without this delay.
      //delayMicroseconds(85); 
    }

    // On the last nibble of the 24 nibble message it seems like the data line has to leave high.
    // This will make sure that the data line is left high on the last bit of the last nibble
    digitalWrite(DATA_PIN, i == 7 && lastByte);
    
    delayMicroseconds(TICK_DELAY);
    digitalWrite(CLOCK_PIN, LOW);
  }
}

/**
 * Format a byte into a string to print it nicely.
 * 
 * @param val to convert to string
 * @param charbuffer points to a buffer of type char[8]
 */
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
    format_bits(obc_buffer[obc_active_buffer][i], binaryString);
    shell_printf("%s ", binaryString);
  }
  shell_println("");
  
  return SHELL_RET_SUCCESS;
}

/**
 * Command to set a particular bit in the buffer to active. 
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

  obc_buffer[obc_active_buffer][atoi(argv[1])] |= (1 << atoi(argv[2]));
  
  shell_println("Bit set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Command to set a particular bit in the buffer to active. 
 */
int set_str(int argc, char** argv)
{
  shell_println("Running \"set\" now");
  shell_printf("argc=%d, arg1=%s, arg2=%s, arg3=%s\n\r", argc, argv[0], argv[1], argv[2]);
  if(argc != 2) {
    shell_println("Error: Need a string to write");
    shell_println("       Command Syntax is \"set [str]");
    shell_printf("%d args provided\n\r", argc);
    return SHELL_RET_FAILURE;
  }

  obc_writestr(argv[1]);
  update_buffer();
  
  shell_println("Bit set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Command to clear a particular bit in the buffer.  
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

  obc_buffer[obc_active_buffer][atoi(argv[1])] &= ~(1 << atoi(argv[2]));
  
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
    shell_println("       Command Syntax is \"set [position] [value]\" Example: set 2 01010101");
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

  obc_buffer[obc_active_buffer][bytePos] = value;
  
  shell_println("Byte set!! Yay!!");
  return SHELL_RET_SUCCESS;
}

/**
 * Set comand used to set all bytes in the display buffer to the same value. 
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
    obc_buffer[obc_active_buffer][i] = value;
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
