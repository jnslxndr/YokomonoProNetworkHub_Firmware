/**
 * Setup PORTC (AnalogIn)
 */
void setupGroupSwitch(){
 DDRC  |= B00001111;
 PORTC |= B00001111; // Pull up A0 to A3 to have reliable values
}

/**
 * Read group number
 */
int group_num() {
  return (int)(~PINC&B00001111)%8; // mask the first 4 ports
}

int is_sender() {
  return group_num()>5;
}


void setupOSC(){
  unsigned int _port = (is_sender())?SERVER_PORT:CLIENT_PORT;
  osc.begin(_port);
  osc.flush();
}

void prepare_send(){
    sendMes.setIp( BROADCAST_IP );
    sendMes.setPort( CLIENT_PORT );
    sendMes.setTopAddress(MAIN_ADDR);
    sendMes.setSubAddress(SUB_ADDR[group_num()]);
}


/**
 * Try to find an MAC Address, that's is not alread taken
 */
void safe_start_ethernet(){
  randomSeed(analogRead(0));
  byte mac[6] = { 
    0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x00   };
  int conn_status, mac_addr;
  conn_status = mac_addr = 0;
  do {
    mac[5] = (byte) mac_addr;
    conn_status = Ethernet.begin(mac);
    if (conn_status) { 
      Serial.println("SUCCESSFULLY configured Ethernet using DHCP"); 
    }
    else {
      //mac_addr=++mac_addr&0xFF; // loop though the bytes to find a right one
      mac_addr = random(255);
      Serial.println("FAILED to configure Ethernet using DHCP! Retrying ...");
    }
    // delay(50); // is a delay needed?
  } 
  while (conn_status == 0);
  
  // DEBUG //
  printIpToSerial();
}

