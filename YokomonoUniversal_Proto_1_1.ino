/**
 * YOKOMO PRO FIRMWARE Ethernet/WiFi VERSION 1.0
 * written @timelab, April 2011 by jens alexander ewald, http://ififelse.net
 *
 * Creative Commons Attribution - Noncommercial - Share alike 2.0 Belgium applies!
 */

#include <SPI.h>
#include <Ethernet.h>
#include "OSCClass.h"

/**
 * Define the PINS
 *
 * Do NOT use pins 10,11,12,13 (used by the ethernetshield for SPI)
 */
#define HAS_DTMF       2 // INPUT
#define MODE_PIN       3 // INPUT

#define POWER_LED      4
#define CONNECTION_LED 5
#define SIGNAL_LED     6

#define OUT_1          8
#define OUT_2          9


unsigned int PORT = 8888;      // send port (client)

//unsigned int localPort = 8888;      // send port (client)
//unsigned int destPort  = 6000;      // receive port (serve)

/**
 * OSC related
 */
byte destIp[]  = { 
  255, 255, 255, 255};

char *topAddress="/yokomonopro";
char *subAddress[1]={"1","2","3","4","5","6","7","8"};

OSCMessage recMes;
OSCMessage sendMes;

OSCClass osc(&recMes);

void setup() {
  // start the serial library:
  Serial.begin(19200);
  Serial.println("Connecting to Network ...");
  
  // start the Ethernet connection:
  safe_start_ethernet();
  // DEBUG
  printIpToSerial();
  
  setupGroupSwitch();
  
  osc.begin(localPort);
  osc.flush();

  pinMode(13,OUTPUT);
  pinMode(9,OUTPUT);

}

void loop() {
  double val = 0.0;  //double only ( not float) 
  int ival = 0;
  while(1) {
    
    if (is_sender()) {
      // update the server
      // read the value of the dtmf
      ival = val = (val==1.0) ? 0.0:1.0;
      sendMes.setArgs("f",&val);
      osc.sendOsc( &sendMes );
      delay(250);
    } 
    else {
      // update the client
      if(osc.available()) {
        logMessage(&recMes);
        uint8_t match = strcmp(recMes.getAddress(recMes.getAddressNum()-1),subAddress[0]);
        Serial.print(match);
        if (match == 0 && recMes.getTypeTag(0) == 'f') {
          ival = recMes.getArgFloat(0);
        } 
        Serial.print(" ");
        Serial.println(ival);

      }
    }
    digitalWrite(9,ival);
  }
}














