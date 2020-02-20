#define fpDhcpConfig 0
#define fpDhcpCkStatus 1

void DhcpConfig(void);
void DhcpCkStatus(void);

// GC
const uint8_t SeiGC_bpSWRST = 15; // Software Reset

const uint16_t ui16SeiSMR_SWRST = 0xAAAD; // Reset

// DHCP
const uint8_t bpDHEN = 7;
const uint8_t bpDHIE = 0;

const uint8_t bmDHS = 7;

const uint8_t bvWCR = 1;
const uint8_t bvRCR = 3;

const uint8_t ui8GC = 0; // General Control register
const uint8_t ui8HHG = 2; // Host IP Address, Host Subnet Mask and Gateway IP Address registers
const uint8_t ui8DHCS = 12; // DHCP Control and Status register
const uint8_t ui8TRISB = 20; // TRISB
const uint8_t ui8TRISE = 23; // TRISE

const uint8_t bvDISABLED = 0;
const uint8_t bvSERVER_ERROR = 6;

// unsigned 8 bit variables
uint8_t ui8Cnt5ms;
uint8_t ui8GpFlag1s;
uint8_t ui8GSH = 0; // GeneralStatus register high byte
uint8_t ui8GSL = 0; // GeneralStatus register low byte */
uint8_t ui8TaskDhcp = 0;

// unsigned 16 bit variables
uint16_t ui8Cnt10s;

// arrays
uint8_t ui8ArrR6[6]; // array, read a 6 bytes register like mac address
uint8_t ui8ArTx[6];
uint8_t ui8Digit[10];

const uint8_t strText[] PROGMEM = "My IP address: ";

typedef void (* const p2f)(void);
const p2f pArrDhcp[] = {
  DhcpConfig,
  DhcpCkStatus
};
