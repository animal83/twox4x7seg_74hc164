#include <Arduino.h>
#include "twox4x7seg_74hc164.h"
/*
    List of character an Positon
    led_character
    POS => Char => Hex
    0   =>  0   => 0x3F
    1   =>  1   => 0x06
    2   =>  2   => 0x5B
    3   =>  3   => 0x4F
    4   =>  4   => 0x66
    5   =>  5   => 0x6D
    6   =>  6   => 0x7D
    7   =>  7   => 0x07
    8   =>  8   => 0x7F
    9   =>  9   => 0x6F
    10  =>  E   => 0x79
    11  =>  r   => 0x50
    12  =>  -   => 0x40
    13  =>  n   => 0x54
    14  =>  o   => 0x5C
    15  =>  P   => 0x73
*/

const uint8_t led_character[] PROGMEM = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x79, 0x50, 0x40, 0x54, 0x5c, 0x74
};


TwoX4X7seg::TwoX4X7seg(void)
    : _index(0), _activeDigits(0){ 
    memset(_out, 0xFF, sizeof(_out)); // Init all LEDs and switch it off 
}

void TwoX4X7seg::begin(uint8_t dataPin, uint8_t clockPin){
    _data = dataPin;
    _clk = clockPin;
    
    pinMode(_clk, OUTPUT);
    pinMode(_data , OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_data, LOW);
}

// Call this function frequently to refresh the next digit.
void TwoX4X7seg::refresh(void){
    if(_activeDigits == 0) return;  // No active digit, do nothing

     // Find the next active digit (modulo 8) 
    do {
        _index = (_index + 1) & 0x07; // Jump to the next digit (modulo 8)
    } while ((_activeDigits & (1 << _index)) == 0); // Skip inactive digits

    
    // First shiftOut: Sends the inverted segment pattern (for the cathodes).
    // Second shiftOut: Selectively activates the digits (anodes).
    
    // First, shift out the 8 bits for the value...
    shiftOut(_data, _clk, MSBFIRST, _out[_index]); // shift the fist 8 bit with the number 
    delayMicroseconds(1);

    // ... then shift out the position (with one active bit)
    shiftOut(_data, _clk, MSBFIRST, 1 << _index);
    delayMicroseconds(1);
}

// Write a single character to a specific digit.
void TwoX4X7seg::write(uint8_t digit, uint8_t value, bool dot=false){

    // Check: Only allow digits 0 to 7 and valid character indices.
    if (digit >= 8 || value >= (sizeof(led_character) / sizeof(led_character[0]))) return;

    // Retrieve the character from flash memory and add the dot if needed.
    uint8_t led_char = pgm_read_byte(&led_character[value]) | (dot ? 0x80 : 0x00); // Add 0x80 if dot true

    // Invert the bits (active low for the LED)(for cathodes).
    _out[digit] = ~led_char;  

     // Mark this digit as active.
    _activeDigits |= (1 << digit); 
}


void  TwoX4X7seg::write_number_line(uint16_t value, uint8_t line=1){
  // Only display values up to 9999.
    if (value > 9999) return;

    // Split the value into its four digits:
    // d[0] -> ones, d[1] -> tens, d[2] -> hundreds, d[3] -> thousands.
    uint8_t d[4];
    d[0] = value % 10;
    d[1] = (value / 10) % 10;
    d[2] = (value / 100) % 10;
    d[3] = (value / 1000) % 10;

    // Write the digits to the corresponding positions based on the line.
    if (line == 1) {
        // Line 1: Positions 4 to 7 (from left: thousands to ones)
        write(4, d[3]);
        write(5, d[2]);
        write(6, d[1]);
        write(7, d[0]);
    } else {
        // Line 0: Positions 0 to 3
        write(0, d[3]);
        write(1, d[2]);
        write(2, d[1]);
        write(3, d[0]);
    }
}
