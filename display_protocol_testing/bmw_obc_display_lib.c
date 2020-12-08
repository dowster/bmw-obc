
#include "bmw_obc_display_lib_converted.h"


#define byte unsigned char

#ifdef __cplusplus
extern "C" {
#endif

uint32_t digits[10] = {0};


byte obc_buffer[2][12] = {0};

byte obc_active_buffer = 0;

/**
 * Set any of the digits to an arbitraty state. 
 * Use this for drawing custom shapes or symbols that aren't in the
 * ASCII character set.
 */
void set_raw_digit(enum Digits digit, uint32_t state) {
    if(digit < DIGIT_1 || digit > DIGIT_8) {
        return;
    }

    digits[digit] = state;
}

void set_decimal(enum Digits digit, bool on) {
    if(digit == DIGIT_2 || digit == DIGIT_3) {
        if(on) {
            digits[digit] |= 0x80;
        } else {
            digits[digit] &= 0x7F;
        }
    }
}

void set_ascii_digit(enum Digits digit, char val, bool decimal) {
    char offsetVal = 0;

    if(val > 0x20 && val < 0xFF) {
        offsetVal = val - 0x20;
    }

    if(digit >= DIGIT_1 && digit <= DIGIT_4) {
        digits[digit] = 0;
        if(decimal && (digit == DIGIT_2 || digit == DIGIT_3)) {
            digits[digit] |= 0x80;
        }
        // set 7seg ascii
        digits[digit] = pgm_read_word_near(SevenSegmentASCII + offsetVal);
    } else if (digit >= DIGIT_5 && digit <= DIGIT_7) {
        // set 16seg ascii
        digits[digit] = pgm_read_word_near(SixteenSegmentASCII + offsetVal);
    } else if (digit == DIGIT_8) {
        // set 14seg ascii
        digits[digit] = pgm_read_word_near(FourteenSegmentASCII + offsetVal);
    }
}

/**
 * Set a colon's state. Any non-zero state will be treated as on and
 * a zero state will be treated as off.
 */
void set_colon(enum Digits digit, bool state) {
    if(digit == COLON_1 || digit == COLON_2) {
        digits[digit] = (state && true);
    }
}

/**
 * Writes a string to the digit buffers.
 * 
 * @param char* displayStr that is to be displayed
 *              Should be between 0 and 8 characters long. 
 *              if the string is 0 characters long the digits
 *              will be cleared and if the string is 8+ charscters
 *              long anything after 8 characters will not be displayed.
 */
void obc_writestr(char* displayStr) {
    int len = strlen(displayStr);
    int digit = DIGIT_1;

    for(digit; digit < len && digit <= DIGIT_8; digit++) {
        set_ascii_digit(digit, displayStr[digit], false);
    }

    for(digit; digit <= DIGIT_8; digit++) {
        digits[digit] = 0x00;
    }

}

/**
 * TODO: Turn this into a macro? 
 */
static inline byte get_top_seven_segment(uint32_t state) {
    return (byte) ((state >> 4) & 0x0F);
}

/**
 * TODO: Turn this into a macro? 
 */
static inline byte get_bottom_seven_segment(uint32_t state) {
    return (byte) (state & 0x0f);
}

#define get_bottom_seven_seg(state) (state & 0x0F)

static inline byte get_top_sixteen_segment(uint32_t state) {
    byte result = 0;

    // Copy G1 segment bit
    result |= (state & 0x4000) >> 8;
    // Copy F segment
    result |= (state & 0x0080) >> 2;
    // Copy H segment
    result |= (state & 0x0100) >> 3;
    // Copy A1 segment 
    result |= (state & 0x0001) << 3;
    // copy I segment
    result |= (state & 0x0200) >> 7;
    // Copy A2 segment
    result |= (state & 0x0002);
    // Copy J segment
    result |= (state & 0x0400) >> 10;
    return result;
}

static inline byte get_bottom_sixteen_segment(uint32_t state) {
    byte result = 0;

    // Copy C segment bit
    result |= (state & 0x0008) << 4; // 0b1000 0000
    // Copy K segment
    result |= (state & 0x0800) >> 5; // 0b0100 0000
    // Copy D2 segment
    result |= (state & 0x0010) << 1; // 0b0010 0000
    // Copy L segment 
    result |= (state & 0x2000) >> 9; // 0b0001 0000
    // copy D1 segment
    result |= (state & 0x0020) >> 2; // 0b0000 1000
    // Copy M segment
    result |= (state & 0x4000) >> 12; // 0b0000 0100
    // Copy E segment
    result |= (state & 0x0040) >> 5; // 0b0000 0010
    return result;
}

static inline int get_inactive_buffer() {
    return obc_active_buffer == 1 ? 0 : 1;
}

void update_buffer() {
    int inactiveBuffer = get_inactive_buffer();

    // clear the inactive buffer
    for(byte i = 0; i < 12; i++) {
        obc_buffer[inactiveBuffer][i] = 0;
    }

    // Get the top half of the 7-segs and write to buffer
    obc_buffer[inactiveBuffer][0] |= get_top_seven_segment(digits[DIGIT_4]);
    obc_buffer[inactiveBuffer][0] |= get_top_seven_segment(digits[DIGIT_3]) << 4;
    obc_buffer[inactiveBuffer][1] |= get_top_seven_segment(digits[DIGIT_2]);
    obc_buffer[inactiveBuffer][1] |= get_top_seven_segment(digits[DIGIT_1]) << 4;
    obc_buffer[inactiveBuffer][2] |= get_bottom_seven_segment(digits[DIGIT_1]);
    obc_buffer[inactiveBuffer][2] |= get_bottom_seven_segment(digits[DIGIT_2]) << 4;
    obc_buffer[inactiveBuffer][3] |= get_bottom_seven_segment(digits[DIGIT_3]);
    obc_buffer[inactiveBuffer][3] |= get_bottom_seven_segment(digits[DIGIT_4]) << 4;

    // Set the decimal, colons, and + characters. 
    obc_buffer[inactiveBuffer][2] |= digits[COLON_1] << 4;
    obc_buffer[inactiveBuffer][4] |= digits[COLON_2] << 7;
    obc_buffer[inactiveBuffer][2] |= (digits[DIGIT_1] & 0x01);
    obc_buffer[inactiveBuffer][3] |= (digits[DIGIT_2] & 0x01);
    obc_buffer[inactiveBuffer][3] |= (digits[DIGIT_3] & 0x01) << 4;

    // Get the 16-segs. Good luck.
    obc_buffer[inactiveBuffer][4] |= get_top_sixteen_segment(digits[DIGIT_6]);
    obc_buffer[inactiveBuffer][5] |= get_top_sixteen_segment(digits[DIGIT_5]) << 1;
    obc_buffer[inactiveBuffer][6] |= get_bottom_sixteen_segment(digits[DIGIT_5]);
    obc_buffer[inactiveBuffer][7] |= get_bottom_sixteen_segment(digits[DIGIT_6]);
    obc_buffer[inactiveBuffer][8] |= get_bottom_sixteen_segment(digits[DIGIT_7]);
    // Time to deal with the oddball digit #8
    // Or not...

    obc_buffer[inactiveBuffer][11] |= get_top_sixteen_segment(digits[DIGIT_7]);
    // Get the B segments on the 16-segment display
    obc_buffer[inactiveBuffer][5] |= (digits[DIGIT_5] & 0x0040) >> 2;
    obc_buffer[inactiveBuffer][10] |= (digits[DIGIT_5] & 0x0040) << 5;
    obc_buffer[inactiveBuffer][11] |= (digits[DIGIT_5] & 0x0040) << 5;
    // Getting the G2 segments
    obc_buffer[inactiveBuffer][7] |= (digits[DIGIT_5] & 0x0040) >> 11;
    obc_buffer[inactiveBuffer][8] |= (digits[DIGIT_6] & 0x0040) >> 11;
    obc_buffer[inactiveBuffer][9] |= (digits[DIGIT_7] & 0x0040) >> 11;

    obc_active_buffer = (obc_active_buffer == 0) ? 1 : 0;
}
#ifdef __cplusplus
}
#endif
