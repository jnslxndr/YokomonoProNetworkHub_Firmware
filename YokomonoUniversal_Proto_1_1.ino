/**
 * YOKOMO PRO FIRMWARE Ethernet/WiFi VERSION 1.0
 * written @timelab, April 2011 by jens alexander ewald, http://ififelse.net
 *
 * Creative Commons Attribution - Noncommercial - Share alike 2.0 Belgium applies!
 *
 * Do NOT use pins 10,11,12,13 (used by the ethernetshield for SPI)
 */

#include <SPI.h>
#include <Ethernet.h>
#include "OSCClass.h"

#define MAX_GROUPS 8

#define POWER_LED      5 // OUT
#define CONNECTION_LED 6 // OUT
#define SIGNAL_LED     7 // OUT

#define PL 0x20
#define CL 0x40
#define SL 0x80
#define AL 0xE0

/**
 * OSC related
 */
byte BROADCAST_IP[] = {255,255,255,255}; // we broadcast to all networks
char *MAIN_ADDR     = "yokomonopro"; // main address
char *GLOBAL        = "all"; // global address

unsigned int SERVER_PORT = 8888;      // send port (client)
unsigned int CLIENT_PORT = 6000;      // receive port (serve)

OSCMessage recMes;
OSCMessage sendMes;
OSCClass   osc(&recMes);



void setup() {
  // start the serial library:
  Serial.begin(19200);
  
  /*
   Basic Setup
  */
  Serial.println("Booting ...");
  setupLeds();
  
  set_leds(PL,HIGH);
  delay(1000);
  set_leds(PL|CL,HIGH);
  delay(1000);
  set_leds(PL|CL|SL,HIGH);
  delay(1000);
  set_leds(PL|CL|SL,LOW);

  setupGroupSwitch();
  setupOutput();
  
  threetimes();  
  powerLedOn();
  delay(100);
  
  
  /*
    NetworkSetup
  */
  Serial.println("Connecting to Network ...");
  safe_start_ethernet();
  /** Setup Ports to listen to **/
  setupOSC();
  delay(100);
  
  char buffer[32];
  sprintf(buffer,"Group: %s | Sender? %i",group_name(),is_sender());
  Serial.println(buffer);


}

void loop() {
  double val1 = 0.0;  //double only ( not float) 
  double val2 = 0.0;  //double only ( not float) 
  int r1val = 0;
  int r2val = 0;
  int ival = 0;
  while(1) {
    if (is_sender()) {
      Serial.println("not yet implemented!");
      delay(1000);
      // update the server
      // read the value of the dtmf
      /*
      sendMes.setArgs("f",&val1);
      osc.sendOsc( &sendMes );
      */
    } 
    else {
      // update the client
      if(osc.available()) {
        // check for PING message?
        //logMessage(&recMes);
        //Serial.println(group_name());
        //char buffer[32];
        //sprintf(buffer,"topaddr: %s | soll: %i",recMes.getTopAddress(),MAIN_ADDR);
        //Serial.println(buffer);
        
        //sprintf(buffer,"subaddr: %s | soll: %i",recMes.getSubAddress(),group_name());
        //Serial.println(buffer);

        //Serial.println(strcmp(recMes.getTopAddress(),MAIN_ADDR),DEC);
        //Serial.println(strcmp(recMes.getSubAddress(),group_name()),DEC);
        
        if(!strcmp(recMes.getTopAddress(),MAIN_ADDR) &&
           !strcmp(recMes.getSubAddress(),group_name()) ) {
          r1val = (recMes.getTypeTag(0) == 'f') ? recMes.getArgFloat(0) : 0;
        } else if ((!strcmp(recMes.getTopAddress(),MAIN_ADDR) &&
           !strcmp(recMes.getSubAddress(),GLOBAL)) && recMes.getArgNum()<=MAX_GROUPS ) {
             logMessage(&recMes);
          r1val = (recMes.getTypeTag(group_num()) == 'f') ? recMes.getArgFloat(0) : 0;
        }
        
        //char vals[32];
        //sprintf(vals,"val1: %i, val2: %i",(int)val1,(int)val2);
        //Serial.println(vals);
      }
      
      // OSC data received..
      PORTD = (r1val||r2val) ? PIND|0x80 : PIND&0x7f; // set state
      
      out1(r1val);
      out2(r2val);
    }
  }
}














