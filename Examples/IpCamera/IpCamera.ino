/*
  Project IpCamera, 2020.02.20

  Hardware:
 - Arduino Uno board
 - ARSHXNS board
 - ArduCam mini

  This IpCamera acts as a web server. The camera MJPEG stream can be viewed from a browser.

  This sketch is similar with the one using ARSHXIP, the difference being that it uses two sockets instead one and more RAM memory.
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
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "global.h"
#include "const_prog_mem.h"
extern "C" {
  #include "i2cmaster.h"
};

const uint16_t ui16Port = 80; // socket port
const uint8_t ui8ArrHostIP[] = {192, 168, 1, 29}; // Host IP Address.
const uint8_t ui8ArrHostSM[] = {255, 255, 255, 0}; // Host subnet mask
const uint8_t ui8ArrGatewayIP[] = {192, 168, 1, 1}; // Gateway IP Address

void setup() {
  // put your setup code here, to run once:
  
  config();
// _delay_ms(500);

  i2c_init(); // initialize I2C library

  // initialize variables
  for (ui8For = 0; ui8For < nSockets; ui8For++)
  {
    tblSk[ui8For].ui8Task = 0;
    tblSk[ui8For].ui16Cnt5ms = 0;
  }
  _delay_ms(200); // wait for SEI to start

  // write GC register in order to reset SEI
  ui8ArRxTx[0] = (ui8GC << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArRxTx[2] = (1 << SeiGC_bpSWRST) >> 8; ui8ArRxTx[3] = 0;
  ui8ArRxTx[4] = ui16SeiSMR_SWRST >> 8; ui8ArRxTx[5] = ui16SeiSMR_SWRST & 255;
  SpiW(6); // write GC

  // reset CPLD
  SpiArCamW1(0x87, 0x80);
  _delay_ms(100);
  SpiArCamW1(0x87, 0x00);
  _delay_ms(100);

  i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
  i2c_write(0xFF); // write reg address
  i2c_write(0x01); // write data
  i2c_stop();
  _delay_ms(1);

  i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
  i2c_write(0x12); // write reg address
  i2c_write(0x80); // write data
  i2c_stop();
  _delay_ms(100);

  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_JPEG_INIT) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_JPEG_INIT[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_JPEG_INIT[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }

  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_YUV422) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_YUV422[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_YUV422[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }

  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_JPEG) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_JPEG[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_JPEG[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }

  i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
  i2c_write(0xFF); // write reg address
  i2c_write(0x01); // write data
  i2c_stop();

  i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
  i2c_write(0x15); // write reg address
  i2c_write(0x00); // write data
  i2c_stop();
/*
  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_320x240_JPEG) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_320x240_JPEG[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_320x240_JPEG[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }

  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_640x480_JPEG) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_640x480_JPEG[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_640x480_JPEG[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }
*/
  // custom resolution 1600*900
  for (uint8_t ui8For = 0; ui8For < (sizeof(OV2640_1600x900_JPEG) >> 1); ui8For++) {
    i2c_start_wait(0x60 + I2C_WRITE); // set device address and write mode
    i2c_write(pgm_read_byte(&OV2640_1600x900_JPEG[ui8For << 1])); // write reg address
    i2c_write(pgm_read_byte(&OV2640_1600x900_JPEG[(ui8For << 1) + 1])); // write data
    i2c_stop();
  }

  // enable interrupt output pin
  //  ui8ArRxTx[0] = ui8VaSl2GC | bvWCR; // virtual address, bit value Write Common Register / same as the previous write
  ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << SeiGC_bpIPINE | 1 << SeiGC_bpMIEN);
  SpiW(4); // write GC

  // write Host IP Address, Subnet Mask and Gateway IP Address
  ui8ArRxTx[0] = (ui8HHG << 2) | bvWCR; // virtual address, r/w, g/s
  ui8ArRxTx[2] = ui8ArrHostIP[0]; ui8ArRxTx[3] = ui8ArrHostIP[1]; ui8ArRxTx[4] = ui8ArrHostIP[2]; ui8ArRxTx[5] = ui8ArrHostIP[3];
  ui8ArRxTx[6] = ui8ArrHostSM[0]; ui8ArRxTx[7] = ui8ArrHostSM[1]; ui8ArRxTx[8] = ui8ArrHostSM[2]; ui8ArRxTx[9] = ui8ArrHostSM[3];
  ui8ArRxTx[10] = ui8ArrGatewayIP[0]; ui8ArRxTx[11] = ui8ArrGatewayIP[1]; ui8ArRxTx[12] = ui8ArrGatewayIP[2]; ui8ArRxTx[13] = ui8ArrGatewayIP[3];
  SpiW(14); // write GIPA

  // set socket 0 start buffer and size
  ui8ArRxTx[0] = (ui8SkBSPS << 2) | bvWSR; // virtual address, bit value Write Common Register
  ui8ArRxTx[1] = 0; // socket 0
  ui8ArRxTx[2] = 24; // transmit size
  ui8ArRxTx[3] = 1; // starts at
  ui8ArRxTx[4] = 0; // receive size
  ui8ArRxTx[5] = 0; // starts at
  SpiW(6); // write SkBSPS - Buffer Start Pointer and Size

  // set socket 1 start buffer and size
  ui8ArRxTx[0] = (ui8SkBSPS << 2) | bvWSR; // virtual address, bit value Write Common Register
  ui8ArRxTx[1] = 1; // socket 1
  ui8ArRxTx[2] = 24; // transmit size
  ui8ArRxTx[3] = 27; // starts at
  ui8ArRxTx[4] = 0; // receive size
  ui8ArRxTx[5] = 26; // starts at
  SpiW(6); // write SkBSPS - Buffer Start Pointer and Size

  // ArpCrl
  ui8ArRxTx[0] = (ui8ArpCtrl << 2) | bvWCR; // virtual address, bit value Write Common Register
  ui8ArRxTx[2] = (1 << 7 | 3); ui8ArRxTx[3] = 2;
  SpiW(4); // write ArpCtrl
  
  // make port B pins as outputs by writing 0 to TRISE
  ui8ArRxTx[0] = (ui8TRISB << 2) | bvWCR; ui8ArRxTx[1] = 0;
  ui8ArRxTx[2] = 0; ui8ArRxTx[3] = 0;
  SpiW(4); // write TRISE

  // make port E pins as outputs by writing 0 to TRISE
  ui8ArRxTx[0] = (ui8TRISE << 2) | bvWCR; ui8ArRxTx[1] = 0;
  SpiW(2); // write TRISE

  SpiArCamW1(0x84, 0x01);

}

