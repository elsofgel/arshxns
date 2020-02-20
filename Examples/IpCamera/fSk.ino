
void DisSk(void) { // 0
UDR0 = 0;

	ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpSEN); // disable socket. It also clears the status field to 0
	SpiW(4);

	tblSk[ui8SkRun].ui8Task = fpCkSkIsDis;
	tblSk[ui8SkRun].ui16Cnt5ms = 0;
//	ui16SkIntFlags = ui16SkIntFlags & ~(1 << ui8SkRun);
}

void CkSkIsDis() { // 1
//UDR0 = 1;
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun);
	if ((ui8ArrR6[1] & (1 << bpSEN)) == 0) { tblSk[ui8SkRun].ui8Task = fpEnSk; }
	else if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
		tblSk[ui8SkRun].ui8Flag5ms = 0;
		tblSk[ui8SkRun].ui16Cnt5ms++;
		if (tblSk[ui8SkRun].ui16Cnt5ms > 10) { tblSk[ui8SkRun].ui8Task = fpCkSkIsDis;	}
	}
}

void EnSk() { // 2; write SkIPA, SkSNM, SkP, SkCS
//UDR0 = 2;
	// write SkIPA
	ui8ArRxTx[0] = (ui8SkIPA << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = ui8ArrHostIP[0]; ui8ArRxTx[3] = ui8ArrHostIP[1]; ui8ArRxTx[4] = ui8ArrHostIP[2]; ui8ArRxTx[5] = ui8ArrHostIP[3];
	SpiW(6);
	
	ui8ArRxTx[0] = (ui8SkSM << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = ui8ArrHostSM[0]; ui8ArRxTx[3] = ui8ArrHostSM[1]; ui8ArRxTx[4] = ui8ArrHostSM[2]; ui8ArRxTx[5] = ui8ArrHostSM[3];
	SpiW(6);

	ui8ArRxTx[0] = (ui8SkP << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = ui16Port & 0xFF; ui8ArRxTx[3] = ui16Port >> 8;
	SpiW(4);

	ui8ArRxTx[0] = (ui8SkCSSET << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpSEN | 1 << bpTOC | MODE_TCPS);
	SpiW(4); // Enable socket in TCP server mode and put it in listening mode
	
	tblSk[ui8SkRun].ui16Cnt5ms = 0;
	tblSk[ui8SkRun].ui8Task = fpCkListening;	
}

void SetListening() { // 3
//UDR0 = 3;
	ui8ArRxTx[0] = (ui8SkCSSET << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
	ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpSEN | 1 << bpTOC | MODE_TCPS);
	SpiW(4); // Enable socket in TCP server mode and put it in listening mode
	
	tblSk[ui8SkRun].ui16Cnt5ms = 0;
	tblSk[ui8SkRun].ui8Task = fpCkListening;
}

void CkListening() { // 4
//UDR0 = 4;
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun);
	
	if ((ui8ArrR6[1] & (1 << bpSEN | MODE_TCPS)) == (1 << bpSEN | MODE_TCPS)) {
		if ((ui8ArrR6[0] & bmSS) == ssLISTENING) { tblSk[ui8SkRun].ui8Task = fpListening; }
		else if ((ui8ArrR6[0] & bmSS) == ssESTABLISHED) {
			tblSk[ui8SkRun].ui8Task = fpEstablished;
			tblSk[ui8SkRun].ui16Cnt5ms = 0;
		}
		else if (((ui8ArrR6[0] & bmSS) > ssESTABLISHED) | ((ui8ArrR6[0] & bmSS) == ssCLOSED)) {
			if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
				tblSk[ui8SkRun].ui8Flag5ms = 0;
				tblSk[ui8SkRun].ui16Cnt5ms++;
				if (tblSk[ui8SkRun].ui16Cnt5ms > 10) { tblSk[ui8SkRun].ui8Task = fpDisSk;	}
			}
		 }
	}
	else if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
		tblSk[ui8SkRun].ui8Flag5ms = 0;
		tblSk[ui8SkRun].ui16Cnt5ms++;
		if (tblSk[ui8SkRun].ui16Cnt5ms > 10) { tblSk[ui8SkRun].ui8Task = fpDisSk;	}
	}
}

