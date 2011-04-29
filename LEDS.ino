/* ----------- LEDs ------------------- */

// LEDS: B11100010 DDRD
#define POWER_LED      5 // OUT
#define CONNECTION_LED 6 // OUT
#define SIGNAL_LED     7 // OUT

void setupLeds(){
  
  pinMode(POWER_LED, OUTPUT);
  pinMode(CONNECTION_LED, OUTPUT );
  pinMode(SIGNAL_LED,OUTPUT );
  return;
  
  DDRD   = B1111110;  // rigth most: 10 for Serial transmit
  PORTD &= B00000011; // turns off 2..7, but leaves pins 0 and 1 alone
}

void allLedOff(){
  PORTD &= B00000011; // LEDS off
}

void powerLedOn(){
  PORTD |= B00100000;
}

void powerLedOff(){
  PORTD &= ~B00100000;
}

void connLedOn(){
  PORTD |= B01000000;
}

void connLedOff(){
  PORTD &= ~B0100000;
}

void statusLedOn(){
  PORTD |= B10000000;
}

void statusLedOff(){
  PORTD &= ~B1000000;
}


