/* ------------ OUTPUT ------------------ */

void setupOutput(){
 DDRB  |= B00000011; // pin 8 & 9 as output
 outputOff();
}

void out1(char state){
 PORTB = (state&0x1) ? PORTB | 0x1 : PORTB & 0xfe;
}

void out2(char state){
 PORTB = (state&0x1) ? PORTB | 0x2 : PORTB & 0xfd;
}

void outputOff(){
  PORTB &= 0xfc;
}