void Listening() { // 5
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun);
	
	if ((ui8ArrR6[0] & bmSS) == ssLISTENING) { tblSk[ui8SkRun].ui8If = 0; } // clear local interrupt flag so this socket will be serviced at 5ms unless a status change is reported via the interrupt system
	else if ((ui8ArrR6[0] & bmSS) == ssESTABLISHED) {
		tblSk[ui8SkRun].ui8Task = fpEstablished;
		tblSk[ui8SkRun].ui16Cnt5ms = 0;
//UDR0 = 5;
	}
	else if (((ui8ArrR6[0] & bmSS) > ssESTABLISHED) | ((ui8ArrR6[0] & bmSS) == ssCLOSED)) {
		tblSk[ui8SkRun].ui8Task = fpClosing;
		tblSk[ui8SkRun].ui16Cnt5ms = 0;
		ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
		ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
		SpiW(4); // clear bit TOC in SkCS register
//UDR0 = 6;
	}
}
void Established() { // 6
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun);

	if ((ui8ArrR6[0] & bmSS) == ssESTABLISHED) {
//UDR0 = 6;
		if ( ui8ArrR6[0] & (1 << bpDRB) ) { // if there is any data received, read and look for GET
			tblSk[ui8SkRun].ui16Cnt5ms = 0; // reset timeout timer
			tblSk[ui8SkRun].ui16RxBfHead = 0; // read data and place it to the beginning of the receiving buffer
			SpiReadSDT(ui8SkRun, tblSk[ui8SkRun].ui16RxBfHead);

			if ((ui8ArRxTx[0] == 'G') & (ui8ArRxTx[1] == 'E') & (ui8ArRxTx[2] == 'T') & (ui8ArRxTx[3] == ' ') & (ui8ArRxTx[4] == '/') & (ui8ArRxTx[5] == ' ') & (ui8ArRxTx[6] == 'H') & (ui8ArRxTx[7] == 'T') & (ui8ArRxTx[8] == 'T') & (ui8ArRxTx[9] == 'P') & (ui8ArRxTx[10] == '/') & (ui8ArRxTx[11] == '1') & (ui8ArRxTx[12] == '.') & (ui8ArRxTx[13] == '1')) {
				// HTTP1/1
				ui16ArRxTxHead = 4;
				memcpy_P((ui8ArRxTx + ui16ArRxTxHead), strHttp1, (sizeof(strHttp1) - 1));
				ui16ArRxTxHead = ui16ArRxTxHead + sizeof(strHttp1) - 1;
				
				
				uint16_t ui16GbLen = sizeof(strPage) - 1;
				itoa(ui16GbLen, strHttpLength, 10);
				memcpy((ui8ArRxTx + ui16ArRxTxHead), strHttpLength, strlen(strHttpLength));
				ui16ArRxTxHead = ui16ArRxTxHead + strlen(strHttpLength);
				
				memcpy_P((ui8ArRxTx + ui16ArRxTxHead), strHttp2Page, (sizeof(strHttp2Page) - 1));
				ui16ArRxTxHead = ui16ArRxTxHead + sizeof(strHttp2Page) - 1;
				
				memcpy_P((ui8ArRxTx + ui16ArRxTxHead), strPage, (sizeof(strPage) - 1));
				ui16ArRxTxHead = ui16ArRxTxHead + sizeof(strPage) - 1;

				ui8ArRxTx[0] = ui8SkDT << 2;
				ui8ArRxTx[1] = ui8SkRun;
				ui8ArRxTx[2] = ((ui16ArRxTxHead - 4) >> 8);
				ui8ArRxTx[3] = (ui16ArRxTxHead - 4) & 0xFF;
				SpiW(ui16ArRxTxHead); // writes bytes through the SPI interface
			}
			else if ((ui8ArRxTx[0] == 'G') & (ui8ArRxTx[1] == 'E') & (ui8ArRxTx[2] == 'T') & (ui8ArRxTx[3] == ' ') & (ui8ArRxTx[4] == '/') & (ui8ArRxTx[5] == 'f') & (ui8ArRxTx[6] == 'a') & (ui8ArRxTx[7] == 'v') & (ui8ArRxTx[8] == 'i') & (ui8ArRxTx[9] == 'c') & (ui8ArRxTx[10] == 'o') & (ui8ArRxTx[11] == 'n')) {
				// no favicon
				tblSk[ui8SkRun].ui8If = 0;
				tblSk[ui8SkRun].ui16Cnt5ms = 0;
				tblSk[ui8SkRun].ui8Task = fpClosing;
				ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
				ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
				SpiW(4); // clear bit TOC in SkCS register
			}
			else if ((ui8ArRxTx[0] == 'G') & (ui8ArRxTx[1] == 'E') & (ui8ArRxTx[2] == 'T') & (ui8ArRxTx[3] == ' ') & (ui8ArRxTx[4] == '/') & (ui8ArRxTx[5] == '1') & (ui8ArRxTx[6] == '.') & (ui8ArRxTx[7] == 'j') & (ui8ArRxTx[8] == 'p') & (ui8ArRxTx[9] == 'g')) {
				// picture 1.jpg

        SpiR4((ui8SkMTU << 2) | bvRSR, ui8SkRun); // MTU and PMSS
        tblSk[ui8SkRun].ui16PMSS = ui8ArrR6[2] * 256 + ui8ArrR6[3];
        if (tblSk[ui8SkRun].ui16PMSS > 1460) { tblSk[ui8SkRun].ui16PMSS = 1200; }
        if (tblSk[ui8SkRun].ui16PMSS < 536) { tblSk[ui8SkRun].ui16PMSS = 536; }

				tblSk[ui8SkRun].ui8Flag5ms = 0;
        tblSk[ui8SkRun].ui8Task = fpSendPicHeader;
			} 
		}
		else { // no data, just stay idle for a while and if peer does not close the connection, do so
			if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
				tblSk[ui8SkRun].ui8Flag5ms = 0;
				tblSk[ui8SkRun].ui16Cnt5ms++;
			}

			tblSk[ui8SkRun].ui8If = 0; // once data has been processed, clear local int flag to slow down servicing this socket

			if (tblSk[ui8SkRun].ui16Cnt5ms > 250) { // after a while in established state and no data exchanged, close the connection
				tblSk[ui8SkRun].ui16Cnt5ms = 0;
				tblSk[ui8SkRun].ui8Task = fpClosing;
				ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
				ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
				SpiW(4); // clear bit TOC in SkCS register
			}
		}
	}
	else if ((ui8ArrR6[0] & bmSS) > ssESTABLISHED) {
//UDR0 = 0;
//UDR0 = tblSk[ui8SkRun].ui16Cnt5ms;
		tblSk[ui8SkRun].ui16Cnt5ms = 0;
		tblSk[ui8SkRun].ui8Task = fpClosing;
		ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
		ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
		SpiW(4); // clear bit TOC in SkCS register
	}
}

