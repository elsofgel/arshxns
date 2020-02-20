
void DhcpConfig() { // 0
//UDR0 = 0;
  // enable DHCP
  ui8ArTx[0] = (ui8DHCS << 2) | bvWCR;
  ui8ArTx[2] = (1 << bpDHEN | 1 << bpDHIE);
  SpiW(4);

  ui8TaskDhcp = fpDhcpCkStatus;
}

void DhcpCkStatus() { // 1
//UDR0 = 1;
  // check DHCP
  SpiR2((ui8DHCS << 2) | bvRCR, 0);

  if (((ui8ArrR6[1] & bmDHS) >= bvSERVER_ERROR) | ((ui8ArrR6[1] & bmDHS) == bvDISABLED)) {
    // disable DHCP
    ui8ArTx[0] = (ui8DHCS << 2) | bvWCR;
    ui8ArTx[2] = 0;
    SpiW(4);

    ui8TaskDhcp = fpDhcpConfig;
  }
}
