#ifdef F_CPU
#undef F_CPU
  #define F_CPU 16000000UL
#endif
//#define _NOP() do {__asm__ __volatile__ ("nop");} while (0)

#include <Arduino.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "global.h"

void setup() {
  // put your setup code here, to run once:
  
  config();

  // initialize variables
  ui8TaskDhcp = 0;
  
  // write GC register in order to reset SEI
  ui8ArTx[0] = (ui8GC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[2] = (1 << SeiGC_bpSWRST) >> 8; ui8ArTx[3] = 0;
  ui8ArTx[4] = ui16SeiSMR_SWRST >> 8; ui8ArTx[5] = ui16SeiSMR_SWRST & 255;
  SpiW(6); // write GC

  // make port B pins as outputs by writing 0 to TRISE
  ui8ArTx[0] = (ui8TRISB << 2) | bvWCR; ui8ArTx[1] = 0;
  ui8ArTx[2] = 0; ui8ArTx[3] = 0;
  SpiW(4); // write TRISE

  // make port E pins as outputs by writing 0 to TRISE
  ui8ArTx[0] = (ui8TRISE << 2) | bvWCR; ui8ArTx[1] = 0;
  SpiW(2); // write TRISE

}

void loop() {
  if (TIFR1 & (1 << OCF1A)) { // check for 5ms tick
    TIFR1 = (1 << OCF1A);

    ui8Cnt5ms ++;

    if (ui8Cnt5ms > 199) {
      ui8Cnt5ms = 0;
      ui8GpFlag1s = 255;
      ui8Cnt10s++;
      
      // task DHCP once a second
      pArrDhcp[ui8TaskDhcp]();

      if (ui8Cnt10s > 9) {
        ui8Cnt10s = 0;
        
        SpiR4((ui8HHG << 2) | bvRCR, 0); // HHG register is 12 bytes.
                                         // Reading only 4 bytes of it makes the SPI to lose synchronisation.
                                         // To address this, read GC register and discard the result
        for (uint8_t ui8For = 0; ui8For < 15; ui8For++) {
          while(!(UCSR0A & (1 << 5))) {}
          UDR0 = pgm_read_byte(&strText[ui8For]);
        }

        itoa(ui8ArrR6[0], ui8Digit, 10);
        for (uint8_t ui8For = 0; ui8For < strlen(ui8Digit); ui8For++) {
          while(!(UCSR0A & (1 << 5))) {}
          UDR0 = ui8Digit[ui8For];
        }
        while(!(UCSR0A & (1 << 5))) {}
        UDR0 = '.';
      
        itoa(ui8ArrR6[1], ui8Digit, 10);
        for (uint8_t ui8For = 0; ui8For < strlen(ui8Digit); ui8For++) {
          while(!(UCSR0A & (1 << 5))) {}
          UDR0 = ui8Digit[ui8For];
        }
        while(!(UCSR0A & (1 << 5))) {}
        UDR0 = '.';
      
        itoa(ui8ArrR6[2], ui8Digit, 10);
        for (uint8_t ui8For = 0; ui8For < strlen(ui8Digit); ui8For++) {
          while(!(UCSR0A & (1 << 5))) {}
          UDR0 = ui8Digit[ui8For];
        }
        while(!(UCSR0A & (1 << 5))) {}
        UDR0 = '.';
      
        itoa(ui8ArrR6[3], ui8Digit, 10);
        for (uint8_t ui8For = 0; ui8For < strlen(ui8Digit); ui8For++) {
          while(!(UCSR0A & (1 << 5))) {}
          UDR0 = ui8Digit[ui8For];
        }
        
        while(!(UCSR0A & (1 << 5))) {}
        UDR0 = 13;
        while(!(UCSR0A & (1 << 5))) {}
        UDR0 = 10;

        // read GC register and discard the result
        SpiR2((ui8GC << 2) | bvRCR, 0); // this restores the SPI synchronization
      }
    }
  }
}

  