void SendPicHeader() { // 8
 SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun); // read socket status and proceed only if it is still in established mode
  if (((ui8ArrR6[0] & bmSS) == ssESTABLISHED) & (tblSk[ui8SkRun].ui16Cnt5ms < 2000)) {
    
    if (ui8CamTaken == 0) {
      ui8CamTaken = 1;
      tblSk[ui8SkRun].ui16Cnt5ms = 0;
        
      ui16ArRxTxHead = 4;
      memcpy_P((ui8ArRxTx + ui16ArRxTxHead), strPicHeader, (sizeof(strPicHeader) - 1));
      ui16ArRxTxHead = ui16ArRxTxHead + sizeof(strPicHeader) - 1;

      ui8ArRxTx[0] = ui8SkDT << 2;
      ui8ArRxTx[1] = ui8SkRun;
      ui8ArRxTx[2] = ((ui16ArRxTxHead - 4) >> 8); //   | (1 << bpDataTcpHold)
      ui8ArRxTx[3] = (ui16ArRxTxHead - 4) & 0xFF;
      SpiW(ui16ArRxTxHead); // writes bytes through the SPI interface

      tblSk[ui8SkRun].ui32PicIdx = 0; // zero bytes has been sent so far
      tblSk[ui8SkRun].ui8Task = fpTakePic;
    }
    else {
      if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
        tblSk[ui8SkRun].ui8Flag5ms = 0;
        tblSk[ui8SkRun].ui16Cnt5ms++;
      }
    }
  }
  else {
    ui8CamTaken = 0;
    tblSk[ui8SkRun].ui16Cnt5ms = 0;
    tblSk[ui8SkRun].ui8Task = fpClosing;
    ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
    ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
    SpiW(4); // clear bit TOC in SkCS register
  }
}


