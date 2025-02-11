#include <Arduino.h>
#include <twox4x7seg_74hc164.h>

#define data_pin 8
#define clk_pin 9

TwoX4X7seg sig2x4;

uint16_t counter = 0;

void setup(){

  sig2x4.begin(data_pin, clk_pin);

    cli();  // Disable interrupts
      // Configure Timer1 (CTC mode)
      TCCR1A = 0;  // Normal operation
      TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  /// CTC mode, Prescaler 1024
      OCR1A = 15624;  // 1-second interval (16MHz clock / 1024 Prescaler / 1Hz - 1)
      TIMSK1 |= (1 << OCIE1A);   // Enable Timer1 Compare Match A interrupt
    sei();  // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
    counter++;  // Increment counter
    if (counter > 9999){
      counter = 0; 
    }
    sig2x4.write_number_line(counter,0); // Display counter in line 0
    sig2x4.write_number_line(counter,1); // Display counter in line 1
}

void loop(){

  sig2x4.refresh();
  delayMicroseconds(2800);
  
}
