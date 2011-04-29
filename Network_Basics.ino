
/**
 * Try to find an MAC Address, that's is not alread taken
 */
void safe_start_ethernet(){
  randomSeed(analogRead(0));
  byte mac[6] = { 
    0x00, 0xAA, 0xBB, 0xCC, (byte)group_num(), 0x00   };
  int conn_status = 0;
  do {
    connLedOff();
    mac[5] = (byte) random(255);
    conn_status = Ethernet.begin(mac);
    if (conn_status) {
      connLedOn();
      Serial.println("SUCCESSFULLY configured Ethernet using DHCP"); 
    }
    else {
      blinkNoConnection();
      //mac_addr=++mac_addr&0xFF; // loop though the bytes to find a right one
      Serial.println("FAILED to configure Ethernet using DHCP! Please restart ...");
    }
  } 
  while (conn_status == 0);
  
  // DEBUG //
  printIpToSerial();
}

void blinkNoConnection(){
        for (int i=0;i<5;i++) {
        digitalWrite(CONNECTION_LED,HIGH);
        delay(50);
        digitalWrite(CONNECTION_LED,LOW);
        delay(50);
      }
}

// *********  utility  ***********************************
void printIpToSerial() {
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
}


