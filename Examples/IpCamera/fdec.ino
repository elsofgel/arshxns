// #define _NOP() do {__asm__ __volatile__ ("nop");} while (0)

uint8_t SpiArCamR1(uint8_t ui8RegAddr)
{
  uint8_t ui8A; // a temporary variable

	SPCR = (1 << SPE | 1 << MSTR);
	SPSR = 0;
	PORTD = PORTD & ~(1 << 7);

	SPSR;
	SPDR;
	SPDR = ui8RegAddr;

	while (!(SPSR & (1 << SPIF))) {}
	SPDR; // read byte and discard
	SPDR = 0; // write dummy byte

	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	ui8A = SPDR;

	PORTD = PORTD | (1 << 7);
	
	return ui8A;
}

void SpiArCamRn(uint16_t ui16Len)
{
	uint16_t ui16ArPointer = 4;
	ui16Len = ui16Len + 3;

	// config SPI for camera
	SPCR = (1 << SPE | 1 << MSTR);
	SPSR = 0;
	PORTD = PORTD & ~(1 << 7);

	SPSR;
	SPDR;
	SPDR = 0x3C;
	
	while (!(SPSR & (1 << SPIF))) {}
	SPDR; // read byte and discard
	_NOP();
	_NOP();
	SPDR = 0; // write dummy byte

	while (ui16ArPointer < ui16Len) {
		while (!(SPSR & (1 << SPIF))) {}
		ui8ArRxTx[ui16ArPointer] = SPDR;
		ui16ArPointer ++;
		SPDR = 0; // write dummy byte
	}
	while (!(SPSR & (1 << SPIF))) {}
	ui8ArRxTx[ui16ArPointer] = SPDR;

	PORTD = PORTD | (1 << 7);
}

void SpiArCamW1(uint8_t ui8RegAddr, uint8_t ui8RegValue)
{
	// config SPI for camera
	SPCR = (1 << SPE | 1 << MSTR);
	SPSR = 0;
	PORTD = PORTD & ~(1 << 7);

	SPSR;
	SPDR;
	SPDR = ui8RegAddr;

	while (!(SPSR & (1 << SPIF))) {}
	SPDR; // read byte and discard
	SPDR = ui8RegValue;

	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard

	PORTD = PORTD | (1 << 7);
}

void SpiR2(uint8_t ui8b0, uint8_t ui8b1) // read SCS(virtual address, G/Sk, socket number)
{
	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);

	PORTB = PORTB & ~(1 << 2); // ui8b2 - 2 for SEI, ui8b2 = 7 for camera

	SPSR;
	SPDR;
	SPDR = ui8b0; // write register address, general/socket, read
	// tSR2Loop1
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = ui8b1; // write socket number
	// tSR2Loop2
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = 0; // write a dummy byte
	// tSR2Loop3
	while (!(SPSR & (1 << SPIF))) {} // wait to read GSH byte
	ui8GSH = SPDR; // read GSH byte
	SPDR = 0; // write a dummy byte
	// tSR2Loop4
	while (!(SPSR & (1 << SPIF))) {} // wait to read GSL byte
	ui8GSL = SPDR; // read GSL byte
	SPDR = 0; // write a dummy byte
	// tSR2Loop5
	while (!(SPSR & (1 << SPIF))) {} // wait to read the first byte
	ui8ArrR6[0] = SPDR; // read first byte
	SPDR = 0; // write a dummy byte
	// tSR2Loop6
	while (!(SPSR & (1 << SPIF))) {} // wait to read the second byte
	ui8ArrR6[1] = SPDR; // read the second byte

	PORTB = PORTB | (1 << 2); // bring nCS line up
}

void SpiR4(uint8_t ui8b0, uint8_t ui8b1) // read a four byte register(virtual address, general/socket, socket number)
{
	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);
	PORTB = PORTB & ~(1 << 2);

	SPSR;
	SPDR;
	SPDR = ui8b0; // write register address, general/socket, read
	// tSR4Loop1
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = ui8b1; // write socket number
	// tSR4Loop2
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = 0; // write a dummy byte
	// tSR4Loop3
	while (!(SPSR & (1 << SPIF))) {} // wait to read GSH byte
	ui8GSH = SPDR; // read GSH byte
	SPDR = 0; // write a dummy byte
	// tSR4Loop4
	while (!(SPSR & (1 << SPIF))) {} // wait to read GSL byte
	ui8GSL = SPDR; // read GSL byte
	SPDR = 0; // write a dummy byte
	// tSR4Loop5
	while (!(SPSR & (1 << SPIF))) {} // wait to read the first byte
	ui8ArrR6[0] = SPDR; // read first byte
	SPDR = 0; // write a dummy byte
	// tSR4Loop6
	while (!(SPSR & (1 << SPIF))) {} // wait to read the second byte
	ui8ArrR6[1] = SPDR; // read first byte
	SPDR = 0; // write a dummy byte
	// tSR4Loop7
	while (!(SPSR & (1 << SPIF))) {} // wait to read the third byte
	ui8ArrR6[2] = SPDR; // read third byte
	SPDR = 0; // write a dummy byte
	// tSR4Loop8
	while (!(SPSR & (1 << SPIF))) {} // wait to read the fourth byte
	ui8ArrR6[3] = SPDR; // read the fourth byte

	PORTB = PORTB | (1 << 2); // bring nCS line up
}

