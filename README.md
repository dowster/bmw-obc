# 80s BMW On-Board Computer Hacking

The purpose of this project is to rebuild the internals of the BMW OBC found in the e24/28/30 cars. All the internals will be replaced except for the backlight, display, and keypad circuitry. To accomplish this I'll need to reverse engineer the [display protocol](./docs/display-protocol.md) and reverse engineer the keypad controls. 

# Display API

## void set_ascii_digit

### Params
 - `Digits digit` to set, can be any of the 7-segment or 16-segment digits
 - `char val` to set the digit to. All ASCII characters from 0x20 to 0xFF are accepted. Not all segments can render all digits so be sure to check the reference for [16-segment](https://github.com/dmadison/LED-Segment-ASCII#16-segment) and [7-segment](https://github.com/dmadison/LED-Segment-ASCII#7-segment) digits. 
 - `bool decimal` sets the decimal point on or off for digits 2 and 3. For digit 1 this flag will enable the H and I segments as part of the plus. The H and I segments can also be enabled by setting `val` to a "+" symbol.

```c
void set_ascii_digit(enum Digits digit, char val, bool decimal);
```

## void set_digit

The set_digit function will allow arbitrary setting of digit segments. This can be used to create output outside of the standard ASCII characters. 

I could just give direct access to the digits array but I'm planning on reducing the ASCII map & bit shifting to just one ASCII map. In that case I may want to have this set function to sanitize the digit states. Sanitation could also be done while copying the digit states to the display buffer. 

### Params
 - `Digit digit` to set
 - `short state` of the digit

```c
void set_digit(Digit digit, short state);
```

## *(Potential)* void set_blink

The blink function will allow the dispay to blink (flash) at a specified interval. 

#### Params
 - `bool enable` the flash behavior
 - `short digits` to flash, bit 0 = digit 1, bit 7 = digit 8, bit 8 = colon 1, bit 9 = colon2
 - `int period` of time from turning the digits on, off, and on again. 

```c
void set_blink(bool enable = 1, short digits = 0x03FF, int period = 1000);
```

## *(Potential)* void write_str

Writes a string across all digits of the display. Valid positions for the colon There should be a space between the 4th and 5th digit. The first group of digits will be right aligned on either the space between the 4th and 5th digits. If the + symbol is followed immediately by a non space character then the text will be left aligned to the +. Strings should be null terminated.

### params 
 - `char* str` to write to the display
 - `return` true if the string could be written, false if there were errors parsing the string

### Examples
```c
write_str("12:45 am\0");
write_str("12.:3.4 56:78\0");
write_str("12:34 56:78\0");
write_str("AB:CD EF:GH\0");
write_str("12.:34 56:78\0");
write_str("12.34 56:78\0");
write_str("12.3.4 5678\0");
write_str("1234 5678\0");
write_str("12.34 TEST\0");
write_str("12:34 TEST\0");
write_str("+ 12 TEST\0");
write_str("+12 TEST\0");
write_str("DISP TEST\0");
```

```c
bool write_str(char* str);
```

## Enum Digits

Used when accessing the digits state array. 

```c
enum Digits {
    DIGIT_1 = 1,
    DIGIT_2 = 2,
    DIGIT_3 = 3,
    DIGIT_4 = 4,
    DIGIT_5 = 5,
    DIGIT_6 = 6,
    DIGIT_7 = 7,
    DIGIT_8 = 8, 
    COLON_1 = 9,
    COLON_2 = 10
};
```