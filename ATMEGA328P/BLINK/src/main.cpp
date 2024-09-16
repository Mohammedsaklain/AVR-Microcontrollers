#include <Arduino.h>

#define PB *((volatile byte *)0x25)  // PORTB register(Special function register) for HIGH/LOW, location in memory 0x25, 
#define DB *((volatile byte *)0x24)  // DDRB register(Special function register) for setting INPUT/OUTPUT , location in memory 0x24, 
void setup() {
  DB = (1<<5);
}
void loop() { 
  PB = (1<<5);
  for(long i = 0; i < 1000000; i++) {
    PB = (1<<5);
  }
  PB = (0<<5);
  for(long i = 0; i < 1000000; i++) {
    PB = (0<<5);
  }
}