void loop() {
  // put your main code here, to run repeatedly:
    if (TIFR1 & (1 << OCF1A)) { // check for 5ms tick
      TIFR1 = (1 << OCF1A);
      for (ui8For = 0; ui8For < nSockets; ui8For++) {
        tblSk[ui8For].ui8Flag5ms = 255; // set 5ms flags for all sockets so they can implement their own independent timers
        tblSk[ui8For].ui8If = 1; // set the interrupt flag every 5ms in order to service the respective socket even no real interrupt has been received from SEI
      }
      ui8GpFlag5ms = 255; // set 5ms flags for general registers
      ui8Cnt5ms ++;

      if (ui8Cnt5ms > 199) {
        ui8Cnt5ms = 0;
      }
      // UDR0 = 1;
    }
    // place other tasks here

    // task Ethernet

    // read interrupt register
    if ((PIND & (1 << bpEthPinInt)) == 0) {
      SpiR4((ui8IF << 2) | bvRCR, 0); // reading the interrupt register clears the interrupt pin. Here the interrupt pin is polled instead to trigger an external interrupt
      ui16IntFlags = ui16IntFlags | (ui8ArrR6[2] << 8) | ui8ArrR6[3];
      for (ui8For = 0; ui8For < nSockets; ui8For++) {
        if ((ui8ArrR6[1] & (1 << ui8For)) > 0) {
          tblSk[ui8For].ui8If = 1;
          //  UDR0 = ui8For;
        }
      }
    }
    
    if (ui16IntFlags > 0) {
      ui16IntFlags = 0;
      // handle here interrupts from ICMP, DHCP, etc
    }

    for (ui8For = 0; ui8For < nSockets; ui8For++) {
      ui8SkRun = ui8SkRun + 1;
      if (ui8SkRun == nSockets) {ui8SkRun = 0;};

      if (tblSk[ui8SkRun].ui8If > 0) {
        pArrfSk[tblSk[ui8SkRun].ui8Task]();
        break;
      } // if
    } // for
} // loop

  
