#include "device.h"

void turn_on_relay(Coil coil, bool on) {
  switch (coil) {
  case Coil0:
    LATBbits.LATB0 = (on ? 1 : 0);
    break;

  default:
    LATBbits.LATB2 = (on ? 1 : 0);
    break;
  }
}

bool in_0_pressed(void) {
  return PORTBbits.RB7 == 0;
}

bool in_1_pressed(void) {
  return PORTBbits.RB6 == 0;
}
