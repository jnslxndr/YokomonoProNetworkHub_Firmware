/* ----------- LEDs ------------------- */

// LEDS: B11100010 DDRD

void setupLeds(){
  
  pinMode(POWER_LED, OUTPUT);
  pinMode(CONNECTION_LED, OUTPUT );
  pinMode(SIGNAL_LED,OUTPUT );
  return;
  
  DDRD   = B11100010;  // rigth most: 10 for Serial transmit
  PORTD &= B00011111; // turns off 2..7, but leaves pins 0 and 1 alone
}


void set_leds(char leds, char state) {
  PORTD = (state) ? PIND|leds : PIND&~leds; // set state
}

void allLedOff(){
  PORTD &= B00011111; // LEDS off
}

void powerLedOn(){
  PORTD |= B00100000;
}

void powerLedOff(){
  PORTD &= B11011111;
}

void connLedOn(){
  PORTD |= B01000000;
}

void connLedOff(){
  PORTD &= B10111111;
}

void statusLedOn(){
  PORTD |= B10000000;
}

void statusLedOff(){
  PORTD &= B01111111;
}


