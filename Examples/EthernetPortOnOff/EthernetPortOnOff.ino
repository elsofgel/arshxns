/*
  Project EthernetPortOnOff, 2020.02.20

  Hardware:
 - Arduino Uno board
 - ARSHXNS board
*/

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
  // make sure bit BUSY in PHYC register is not set
/*  while(1) {
    _delay_us(100);
    SpiR2((ui8PHYC << 2) | bvRCR, 0); // read PHYC register and check for bit BUSY to clear
    if (!(ui8ArrR6[1] & (1 << 7))) {
      break;
    }        
  }
*/
  // write PHYA register (PHY Adress)
  ui8ArTx[0] = (ui8PHYA << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 29;
  SpiW(4);

  // write PHYD register (PHY Data)
  ui8ArTx[0] = (ui8PHYD << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 1 << 3;
  SpiW(4);
  
  // write PHYC register (PHY Control)
  ui8ArTx[0] = (ui8PHYC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 1 << 7; // set bit BUSY for a write operation. Bit LSCIE is thought to be 0
  SpiW(4);

  _delay_ms(10000);



  // write PHYA register (PHY Adress)
  ui8ArTx[0] = (ui8PHYA << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 29;
  SpiW(4);

  // write PHYD register (PHY Data)
  ui8ArTx[0] = (ui8PHYD << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 0;
  SpiW(4);
  
  // write PHYC register (PHY Control)
  ui8ArTx[0] = (ui8PHYC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 1 << 7; // set bit BUSY for a write operation. Bit LSCIE is thought to be 0
  SpiW(4);

  _delay_ms(10000);

}

  
