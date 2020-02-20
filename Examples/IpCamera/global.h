#define nSockets 2
#define ui16GenBufSize 1500 // a general purpose buffer size
#define nCsSei 2
#define nCsCam 7

#define OV2640_idle 0
#define OV2640_JPEG_160x120 1
#define OV2640_JPEG_320x240 2
#define OV2640_JPEG_640x480 3
#define OV2640_JPEG_800x600 4
#define OV2640_JPEG_1024x768 5
#define OV2640_JPEG_x1280x1024 6
#define OV2640_JPEG_1600x1200 7

#define fpDisSk 0
#define fpCkSkIsDis 1
#define fpEnSk 2
#define fpSetListening 3
#define fpCkListening 4
#define fpListening 5
#define fpEstablished 6
#define fpSendPicHeader 7
#define fpTakePic 8
#define fpSendBoundary 9
#define fpSendPic 10
#define fpClosing 11

void DisSk(void);
void CkSkIsDis(void);
void EnSk(void);
void SetListening(void);
void CkListening(void);
void Listening(void);
void Established(void);
void SendPicHeader(void);
void TakePic(void);
void SendBoundary(void);
void SendPic(void);
void Closing(void);

const uint8_t bpEthPinInt = 2;

// GC
const uint8_t SeiGC_bpSWRST = 15; // Software Reset
const uint8_t SeiGC_bpSPIM = 14; // SPI Mode
const uint8_t SeiGC_bpIPINE = 4; // Interrupt Pin Enable
const uint8_t SeiGC_bpMIEN = 0; // Monitor Interface Enable

// SMR SPI Mode and Reset register
const uint16_t ui16SeiSMR_SWRST = 0xAAAD; // Reset
const uint16_t ui16SeiSMR_SPIM0 = 0xAAA8; // SPI Mode 0
const uint16_t ui16SeiSMR_SPIM1 = 0xAAA9; // SPI Mode 1
const uint16_t ui16SeiSMR_SPIM2 = 0xAAAA; // SPI Mode 2
const uint16_t ui16SeiSMR_SPIM3 = 0xAAAB; // SPI Mode 3

// PHYC
const uint8_t bpLSTCIE = 6; // Link Status Changed Interrupt Enable
const uint8_t bpAN = 4; // Auto Negotiation


// SCS bit positions
const uint8_t bpIPDF = 7;
const uint8_t bpKA = 6;
const uint8_t bpTBE = 5;
const uint8_t bpDRB = 4;
const uint8_t bmSS = 15;
const uint8_t bpSEN = 7;
const uint8_t bpCFR = 5;
const uint8_t bpTOC = 4;

const uint8_t MODE_UDPC = 0; // UDP Client
const uint8_t MODE_UDPS = 1; // UDP Server
const uint8_t MODE_TCPC = 2; // TCP Client
const uint8_t MODE_TCPS = 3; // TCP Server

// SIETTL
const uint8_t bpSCIE = 1;
const uint8_t bpRPIE = 0;

// unsigned 8 bit constants
const uint8_t bvWSR = 0;
const uint8_t bvWCR = 1;
const uint8_t bvRSR = 2;
const uint8_t bvRCR = 3;

const uint8_t ui8GC = 0; // General Control register
const uint8_t ui8HHG = 2; // Host IP Address, Host Subnet Mask and Gateway IP Address registers
const uint8_t ui8IF = 4; // Interrupt Flags register
const uint8_t ui8ArpCtrl = 15; // ARP Control register
const uint8_t ui8ATTL = 16; // ARP Time-To-Live
const uint8_t ui8TRISB = 20; // TRISB
const uint8_t ui8LATB = 21; // LATB
const uint8_t ui8TRISE = 23; // TRISE
const uint8_t ui8LATE = 24; // LATE
const uint8_t ui8ODCE = 25; // ODCE