void TakePic() { // 7
  //UDR0 = 7;
  SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun); // read socket status and proceed only if it is still in established mode
  if ((ui8ArrR6[0] & bmSS) == ssESTABLISHED) {
    
    tblSk[ui8SkRun].ui16Cnt5ms = 0;
    tblSk[ui8SkRun].ui8Task = fpSendBoundary;
        
    // trigger one frame
    SpiArCamW1(0x84, 0x01);
    SpiArCamW1(0x84, 0x01);
    SpiArCamW1(0x84, 0x01);
    SpiArCamW1(0x84, 0x02);
  }
  else {
    ui8CamTaken = 0;
    tblSk[ui8SkRun].ui16Cnt5ms = 0;
    tblSk[ui8SkRun].ui8Task = fpClosing;
    ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
    ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
    SpiW(4); // clear bit TOC in SkCS register
  }
}


void SendBoundary() { // 8
//UDR0 = 7;
  SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun); // read socket status and proceed only if it is still in established mode
  if (((ui8ArrR6[0] & bmSS) == ssESTABLISHED) & (tblSk[ui8SkRun].ui16Cnt5ms < 2000)) { // and not more than 10 seconds
    // increment timer
    if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
      tblSk[ui8SkRun].ui8Flag5ms = 0;
      tblSk[ui8SkRun].ui16Cnt5ms++;
    }
    
    if ((SpiArCamR1(0x41) & (1 << 3)) == (1 << 3)) {

      // read image length
      ui32ImageLen = (uint32_t)SpiArCamR1(0x42) + ((uint32_t)SpiArCamR1(0x43) << 8) + ((uint32_t)SpiArCamR1(0x44) << 16);
      if (ui32ImageLen > 400000) { ui32ImageLen = 10; };        
      ui16ArRxTxHead = 4;
      memcpy_P((ui8ArRxTx + ui16ArRxTxHead), strBoundary, (sizeof(strBoundary) - 1));
      ui16ArRxTxHead = ui16ArRxTxHead + sizeof(strBoundary) - 1;
        
      ui8ArRxTx[0] = ui8SkDT << 2;
      ui8ArRxTx[1] = ui8SkRun;
      ui8ArRxTx[2] = ((ui16ArRxTxHead - 4) >> 8); //   | (1 << bpDataTcpHold)
      ui8ArRxTx[3] = (ui16ArRxTxHead - 4) & 0xFF;
      SpiW(ui16ArRxTxHead); // writes bytes through the SPI interface

      tblSk[ui8SkRun].ui32PicIdx = 0; // zero bytes has been sent so far
      tblSk[ui8SkRun].ui8Task = fpSendPic;
    }
  }
  else if ((ui8ArrR6[0] & bmSS) > ssESTABLISHED) {
    tblSk[ui8SkRun].ui16Cnt5ms = 0;
    tblSk[ui8SkRun].ui8Task = fpClosing;
    ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
    ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
    SpiW(4); // clear bit TOC in SkCS register
  }
}

