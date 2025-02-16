#ifndef _TWOX4X7SEG_74HC164_H_
#define _TWOX4X7SEG_74HC164_H_

#include <Arduino.h>

class TwoX4X7seg{
    public:
        
        TwoX4X7seg(void);
        
            void begin(uint8_t dataPin, uint8_t clockPin);

            void write(uint8_t digit, uint8_t value, bool dot=false);

            void write_number_line(uint16_t value, uint8_t line=1);
            
            void refresh(void);
        
    private:
        uint8_t _data, _clk;
        uint8_t _index;
        uint8_t _activeDigits;
        uint8_t _out[8]; 
         
};

#endif

