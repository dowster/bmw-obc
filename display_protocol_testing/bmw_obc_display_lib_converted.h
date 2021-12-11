#include "arduino.h"
#include "stdint.h"
#include "string.h"

#ifndef __BMW_OBC_H
#define __BMW_OBC_H

typedef uint8_t byte;

const uint8_t SevenSegmentASCII[96] PROGMEM  = {
	0b00000000, /* (space) */ /* From: 00000000 */
	0b00101001, /* ! */ /* From: 10000110 */
	0b10100000, /* " */ /* From: 00100010 */
	0b10111110, /* # */ /* From: 01111110 */
	0b11011100, /* $ */ /* From: 01101101 */
	0b00110011, /* % */ /* From: 11010010 */
	0b00111000, /* & */ /* From: 01000110 */
	0b10000000, /* ' */ /* From: 00100000 */
	0b11000100, /* ( */ /* From: 00101001 */
	0b01100100, /* ) */ /* From: 00001011 */
	0b11000000, /* * */ /* From: 00100001 */
	0b10010010, /* + */ /* From: 01110000 */
	0b00000010, /* , */ /* From: 00010000 */
	0b00010000, /* - */ /* From: 01000000 */
	0b00000001, /* . */ /* From: 10000000 */
	0b00110010, /* / */ /* From: 01010010 */
	0b11101110, /* 0 */ /* From: 00111111 */
	0b00101000, /* 1 */ /* From: 00000110 */
	0b01110110, /* 2 */ /* From: 01011011 */
	0b01111100, /* 3 */ /* From: 01001111 */
	0b10111000, /* 4 */ /* From: 01100110 */
	0b11011100, /* 5 */ /* From: 01101101 */
	0b11011110, /* 6 */ /* From: 01111101 */
	0b01101000, /* 7 */ /* From: 00000111 */
	0b11111110, /* 8 */ /* From: 01111111 */
	0b11111100, /* 9 */ /* From: 01101111 */
	0b01000100, /* : */ /* From: 00001001 */
	0b01001100, /* ; */ /* From: 00001101 */
	0b11010000, /* < */ /* From: 01100001 */
	0b00010100, /* = */ /* From: 01001000 */
	0b01110000, /* > */ /* From: 01000011 */
	0b01110011, /* ? */ /* From: 11010011 */
	0b01111110, /* @ */ /* From: 01011111 */
	0b11111010, /* A */ /* From: 01110111 */
	0b10011110, /* B */ /* From: 01111100 */
	0b11000110, /* C */ /* From: 00111001 */
	0b00111110, /* D */ /* From: 01011110 */
	0b11010110, /* E */ /* From: 01111001 */
	0b11010010, /* F */ /* From: 01110001 */
	0b11001110, /* G */ /* From: 00111101 */
	0b10111010, /* H */ /* From: 01110110 */
	0b10000010, /* I */ /* From: 00110000 */
	0b00101110, /* J */ /* From: 00011110 */
	0b11011010, /* K */ /* From: 01110101 */
	0b10000110, /* L */ /* From: 00111000 */
	0b01001010, /* M */ /* From: 00010101 */
	0b11101010, /* N */ /* From: 00110111 */
	0b11101110, /* O */ /* From: 00111111 */
	0b11110010, /* P */ /* From: 01110011 */
	0b11110100, /* Q */ /* From: 01101011 */
	0b11100010, /* R */ /* From: 00110011 */
	0b11011100, /* S */ /* From: 01101101 */
	0b10010110, /* T */ /* From: 01111000 */
	0b10101110, /* U */ /* From: 00111110 */
	0b10101110, /* V */ /* From: 00111110 */
	0b10100100, /* W */ /* From: 00101010 */
	0b10111010, /* X */ /* From: 01110110 */
	0b10111100, /* Y */ /* From: 01101110 */
	0b01110110, /* Z */ /* From: 01011011 */
	0b11000110, /* [ */ /* From: 00111001 */
	0b10011000, /* \ */ /* From: 01100100 */
	0b01101100, /* ] */ /* From: 00001111 */
	0b11100000, /* ^ */ /* From: 00100011 */
	0b00000100, /* _ */ /* From: 00001000 */
	0b00100000, /* ` */ /* From: 00000010 */
	0b01111110, /* a */ /* From: 01011111 */
	0b10011110, /* b */ /* From: 01111100 */
	0b00010110, /* c */ /* From: 01011000 */
	0b00111110, /* d */ /* From: 01011110 */
	0b11110110, /* e */ /* From: 01111011 */
	0b11010010, /* f */ /* From: 01110001 */
	0b11111100, /* g */ /* From: 01101111 */
	0b10011010, /* h */ /* From: 01110100 */
	0b00000010, /* i */ /* From: 00010000 */
	0b00001100, /* j */ /* From: 00001100 */
	0b11011010, /* k */ /* From: 01110101 */
	0b10000010, /* l */ /* From: 00110000 */
	0b00001010, /* m */ /* From: 00010100 */
	0b00011010, /* n */ /* From: 01010100 */
	0b00011110, /* o */ /* From: 01011100 */
	0b11110010, /* p */ /* From: 01110011 */
	0b11111000, /* q */ /* From: 01100111 */
	0b00010010, /* r */ /* From: 01010000 */
	0b11011100, /* s */ /* From: 01101101 */
	0b10010110, /* t */ /* From: 01111000 */
	0b00001110, /* u */ /* From: 00011100 */
	0b00001110, /* v */ /* From: 00011100 */
	0b00001010, /* w */ /* From: 00010100 */
	0b10111010, /* x */ /* From: 01110110 */
	0b10111100, /* y */ /* From: 01101110 */
	0b01110110, /* z */ /* From: 01011011 */
	0b00111000, /* { */ /* From: 01000110 */
	0b10000010, /* | */ /* From: 00110000 */
	0b10010010, /* } */ /* From: 01110000 */
	0b01000000, /* ~ */ /* From: 00000001 */
	0b00000000, /* (del) */ /* From: 00000000 */
};

