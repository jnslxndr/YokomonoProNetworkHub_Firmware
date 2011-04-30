/**
 * YOKOMO PRO FIRMWARE Ethernet/WiFi VERSION 1.0
 * written @timelab, April 2011 by jens alexander ewald, http://ififelse.net
 *
 * Creative Commons Attribution - Noncommercial - Share alike 2.0 applies!
 *
 * See http://creativecommons.org/licenses/by-nc-sa/2.0/legalcode
 *
 * Do NOT use pins 10,11,12,13 (used by the ethernetshield for SPI)
 * 
 * Libraries used: OSCClass (http://recotana.com, GPL 2.1)
 * 
 * This Firmware uses commit 059bc5f of the new-extension branch. See:
 * https://github.com/arduino/Arduino/tree/059bc5f
 * 
 * THIS IS A BETA VERSION! DO NOT EXPECT IT TO WORK FLAWLESSLY! WORKS FOR US.
 * 
 * VERSION USED FOR CONCERT @TIMELAB GENT (http://timelab.org), 30th April 2011
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

//#define MAC_ADDR 0x00 //  0
//#define MAC_ADDR 0x01 //  1
//#define MAC_ADDR 0x02 //  2
//#define MAC_ADDR 0x03 //  3
//#define MAC_ADDR 0x04 //  4
//#define MAC_ADDR 0x05 //  5
//#define MAC_ADDR 0x06 //  6
//#define MAC_ADDR 0x07 //  7
//#define MAC_ADDR 0x08 //  8
//#define MAC_ADDR 0x09 //  9
//#define MAC_ADDR 0x0A // 10
//#define MAC_ADDR 0x0B // 11
//#define MAC_ADDR 0x0C // 12
//#define MAC_ADDR 0x0D // 13
//#define MAC_ADDR 0x0E // 14
//#define MAC_ADDR 0x0F // 15
//#define MAC_ADDR 0x10 // 16
//#define MAC_ADDR 0x11 // 17

//#define MAC_ADDR 0x12 // 18 // spare -> defect

//#define MAC_ADDR 0x13 // 19
//#define MAC_ADDR 0x14 // 20
//#define MAC_ADDR 0x15 // 21
//#define MAC_ADDR 0x16 // 22
//#define MAC_ADDR 0x17 // 23
//#define MAC_ADDR 0x18 // 24
//#define MAC_ADDR 0x19 // 25
//#define MAC_ADDR 0x1A // 26
//#define MAC_ADDR 0x1B // 27
//#define MAC_ADDR 0x1C // 28
//#define MAC_ADDR 0x1D // 29


/**
 * OSC related
 */
byte BROADCAST_IP[] = {255,255,255,255}; // we broadcast to all networks
char *MAIN_ADDR     = "yokomonopro"; // main address
char *GLOBAL        = "all"; // global address
char *PING          = "ping"; // keep alive ping

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
  
  /*
    Fancy display ...
  */
  set_leds(PL,HIGH);
  delay(1000);
  set_leds(PL|CL,HIGH);
  delay(1000);
  set_leds(PL|CL|SL,HIGH);
  delay(1000);
  set_leds(PL | CL | SL,LOW);

  Serial.println("Setting up GroupSwitch");
  setupGroupSwitch();
  
  Serial.println("Setting up Outputs");
  setupOutput();
  
  // first round done
  threetimes();  
  set_leds(PL,HIGH);
  delay(100);
  
  
  /*
    NetworkSetup
  */
  Serial.println("Connecting to Network ...");
  safe_start_ethernet();
  
  /** Setup Ports to listen to **/
  setupOSC();
  
  /*
    Blink shortly to signal end of setup
  */
  char state = 0x1;
  for (int i=0;i<3;i++) {
        set_leds(SL,state); state = !state;
        delay(250);
  }
  set_leds(PL,HIGH); // bug?
  
  char buffer[32];
  sprintf(buffer,"Group: %s | Sender? %i",group_name(),is_sender());
  Serial.println(buffer);

}

void loop() {
  double val1 = 0.0;  //double only ( not float) 
  double val2 = 0.0;  //double only ( not float) 
  int r1val = 0;
  int r2val = 0;
  
  unsigned long timeout  = 300;
  unsigned long pingtime = 500;
  unsigned long next_ping = millis()+pingtime;
  unsigned long last_message = millis();
  
  for(;;) {
    if (is_sender()) {
      Serial.println("not yet implemented!");
      delay(1000);
      /* NOT YET IMPLEMENTED! But it should work as follows:
			char dtmf;
      readDTMF(&dtmf);
			if changed:
      	sendMes.setArgs("f",&dtmf);
      	osc.sendOsc( &sendMes );
      */
    } 
    else {
      unsigned long now = millis();
      // update the client
      if(osc.available()) {
        if (!strcmp(recMes.getTopAddress(),PING)) {
          if (recMes.getArgNum()>0) timeout = (recMes.getTypeTag(0) == 'i') ? recMes.getArgInt(0):timeout;
          if (recMes.getArgNum()>1) pingtime = (recMes.getTypeTag(1) == 'i') ? recMes.getArgInt(1):pingtime;
          next_ping = now+pingtime;
        }
        
        if(!strcmp(recMes.getTopAddress(),MAIN_ADDR) && !strcmp(recMes.getSubAddress(),group_name()) ) {
          r1val = (recMes.getTypeTag(0) == 'f') ? recMes.getArgFloat(0) : 0;
          next_ping = now+pingtime;
        }        
        osc.flush();
      }
      
      if ((next_ping<now-timeout)) { r1val = 0; r2val = 0; }

      byte sig_state = (r1val|r2val);
      PORTD = (PIND&B00011111)|((B01100000)|(sig_state<<7));

      byte v1 = (r1val&0x1);
      byte v2 = (r2val&0x1<<1);
      PORTB = (PINB&B11111100)|((v1|v2));
    }
  }
}














