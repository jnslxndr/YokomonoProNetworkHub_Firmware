/**
 * YOKOMO PRO FIRMWARE Ethernet/WiFi VERSION 1.0
 * written @timelab, April 2011 by jens alexander ewald, http://ififelse.net
 *
 * Creative Commons Attribution - Noncommercial - Share alike 2.0 Belgium applies!
 *
 * Do NOT use pins 10,11,12,13 (used by the ethernetshield for SPI)
 *
 * Libraries used: OSCClass (http://recotana.com, GPL 2.1)
 *
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
  
  char state = 0x1;
  for (int i=0;i<3;i++) {
        set_leds(SL,state); state = !state;
        delay(250);
  }
  set_leds(PL,HIGH);
  
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
      // update the server
      // read the value of the dtmf
      /*
      sendMes.setArgs("f",&val1);
      osc.sendOsc( &sendMes );
      */
    } 
    else {
      unsigned long now = millis();
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
        
        // check for timeout
        
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
        
        //char vals[32];
        //sprintf(vals,"val1: %i, val2: %i",(int)val1,(int)val2);
        //Serial.println(vals);
      }
      
      byte timed_out = (next_ping<now-timeout);

      if (timed_out) { r1val = 0; r2val = 0; }
      // OSC data received..

      byte sig_state = (r1val|r2val); //&timed_out;
      PORTD = (PIND&B00011111)|((B01100000)|(sig_state<<7));

      byte v1 = (r1val&0x1);
      byte v2 = (r2val&0x1<<1);
      PORTB = (PINB&B11111100)|((v1|v2));//&timed_out);
    }
  }
}