const uint16_t FourteenSegmentASCII[96] PROGMEM  = {
	0b000 0000 0000 0000, /* (space) */ /* Not converted.. */
	0b100 0000 0000 0110, /* ! */ /* Not converted.. */
	0b000 0010 0000 0010, /* " */ /* Not converted.. */
	0b001 0010 1100 1110, /* # */ /* Not converted.. */
	0b001 0010 1110 1101, /* $ */ /* Not converted.. */
	0b011 1111 1110 0100, /* % */ /* Not converted.. */
	0b010 0011 0101 1001, /* & */ /* Not converted.. */
	0b000 0010 0000 0000, /* ' */ /* Not converted.. */
	0b010 0100 0000 0000, /* ( */ /* Not converted.. */
	0b000 1001 0000 0000, /* ) */ /* Not converted.. */
	0b011 1111 1100 0000, /* * */ /* Not converted.. */
	0b001 0010 1100 0000, /* + */ /* Not converted.. */
	0b000 1000 0000 0000, /* , */ /* Not converted.. */
	0b000 0000 1100 0000, /* - */ /* Not converted.. */
	0b100 0000 0000 0000, /* . */ /* Not converted.. */
	0b000 1100 0000 0000, /* / */ /* Not converted.. */
	0b000 1100 0011 1111, /* 0 */ /* Not converted.. */
	0b000 0100 0000 0110, /* 1 */ /* Not converted.. */
	0b000 0000 1101 1011, /* 2 */ /* Not converted.. */
	0b000 0000 1000 1111, /* 3 */ /* Not converted.. */
	0b000 0000 1110 0110, /* 4 */ /* Not converted.. */
	0b010 0000 0110 1001, /* 5 */ /* Not converted.. */
	0b000 0000 1111 1101, /* 6 */ /* Not converted.. */
	0b000 0000 0000 0111, /* 7 */ /* Not converted.. */
	0b000 0000 1111 1111, /* 8 */ /* Not converted.. */
	0b000 0000 1110 1111, /* 9 */ /* Not converted.. */
	0b001 0010 0000 0000, /* : */ /* Not converted.. */
	0b000 1010 0000 0000, /* ; */ /* Not converted.. */
	0b010 0100 0100 0000, /* < */ /* Not converted.. */
	0b000 0000 1100 1000, /* = */ /* Not converted.. */
	0b000 1001 1000 0000, /* > */ /* Not converted.. */
	0b101 0000 1000 0011, /* ? */ /* Not converted.. */
	0b000 0010 1011 1011, /* @ */ /* Not converted.. */
	0b000 0000 1111 0111, /* A */ /* Not converted.. */
	0b001 0010 1000 1111, /* B */ /* Not converted.. */
	0b000 0000 0011 1001, /* C */ /* Not converted.. */
	0b001 0010 0000 1111, /* D */ /* Not converted.. */
	0b000 0000 0111 1001, /* E */ /* Not converted.. */
	0b000 0000 0111 0001, /* F */ /* Not converted.. */
	0b000 0000 1011 1101, /* G */ /* Not converted.. */
	0b000 0000 1111 0110, /* H */ /* Not converted.. */
	0b001 0010 0000 1001, /* I */ /* Not converted.. */
	0b000 0000 0001 1110, /* J */ /* Not converted.. */
	0b010 0100 0111 0000, /* K */ /* Not converted.. */
	0b000 0000 0011 1000, /* L */ /* Not converted.. */
	0b000 0101 0011 0110, /* M */ /* Not converted.. */
	0b010 0001 0011 0110, /* N */ /* Not converted.. */
	0b000 0000 0011 1111, /* O */ /* Not converted.. */
	0b000 0000 1111 0011, /* P */ /* Not converted.. */
	0b010 0000 0011 1111, /* Q */ /* Not converted.. */
	0b010 0000 1111 0011, /* R */ /* Not converted.. */
	0b000 0000 1110 1101, /* S */ /* Not converted.. */
	0b001 0010 0000 0001, /* T */ /* Not converted.. */
	0b000 0000 0011 1110, /* U */ /* Not converted.. */
	0b000 1100 0011 0000, /* V */ /* Not converted.. */
	0b010 1000 0011 0110, /* W */ /* Not converted.. */
	0b010 1101 0000 0000, /* X */ /* Not converted.. */
	0b000 0000 1110 1110, /* Y */ /* Not converted.. */
	0b000 1100 0000 1001, /* Z */ /* Not converted.. */
	0b000 0000 0011 1001, /* [ */ /* Not converted.. */
	0b010 0001 0000 0000, /* \ */ /* Not converted.. */
	0b000 0000 0000 1111, /* ] */ /* Not converted.. */
	0b010 1000 0000 0000, /* ^ */ /* Not converted.. */
	0b000 0000 0000 1000, /* _ */ /* Not converted.. */
	0b000 0001 0000 0000, /* ` */ /* Not converted.. */
	0b001 0000 0101 1000, /* a */ /* Not converted.. */
	0b010 0000 0111 1000, /* b */ /* Not converted.. */
	0b000 0000 1101 1000, /* c */ /* Not converted.. */
	0b000 1000 1000 1110, /* d */ /* Not converted.. */
	0b000 1000 0101 1000, /* e */ /* Not converted.. */
	0b001 0100 1100 0000, /* f */ /* Not converted.. */
	0b000 0100 1000 1110, /* g */ /* Not converted.. */
	0b001 0000 0111 0000, /* h */ /* Not converted.. */
	0b001 0000 0000 0000, /* i */ /* Not converted.. */
	0b000 1010 0001 0000, /* j */ /* Not converted.. */
	0b011 0110 0000 0000, /* k */ /* Not converted.. */
	0b000 0000 0011 0000, /* l */ /* Not converted.. */
	0b001 0000 1101 0100, /* m */ /* Not converted.. */
	0b001 0000 0101 0000, /* n */ /* Not converted.. */
	0b000 0000 1101 1100, /* o */ /* Not converted.. */
	0b000 0001 0111 0000, /* p */ /* Not converted.. */
	0b000 0100 1000 0110, /* q */ /* Not converted.. */
	0b000 0000 0101 0000, /* r */ /* Not converted.. */
	0b010 0000 1000 1000, /* s */ /* Not converted.. */
	0b000 0000 0111 1000, /* t */ /* Not converted.. */
	0b000 0000 0001 1100, /* u */ /* Not converted.. */
	0b000 1000 0001 0000, /* v */ /* Not converted.. */
	0b010 1000 0001 0100, /* w */ /* Not converted.. */
	0b010 1101 0000 0000, /* x */ /* Not converted.. */
	0b000 0010 1000 1110, /* y */ /* Not converted.. */
	0b000 1000 0100 1000, /* z */ /* Not converted.. */
	0b000 1001 0100 1001, /* { */ /* Not converted.. */
	0b001 0010 0000 0000, /* | */ /* Not converted.. */
	0b010 0100 1000 1001, /* } */ /* Not converted.. */
	0b000 1100 1100 0000, /* ~ */ /* Not converted.. */
	0b000 0000 0000 0000, /* (del) */ /* Not converted.. */
};

