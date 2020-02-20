
void config() {
	// config PORTB

	// portb
	// -----

	// PortB.0   - NC - input with pull-up resistor enabled
	// PortB.1   - NC - input with pull-up resistor enabled
	// PortB.2   - nCS Ethernet; set output high
	// PortB.3   - MOSI; set output high
	// PortB.4   - MISO; set as input
	// PortB.5   - SCK; set as output high
	// PortB.6   - SDA - input with pull-up resistor enabled
	// PortB.7   - SCL - input with pull-up resistor enabled

	DDRB = (1 << 5 | 1 << 3 | 1 << 2);
	PORTB = (1 << 5 | 1 << 3 | 1 << 2);
	PINB = (1 << 7 | 1 << 6 | 1 << 4 | 1 << 1 | 1 << 0);

	// portc - enable pull-ups on all pins
	// -----------------------------------
	DDRC = 0;
	PORTC = 0;
	PINC = 255;

  // portd - enable pull-ups on all pins
  // -----------------------------------
  DDRD = 0;
  PORTD = 0;
  PIND = 255;

	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);
}
