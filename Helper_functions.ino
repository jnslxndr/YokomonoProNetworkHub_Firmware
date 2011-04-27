/**
 * Setup PORTC (AnalogIn)
 */
void setupGroupPort(){
 DDRC  |= B00001111;
 PORTC |= B00001111; // Pull up A0 to A3 to have reliable values
}


/**
 * Read group number
 */

int group_num() {
  return (int)(~PINC&B00001111); // mask the first 4 ports
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
}

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

// *********  utility  ***********************************
void logMessage(OSCMessage *mes){
  uint8_t *ip=mes->getIp();

  //disp ip & port
  Serial.print("from IP:");
  Serial.print(ip[0],DEC);
  Serial.print(".");
  Serial.print(ip[1],DEC);
  Serial.print(".");
  Serial.print(ip[2],DEC);
  Serial.print(".");
  Serial.print(ip[3],DEC);
  Serial.print(" port:");
  Serial.print(mes->getPort(),DEC);
  Serial.print("   ");

  //disp adr
  for(int i = 0 ; i < mes->getAddressNum() ; i++){
    Serial.print("/");
    Serial.print(mes->getAddress(i));
  }

  //disp type tags
  Serial.print("  ,");
  for(int i = 0 ; i < mes->getArgNum() ; i++){
    Serial.print(mes->getTypeTag(i));
    Serial.print(" ");
  }

  //disp args
  for(int i = 0 ; i < mes->getArgNum() ; i++){

    switch( mes->getTypeTag(i) ){

    case 'i': 
      {
        Serial.print( mes->getArgInt(i) );
        Serial.print(" ");
      }
      break;

    case 'f':  
      {
        Serial.print( mes->getArgFloat(i) );
        Serial.print(" ");
      }
      break;

    }
  }
  Serial.println(""); // last thing is a LF
}



