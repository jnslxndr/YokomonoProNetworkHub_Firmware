/**
 * Try to find an MAC Address, that's is not alread taken
 */

void safe_start_ethernet(){
  set_leds(CL,LOW);
  int conn_status = 0;
  byte mac[6] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDD, MAC_ADDR};
  do {
    conn_status = Ethernet.begin(mac);
    if (conn_status) {
      set_leds(CL,HIGH);
      Serial.println("SUCCESSFULLY configured Ethernet using DHCP"); 
    }
    else {
      blinkNoConnection();
      Serial.println("FAILED to configure Ethernet using DHCP! Please restart ...");
    }
  } while (conn_status == 0);
  
  // DEBUG //
  printIpToSerial();
}

void blinkNoConnection(){
  char state = 0x1;
  for (int i=0;i<5;i++) {
        set_leds(CL,state); state = !state;
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