const uint32_t SixteenSegmentASCII[96] PROGMEM  = {
	0b00000000000000000, /* (space) */ /* Not converted.. */
	0b10000000000001100, /* ! */ /* Not converted.. */
	0b00000001000000100, /* " */ /* Not converted.. */
	0b01010101000111100, /* # */ /* Not converted.. */
	0b01010101010111011, /* $ */ /* Not converted.. */
	0b01110111010011001, /* % */ /* Not converted.. */
	0b01001001101110001, /* & */ /* Not converted.. */
	0b00000001000000000, /* ' */ /* Not converted.. */
	0b00001010000000000, /* ( */ /* Not converted.. */
	0b00100000100000000, /* ) */ /* Not converted.. */
	0b01111111100000000, /* * */ /* Not converted.. */
	0b01010101000000000, /* + */ /* Not converted.. */
	0b00100000000000000, /* , */ /* Not converted.. */
	0b01000100000000000, /* - */ /* Not converted.. */
	0b10000000000000000, /* . */ /* Not converted.. */
	0b00100010000000000, /* / */ /* Not converted.. */
	0b00100010011111111, /* 0 */ /* Not converted.. */
	0b00000010000001100, /* 1 */ /* Not converted.. */
	0b01000100001110111, /* 2 */ /* Not converted.. */
	0b00000100000111111, /* 3 */ /* Not converted.. */
	0b01000100010001100, /* 4 */ /* Not converted.. */
	0b01001000010110011, /* 5 */ /* Not converted.. */
	0b01000100011111011, /* 6 */ /* Not converted.. */
	0b00000000000001111, /* 7 */ /* Not converted.. */
	0b01000100011111111, /* 8 */ /* Not converted.. */
	0b01000100010111111, /* 9 */ /* Not converted.. */
	0b00010001000000000, /* : */ /* Not converted.. */
	0b00100001000000000, /* ; */ /* Not converted.. */
	0b01001010000000000, /* < */ /* Not converted.. */
	0b01000100000110000, /* = */ /* Not converted.. */
	0b00100100100000000, /* > */ /* Not converted.. */
	0b10010100000000111, /* ? */ /* Not converted.. */
	0b00000101011110111, /* @ */ /* Not converted.. */
	0b01000100011001111, /* A */ /* Not converted.. */
	0b00010101000111111, /* B */ /* Not converted.. */
	0b00000000011110011, /* C */ /* Not converted.. */
	0b00010001000111111, /* D */ /* Not converted.. */
	0b01000000011110011, /* E */ /* Not converted.. */
	0b01000000011000011, /* F */ /* Not converted.. */
	0b00000100011111011, /* G */ /* Not converted.. */
	0b01000100011001100, /* H */ /* Not converted.. */
	0b00010001000110011, /* I */ /* Not converted.. */
	0b00000000001111100, /* J */ /* Not converted.. */
	0b01001010011000000, /* K */ /* Not converted.. */
	0b00000000011110000, /* L */ /* Not converted.. */
	0b00000010111001100, /* M */ /* Not converted.. */
	0b00001000111001100, /* N */ /* Not converted.. */
	0b00000000011111111, /* O */ /* Not converted.. */
	0b01000100011000111, /* P */ /* Not converted.. */
	0b00001000011111111, /* Q */ /* Not converted.. */
	0b01001100011000111, /* R */ /* Not converted.. */
	0b01000100010111011, /* S */ /* Not converted.. */
	0b00010001000000011, /* T */ /* Not converted.. */
	0b00000000011111100, /* U */ /* Not converted.. */
	0b00100010011000000, /* V */ /* Not converted.. */
	0b00101000011001100, /* W */ /* Not converted.. */
	0b00101010100000000, /* X */ /* Not converted.. */
	0b01000100010111100, /* Y */ /* Not converted.. */
	0b00100010000110011, /* Z */ /* Not converted.. */
	0b00010001000010010, /* [ */ /* Not converted.. */
	0b00001000100000000, /* \ */ /* Not converted.. */
	0b00010001000100001, /* ] */ /* Not converted.. */
	0b00101000000000000, /* ^ */ /* Not converted.. */
	0b00000000000110000, /* _ */ /* Not converted.. */
	0b00000000100000000, /* ` */ /* Not converted.. */
	0b01010000001110000, /* a */ /* Not converted.. */
	0b01010000011100000, /* b */ /* Not converted.. */
	0b01000000001100000, /* c */ /* Not converted.. */
	0b00010100000011100, /* d */ /* Not converted.. */
	0b01100000001100000, /* e */ /* Not converted.. */
	0b01010101000000010, /* f */ /* Not converted.. */
	0b01010001010100001, /* g */ /* Not converted.. */
	0b01010000011000000, /* h */ /* Not converted.. */
	0b00010000000000000, /* i */ /* Not converted.. */
	0b00010001001100000, /* j */ /* Not converted.. */
	0b00011011000000000, /* k */ /* Not converted.. */
	0b00000000011000000, /* l */ /* Not converted.. */
	0b01010100001001000, /* m */ /* Not converted.. */
	0b01010000001000000, /* n */ /* Not converted.. */
	0b01010000001100000, /* o */ /* Not converted.. */
	0b01000001011000001, /* p */ /* Not converted.. */
	0b01010001010000001, /* q */ /* Not converted.. */
	0b01000000001000000, /* r */ /* Not converted.. */
	0b01010000010100001, /* s */ /* Not converted.. */
	0b01000000011100000, /* t */ /* Not converted.. */
	0b00010000001100000, /* u */ /* Not converted.. */
	0b00100000001000000, /* v */ /* Not converted.. */
	0b00101000001001000, /* w */ /* Not converted.. */
	0b00101010100000000, /* x */ /* Not converted.. */
	0b00000101000011100, /* y */ /* Not converted.. */
	0b01100000000100000, /* z */ /* Not converted.. */
	0b01010001000010010, /* { */ /* Not converted.. */
	0b00010001000000000, /* | */ /* Not converted.. */
	0b00010101000100001, /* } */ /* Not converted.. */
	0b01100110000000000, /* ~ */ /* Not converted.. */
	0b00000000000000000, /* (del) */ /* Not converted.. */
};

enum Digits {
    DIGIT_1 = 0,
    DIGIT_2 = 1,
    DIGIT_3 = 2,
    DIGIT_4 = 3,
    DIGIT_5 = 4,
    DIGIT_6 = 5,
    DIGIT_7 = 6,
    DIGIT_8 = 7,
    COLON_1 = 8,
    COLON_2 = 9
};

#ifdef __cplusplus
extern "C" {
#endif

void obc_writestr(char* displayStr);
void update_buffer();
void set_ascii_digit(enum Digits digit, char val, bool decimal);


extern byte obc_buffer[2][12];

extern byte obc_active_buffer;
#ifdef __cplusplus
}
#endif

#endif
