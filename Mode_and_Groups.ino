/*
DIL Switch Settings (read from RIGHT TO LEFT!)

For the Clients:
 1: 0000 (0)
 2: 0001 (1)
 3: 0010 (2)
 4: 0011 (3)
 5: 0100 (4)
 6: 0101 (5)
 7: 0110 (6)
 8: 0111 (7)
For the Servers:
 1: 1000 (8)
 2: 1001 (9)
 3: 1010 (10)
 4: 1011 (11)
 5: 1100 (12)
 6: 1101 (13)
 7: 1110 (14)
 8: 1111 (15)
*/


/**
 * Setup PORTC (AnalogIn)
 */
void setupGroupSwitch(){
 DDRC  |= B0; //B00111111;
 PORTC |= B00111111; // Pull up A0 to A3 to have reliable values
}

/**
 * Read group number
 */
int group_num() {
  return (int)(~PINC&B00000111); // mask the group number
}

#define GROUP_TMPL "group%i"
char __group_name[8];
char *group_name(){
  sprintf(__group_name,GROUP_TMPL,group_num());
  return __group_name;
}

int is_sender() {
  return (int)(~PINC&B00001000)>>3; // SERVER_BIT: B00001000 and shift to 1
}


