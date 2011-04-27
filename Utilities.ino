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

