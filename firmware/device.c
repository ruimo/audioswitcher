#include <xc.h>
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

ButtonPressed button_pressed(void) {
  switch ((PORTBbits.RB7 == 0 ? 1 : 0) + (PORTBbits.RB6 == 0 ? 2 : 0)) {
  case 0:
    return NoButtonPressed;
  case 1:
    return In0Pressed;
  case 2:
    return In1Pressed;
  default:
    return BothPressed;
  }
}

void turn_on_led(Led led, bool on) {
  switch (led) {
  case Led0:
    LATBbits.LATB5 = on ? 1 : 0;
    break;

  case Led1:
    LATBbits.LATB3 = on ? 1 : 0;
    break; 
  }
}

bool uart_tx_ready(void) {
  return TXSTAbits.TRMT == 1;
}

void uart_tx(unsigned char c) {
  TXREG = c;
}

void start_ad(AdPort port) {
  switch (port) {
  case Ad0:
    ADCON0bits.CHS = 0;
    break;

  case Ad1:
    ADCON0bits.CHS = 1;
    break;
  }

  ADCON0bits.GO_nDONE = 1;
}

bool get_ad(uint16_t *pValue) {
  if (ADCON0bits.GO_nDONE != 0) {
    return false;
  } else {
    *pValue = ADRES;
    return true;
  }
}