void SpiW(uint16_t ui16Len) // writes ui16SpiWLen bytes through the SPI interface
{
	uint16_t ui16TempA = 0; // a temporary variable

	if (ui16Len & 1) // if length is odd
	{
		ui16Len = ui16Len + 1; // if length is odd, make it even by adding a dummy byte
	}

	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);
	PORTB = PORTB & ~(1 << 2);

	SPDR = ui8ArRxTx[ui16TempA];
	ui16TempA = ui16TempA + 1;
	_delay_loop_1(2);

	while (1)
	{
		if (ui16Len == ui16TempA) {	break;}
		_delay_loop_1(2);
//		while (!(SPSR & (1 << SPIF))) {} // wait byte to go out */
//		ui8A = SPDR;
		SPDR = ui8ArRxTx[ui16TempA];
		ui16TempA = ui16TempA + 1;
		_delay_loop_1(1);
		_NOP();
		_NOP();
	}
	_delay_loop_1(3);
//	_NOP();
//	while (!(SPSR & (1 << SPIF))) {} // wait byte to go out
//	ui8A = SPDR;
	
	PORTB = PORTB | (1 << 2); // bring nCS line up}
}

void SpiReadSDT(uint8_t ui8Sk, uint16_t ui16Head) // tries to read (ui16GbSize - ui8Head) bytes from the socket
{
	uint8_t ui8A; // a temporary variable
	uint16_t ui16TempLen;
	uint16_t ui16TempLenLess1;
	
	// tell socket I want to read ui16GbLen = ui16GbSize - tblSk[ui8Sk].ui8RxBfLen bytes
	// socket say that there are ui16TempLen bytes available for read
	// make ui16GbLen = MIN(ui16GbLen, ui16TempLen). If this is even, ui8A = 0, else ui8A = 1
	// make ui16GbLen = ui16GbLen + tblSk[ui8Sk].ui8RxBfLen // this is the final length of ui8ArRxTx
	// make ui16TempLen = tblSk[ui8Sk].ui8RxBfLen // this the head - place here data read from SPI interface
	// read until ui16TempLen = ui16GbLen
	// read one more byte if ui8A > 0 and discard it
	
	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);

	PORTB = PORTB & ~(1 << 2); // bring nCS line down
	
	uint16_t ui16GbLen = ui16GenBufSize - ui16Head;
	
	SPSR;
	SPDR;
	SPDR = (ui8SkDT << 2) | bvRSR; // write register address, r/w, general/socket
	// SpiReadSdtLoop1
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = ui8Sk; // socket number

	// SpiReadSdtLoop2
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	SPDR; // read byte and discard
	SPDR = ui16GbLen >> 8; // write MSB of data length wanted to be read

	// SpiReadSdtLoop3
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	ui8GSH = SPDR; // read GSH byte
	SPDR = ui16GbLen & 0xFF; // write LSB of data length wanted to be read

	// SpiReadSdtLoop4
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	ui8GSL = SPDR; // read GSL byte
	SPDR = 0; // write a dummy byte

	// SpiReadSdtLoop5
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	ui16TempLen = SPDR << 8; // read MSB of data length available
	SPDR = 0; // write a dummy byte

	// SpiReadSdtLoop6
	while (!(SPSR & (1 << SPIF))) {} // check if SPI received a dummy byte
	ui16TempLen = ui16TempLen + SPDR; // read LSB of data length available

	// make ui16GbLen = MIN(ui16GbLen, ui16TempLen)
	if (ui16TempLen < ui16GbLen) { ui16GbLen = ui16TempLen; } // I am going to read ui16GbLen from the SPI interface

	if (ui16GbLen & 1)
	{
		ui8A = 1; // read one more byte which is a dummy byte, do not copy it in the buffer
	}
	else { ui8A = 0;}
		
	ui16GbLen = ui16GbLen + ui16Head; // this is the final length of ui8ArRxTx
	ui16TempLen = ui16Head; // this is the head - place here data read from SPI interface

	// read until ui16TempLen = ui16GbLen
	if (ui16TempLen < ui16GbLen)
	{
		SPDR = 0; // write a dummy byte

		ui16TempLenLess1 = ui16GbLen - 1; // it helps the next while()
		
		while ((ui16TempLen) < ui16TempLenLess1)
		{
			while (!(SPSR & (1 << SPIF))) {}
			ui8ArRxTx[ui16TempLen] = SPDR;
			SPDR = 0; // write a dummy byte
			ui16TempLen++;
		}
		while (!(SPSR & (1 << SPIF))) {}
		ui8ArRxTx[ui16TempLen] = SPDR;

		if (ui8A)
		{
			SPDR = 0; // write a dummy byte
			while (!(SPSR & (1 << SPIF))) {}
			SPDR; // discard the last byte			
		}
	}

	PORTB = PORTB | (1 << 2); // bring nCS line up}
}
