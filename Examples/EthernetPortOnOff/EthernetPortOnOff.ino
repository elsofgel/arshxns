/*
  Project EthernetPortOnOff, 2020.02.20

  Hardware:
 - Arduino Uno board
 - ARSHXNS board

   This program turns on and off network switch port 1 while port 2 stays on. This can be used to cut connection downstream.
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

  // write NSRAD register (Network Switch Register Adress and Data)
  ui8ArTx[0] = (ui8NSRAD << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[2] = 1 << 3; // data regsiter
  ui8ArTx[3] = 29; // address register
  SpiW(4);

  // write NSC register (Network Switch Control)
  ui8ArTx[0] = (ui8NSC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 1 << 7; // set bit BUSY for a write operation. Bit LSCIE is thought to be 0
  SpiW(4);

  _delay_ms(5000);

  // write NSRAD register (Network Switch Register Adress and Data)
  ui8ArTx[0] = (ui8NSRAD << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[2] = 0; // data regsiter
  ui8ArTx[3] = 29; // address register
  SpiW(4);

  // write PHYC register (PHY Control)
  ui8ArTx[0] = (ui8NSC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArTx[3] = 1 << 7; // set bit BUSY for a write operation. Bit LSCIE is thought to be 0
  SpiW(4);

  _delay_ms(5000);

}

  
