/*
 YOKOMO PRO FIRMWARE WiFi VERSION 1
 written @timelab, April 2011
 CC applies
 */

#include <SPI.h>
#include <Ethernet.h>
//#include <Udp.h>   // UDP library from: bjoern@cs.stanford.edu 12/30/2008
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



//char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
// UDP Udp;

int counter = 0;

IPAddress bc_addr(255, 255, 255, 255); // absolute BC
//IPAddress bc_addr(224,0,0,1); // official BC, does not work with WizNet Chip

unsigned int localPort = 8888;      // local port to listen on

/**
 * OSC related
 */
byte destIp[]  = { 
  255, 255, 255, 255};
int  destPort = 6000;

char *topAddress="/yokomono";
char *subAddress[1]={ 
  "red1" };

OSCMessage recMes;
OSCMessage sendMes;

OSCClass osc(&recMes);

unsigned int SENDER = 0;


void setup() {
  // start the serial library:
  Serial.begin(19200);
  Serial.println("Connecting to Network ...");
  
  // start the Ethernet connection:
  safe_start_ethernet();
  
  printIpToSerial();

  //  Udp.begin(localPort);

  if(SENDER){
    osc.begin(localPort);
    osc.flush();
    sendMes.setIp( destIp );
    sendMes.setPort( destPort );
    sendMes.setTopAddress(topAddress);
    sendMes.setSubAddress(subAddress[0]);
  } 
  else {
    osc.begin(destPort);
    osc.flush();
  }

  pinMode(13,OUTPUT);
  pinMode(9,OUTPUT);
}

void loop() {
  
  /*
   sprintf(packetBuffer,"ping %0i3",counter);
   Udp.beginPacket(bc_addr,6000); // bc on 6000
   Udp.write(packetBuffer);
   Udp.endPacket();
   counter++;
   */

  double val = 0.0;  //double only ( not float) 
  int ival = 0;
  while(1){

    if (SENDER) {
      ival = val = (val==1.0) ? 0.0:1.0;
      sendMes.setArgs("f",&val);
      osc.sendOsc( &sendMes );
      delay(250);
    } 
    else {
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














