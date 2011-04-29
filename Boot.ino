
void bootSequence(int pause){
  allLedOff();
  powerLedOn();
  delay(pause);
  powerLedOff();
  connLedOn();
  delay(pause);
  connLedOff();
  statusLedOn();
  delay(pause);
  allLedOff();
  delay(pause);
}

void bootSequence(){
  bootSequence(200);
}

void threetimes(){
  for (int i=0; i<3; i++) bootSequence(75);
}

/*
  digitalWrite(POWER_LED, HIGH);
  digitalWrite(CONNECTION_LED, LOW );
  digitalWrite(SIGNAL_LED,LOW );
  delay(pause);
  digitalWrite(POWER_LED, HIGH);
  digitalWrite(CONNECTION_LED, HIGH);
  digitalWrite(SIGNAL_LED,LOW );
  delay(pause);
  digitalWrite(POWER_LED, LOW);
  digitalWrite(CONNECTION_LED, HIGH);
  digitalWrite(SIGNAL_LED,HIGH);
  delay(pause);
  digitalWrite(POWER_LED, LOW );
  digitalWrite(CONNECTION_LED, LOW);
  digitalWrite(SIGNAL_LED,HIGH);
  delay(pause);
  digitalWrite(POWER_LED, LOW );
  digitalWrite(CONNECTION_LED, LOW );
  digitalWrite(SIGNAL_LED,LOW);
  delay(pause);
*/
