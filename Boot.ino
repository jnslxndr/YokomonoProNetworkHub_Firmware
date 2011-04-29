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

void threetimes(){
  for (int i=0; i<3; i++) bootSequence(75);
}