void SendPic() { // 9
	//UDR0 = 8;
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun); // read socket status and proceed only if it is still in established mode
	if ((ui8ArrR6[0] & bmSS) == ssESTABLISHED) {
		
		SpiR2((ui8SkTFS << 2) | bvRSR, ui8SkRun); // read register  in order to find out how much free space is in the transmit buffer. Because cannot be more than 65535, read only two bytes
		ui16Temp = (ui8ArrR6[0] << 8) + ui8ArrR6[1]; // free space in the transmit buffer
		uint32_t ui32Temp = ui32ImageLen - tblSk[ui8SkRun].ui32PicIdx; // data to be sent out
		if (ui32Temp > tblSk[ui8SkRun].ui16PMSS) { ui16Temp2 = tblSk[ui8SkRun].ui16PMSS; } else { ui16Temp2 = (ui32Temp & 0xFFFF); }
		while ((ui16Temp >= ui16Temp2) & (ui16Temp2 > 0)) {
			SpiArCamRn(ui16Temp2); // this copies ui16Temp2 bytes from camera to ui8ArRxTx starting with position 4. There is no way to read from camera starting at a custom position

			ui8ArRxTx[0] = ui8SkDT << 2;
			ui8ArRxTx[1] = ui8SkRun;
			ui8ArRxTx[2] = (ui16Temp2) >> 8; // here the hold flag is not set, so data will go out right away
			ui8ArRxTx[3] = (ui16Temp2) & 0xFF;
			SpiW(ui16Temp2 + 4); // writes bytes through the SPI interface

			tblSk[ui8SkRun].ui32PicIdx = tblSk[ui8SkRun].ui32PicIdx + (uint32_t)ui16Temp2;
			ui32Temp = ui32ImageLen - tblSk[ui8SkRun].ui32PicIdx;
			SpiR2((ui8SkTFS << 2) | bvRSR, ui8SkRun); // read register  in order to find out how much free space is in the transmit buffer. Because cannot be more than 65535, read only two bytes
			ui16Temp = (ui8ArrR6[0] << 8) + ui8ArrR6[1]; // free space in the transmit buffer
			if (ui32Temp > tblSk[ui8SkRun].ui16PMSS) { ui16Temp2 = tblSk[ui8SkRun].ui16PMSS; } else { ui16Temp2 = (ui32Temp & 0xFFFF); }
		}
		if (ui16Temp2 == 0) {
//			ui8CamTaken = 0;
			tblSk[ui8SkRun].ui16Cnt5ms = 0;
			tblSk[ui8SkRun].ui8Task = fpTakePic;
		}
	}
	else if ((ui8ArrR6[0] & bmSS) > ssESTABLISHED) {
		ui8CamTaken = 0;
		tblSk[ui8SkRun].ui16Cnt5ms = 0;
		tblSk[ui8SkRun].ui8Task = fpClosing;
		ui8ArRxTx[0] = (ui8SkCSCLR << 2) | bvWSR; ui8ArRxTx[1] = ui8SkRun;
		ui8ArRxTx[2] = 0; ui8ArRxTx[3] = (1 << bpTOC);
		SpiW(4); // clear bit TOC in SkCS register
	}
}

void Closing() { // 10
//UDR0 = 9;
	SpiR2((ui8SkCS << 2) | bvRSR, ui8SkRun);
	if (tblSk[ui8SkRun].ui8Flag5ms > 0) {
		tblSk[ui8SkRun].ui8Flag5ms = 0;
		tblSk[ui8SkRun].ui16Cnt5ms++;
	}
	if (((ui8ArrR6[0] & bmSS) >= ssTIME_WAIT) | ((ui8ArrR6[0] & bmSS) == ssCLOSED)) { tblSk[ui8SkRun].ui8Task = fpSetListening; }	
	if (tblSk[ui8SkRun].ui16Cnt5ms > 2000) { tblSk[ui8SkRun].ui8Task = fpDisSk; }
}