const uint8_t ui8SkCS = 0; // Socket Control and Status
const uint8_t ui8SkCSCLR = 0; // Socket Control and Status CLEAR
const uint8_t ui8SkCSSET = 1; // Socket Control and Status SET
const uint8_t ui8SkIPA = 3; // Socket IP Address
const uint8_t ui8SkSM = 4; // Socket Subnet Mask
const uint8_t ui8SkPIPA = 5; // Socket Peer IP Address
const uint8_t ui8SkP = 6; // Socket Port number
const uint8_t ui8SkPP = 7; // Socket Peer Port number
const uint8_t ui8SkTTL = 8; // Socket TimeToLive
const uint8_t ui8SkBSPS = 9; // Socket Buffer Start Pointer and Size
const uint8_t ui8SkRDS = 10; // Socket Received Data Size
const uint8_t ui8SkTFS = 11; // Socket Transmit [buffer] Free Space
const uint8_t ui8SkMTU = 12; // Socket Maximum Transmission Unit and PMSS
const uint8_t ui8SkDU = 13; // Socket Data UDP
const uint8_t ui8SkDT = 14; // Socket Data TCP
const uint8_t ui8SkTCPT = 15; // Socket TCP Tunning
const uint8_t ui8SkCW_STHR = 16; // Socket Congestion Window and Slow Start Threshold


const uint8_t ssCLOSED = 0;
const uint8_t ssLISTENING = 1;
const uint8_t ssSEND_SYN = 2;
const uint8_t ssSYN_RECEIVED = 3;
const uint8_t ssESTABLISHED = 4;
const uint8_t ssFIN_WAIT_Tx = 5;
const uint8_t ssFIN_WAIT_1 = 6;
const uint8_t ssFIN_WAIT_2 = 7;
const uint8_t ssCLOSING = 8;
const uint8_t ssCLOSE_WAIT = 9;
const uint8_t ssCLOSE_WAIT_TX = 10;
const uint8_t ssLAST_ACK = 11;
const uint8_t ssTIME_WAIT = 12;
const uint8_t ssRESET_BY_PEER = 13;
const uint8_t ssTIMEOUT = 13;
const uint8_t ssCONFIG_ERR = 15;

const uint8_t bpDataTcpHold = 6;

// unsigned 8 bit variables
uint8_t ui8A;
uint8_t ui8GpFlag5ms;
uint8_t ui8Cnt5ms;
uint8_t ui8CntSecs;
uint8_t ui8For;
uint8_t ui8GSH = 0; // GeneralStatus register high byte
uint8_t ui8GSL = 0; // GeneralStatus register low byte */
uint8_t ui8PHYC = 0; // PHY Control (a local mirror of the one on the Ethernet side)
uint8_t ui8TaskEthSwitch = 0;
uint8_t ui8SkRun = 0;
uint8_t ui8CamTaken = 0;
uint8_t ui8ImgRes = OV2640_JPEG_320x240; // image resolution

// unsigned 16 bit variables
uint16_t ui16SkIntFlags;
uint16_t ui16IntFlags;
uint16_t ui16ArRxTxHead; // head pointer for ui16ArRxTx
uint16_t ui16Temp;
uint16_t ui16Temp2;
uint16_t ui16TcRedir = 0;
uint16_t ui16SeiStatus;
uint16_t ui16CmIntFlags;

uint32_t ui32ImageLen;

char strHttpLength[10];
// custom structures
typedef struct {
  uint16_t ui16RxBfHead;
  uint16_t ui16PMSS;
  uint16_t ui16Cnt5ms;
  uint32_t ui32PicIdx;
  uint8_t ui8Task;
  uint8_t ui8Flag5ms;
  uint8_t ui8If;
} recSk;

// arrays
uint8_t ui8ArrSpi[2]; // array, first two bytes of any Spi transaction
uint8_t ui8ArrR6[6]; // array, read a 6 bytes register like mac address
uint8_t ui8ArRxTx[ui16GenBufSize];

typedef void (* const p2f)(void);
const p2f pArrfSk[] = {
  DisSk,
  CkSkIsDis,
  EnSk,
  SetListening,
  CkListening,
  Listening,
  Established,
  SendPicHeader,
  TakePic,
  SendBoundary,
  SendPic,
  Closing
};

// tables (arrays of structures)
recSk tblSk[1]; // 16 // 4
