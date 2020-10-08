
#include "bmw_obc_display_lib.h"

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

short digits[11] = {0};

byte buffer[2][12] = {0};

int activeBuffer = 0;

/**
 * Set any of the digits to an arbitraty state. 
 * Use this for drawing custom shapes or symbols that aren't in the
 * ASCII character set.
 */
void set_raw_digit(enum Digits digit, short state) {
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
        digits[digit] = SevenSegmentASCII[offsetVal];
    } else if (digit >= DIGIT_5 && digit <= DIGIT_7) {
        // set 16seg ascii
        digits[digit] = SixteenSegmentASCII[offsetVal];
    } else if (digit == DIGIT_8) {
        // set 14seg ascii
        digits[digit] = FourteenSegmentASCII[offsetVal];
    }
}

/**
 * Set a colon's state. Any non-zero state will be treated as on and
 * a zero state will be treated as off.
 */
void set_colon(enum Digits digit, short state) {
    if(digit == COLON_1 || digit == COLON_2) {
        digits[digit] = (state > 0);
    }
}

void obc_writestr(char* displayStr) {
    int length = strlen(displayStr);

    // TODO
}

byte get_top_seven_segment(short state) {
    byte result = 0;
    // copy A segment bit
    result |= (state & 0x01) << 2;
    // copy the B segment bit
    result |= (state & 0x02);
    // copy the G segment bit
    result |= (state & 0x40) >> 6;
    // copy the F segment bit
    result |= (state & 0x20) >> 2;
    return result;
}

byte get_bottom_seven_segment(short state) {
    byte result = 0;

    // copy C segment bit
    result |= (state & 0x04) << 1;
    // copy D segment bit
    result |= (state & 0x08) >> 1;
    // copy E segment bit
    result |= (state & 0x10) >> 3;
    return result;
}

byte get_top_sixteen_segment(short state) {
    short result = 0;

    // Copy G1 segment bit
    result |= (state & 0x4000) >> 8;
    // Copy F segment
    result |= (state & 0x0080) >> 2;
    // Copy H segment
    result |= (state & 0x0100) >> 3;
    // Copy A1 segment 
    result |= (state & 0x0001) << 3;
    // copy I segment
    result |= (state & 0x0200) >> 6;
    // Copy A2 segment
    result |= (state & 0x0002);
    // Copy J segment
    result |= (state & 0x0400) >> 10;
    return result;
}

byte get_bottom_sixteen_segment(short state) {
    short result = 0;

    // Copy C segment bit
    result |= (state & 0x0008) << 4;
    // Copy K segment
    result |= (state & 0x0800) >> 5;
    // Copy D2 segment
    result |= (state & 0x0010) << 1;
    // Copy L segment 
    result |= (state & 0x1000) >> 8;
    // copy D1 segment
    result |= (state & 0x0020) >> 2;
    // Copy M segment
    result |= (state & 0x2000) >> 12;
    // Copy E segment
    result |= (state & 0x0040) >> 5;
    return result;
}

int get_inactive_buffer() {
    return activeBuffer == 1 ? 0 : 1;
}

void update_buffer() {
    int inactiveBuffer = get_inactive_buffer();

    // clear the inactive buffer
    for(byte i = 0; i < 12; i++) {
        buffer[inactiveBuffer][i] = 0;
    }

    // Get the top half of the 7-segs and write to buffer
    buffer[inactiveBuffer][0] |= get_top_seven_segment(digits[DIGIT_4]);
    buffer[inactiveBuffer][0] |= get_top_seven_segment(digits[DIGIT_3]) << 4;
    buffer[inactiveBuffer][1] |= get_top_seven_segment(digits[DIGIT_2]);
    buffer[inactiveBuffer][1] |= get_top_seven_segment(digits[DIGIT_1]) << 4;
    buffer[inactiveBuffer][2] |= get_bottom_seven_segment(digits[DIGIT_1]);
    buffer[inactiveBuffer][2] |= get_bottom_seven_segment(digits[DIGIT_2]) << 4;
    buffer[inactiveBuffer][3] |= get_bottom_seven_segment(digits[DIGIT_3]);
    buffer[inactiveBuffer][3] |= get_bottom_seven_segment(digits[DIGIT_4]) << 4;

    // Set the decimal, colons, and + characters. 
    buffer[inactiveBuffer][2] |= digits[COLON_1] << 4;
    buffer[inactiveBuffer][4] |= digits[COLON_2] << 7;
    buffer[inactiveBuffer][2] |= (digits[DIGIT_1] & 0x80) ? 0x01 : 0x00;
    buffer[inactiveBuffer][3] |= (digits[DIGIT_2] & 0x80) ? 0x01 : 0x00;
    buffer[inactiveBuffer][3] |= (digits[DIGIT_3] & 0x80) ? 0x10 : 0x00;

    // Get the 16-segs. Good luck.
    buffer[inactiveBuffer][4] |= get_top_sixteen_segment(digits[DIGIT_6]);
    buffer[inactiveBuffer][5] |= get_top_sixteen_segment(digits[DIGIT_5]) << 1;
    buffer[inactiveBuffer][6] |= get_bottom_sixteen_segment(digits[DIGIT_5]);
    buffer[inactiveBuffer][7] |= get_bottom_sixteen_segment(digits[DIGIT_6]);
    buffer[inactiveBuffer][8] |= get_bottom_sixteen_segment(digits[DIGIT_7]);
    // Time to deal with the oddball digit #8
    // Or not...

    buffer[inactiveBuffer][11] |= get_top_sixteen_segment(digits[DIGIT_7]);
    // Get the B segments on the 16-segment display
    buffer[inactiveBuffer][5] |= (digits[DIGIT_5] & 0x0040) >> 2;
    buffer[inactiveBuffer][10] |= (digits[DIGIT_5] & 0x0040) << 5;
    buffer[inactiveBuffer][11] |= (digits[DIGIT_5] & 0x0040) << 5;
    // Getting the G2 segments
    buffer[inactiveBuffer][7] |= (digits[DIGIT_5] & 0x0040) >> 11;
    buffer[inactiveBuffer][8] |= (digits[DIGIT_6] & 0x0040) >> 11;
    buffer[inactiveBuffer][9] |= (digits[DIGIT_7] & 0x0040) >> 11;
}

void update_display() {

}
