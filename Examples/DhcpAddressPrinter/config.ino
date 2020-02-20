
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

	// portd
	// -----

	// PortD.0   - USART Rx - leave it as input
	// PortD.1   - USART Tx - set as output
	// PortD.2   - input interrupt from SEI
	// PortD.3   - NC
	// PortD.4   - nCS for microSD - set output high
	// PortD.5   - NC
	// PortD.6   - NC
	// PortD.7   - CS for arducam - set as output high

	DDRD = (1 << 7 | 1 << 4 | 1 << 1);
	PORTD = (1 << 7 | 1 << 4 | 1 << 1);

		
	// config TC1 - 5ms

	TCCR1A = 0;
	TCCR1B = (1 << WGM12 | 1 << CS11); //div by 8
	OCR1A = 10000;


	// config SPI for SEI
	SPCR = (1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA);
	SPSR = (1 << SPI2X);


	// config USART0 - PC; baud rate = 9600 bps
	UCSR0A = (1 << U2X0);
	UCSR0B = (1 << RXCIE0 | 1 << RXEN0 | 1 << TXEN0);
	UCSR0C = (1 << UCSZ01 | 1 << UCSZ00);
	UBRR0 = 207;
}
