void SpiR2(uint8_t ui8b0, uint8_t ui8b1) // read SCS(virtual address, G/Sk, socket number)
{
	PORTB = PORTB & ~(1 << 2);

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

	SPDR = ui8ArTx[ui16TempA];
	ui16TempA = ui16TempA + 1;
	_delay_loop_1(2);

	while (1)
	{
		if (ui16Len == ui16TempA) {	break;}
		_delay_loop_1(2);
//		while (!(SPSR & (1 << SPIF))) {} // wait byte to go out */
//		ui8A = SPDR;
		SPDR = ui8ArTx[ui16TempA];
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
