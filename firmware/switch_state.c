#include <stdint.h>
#include "switch_state.h"
#include "device.h"
#include "ad_state.h"

SwitchState switch_state = {
    SwitchInit,
    {},
};

void enter_switch_init(void) {
    switch_state.id = SwitchInit;
}

void on_switch_init_state(void) {
    enter_switch_in_0();
}

void enter_switch_in_0(void) {
    switch_state.id = In0;
    switch_state.StateData.In.relay_wait = RELAY_WAIT;
    turn_on_relay(Coil1, false);
    turn_on_led(Led1, false);

    turn_on_relay(Coil0, true);
    turn_on_led(Led0, true);
}

void enter_switch_in_1(void) {
    switch_state.id = In1;
    switch_state.StateData.In.relay_wait = RELAY_WAIT;
    turn_on_relay(Coil0, false);
    turn_on_led(Led0, false);

    turn_on_relay(Coil1, true);
    turn_on_led(Led1, true);
}

void on_switch_in_0(void) {
    if (AD_THRESHOLD < ad1_value) {
        enter_switch_in_1();
        return;
    }
    
  switch (button_pressed()) {
  case BothPressed:
    /* TODO */
    break;

  case In1Pressed:
    enter_switch_in_1();
    break;

  default:
    if (switch_state.StateData.In.relay_wait != 0) {
      --switch_state.StateData.In.relay_wait;
    } else {
      turn_on_relay(Coil0, false);
    }
    break;
  }
}

void on_switch_in_1(void) {
    if (AD_THRESHOLD < ad0_value) {
        enter_switch_in_0();
        return;
    }

    switch (button_pressed()) {
  case BothPressed:
    /* TODO */
    break;

  case In0Pressed:
    enter_switch_in_0();
    break;

  default:
    if (switch_state.StateData.In.relay_wait != 0) {
      --switch_state.StateData.In.relay_wait;
    } else {
      turn_on_relay(Coil1, false);
    }
    break;
  }
}

void tick_switch(void) {
    switch (switch_state.id) {
        case SwitchInit:
            on_switch_init_state();
            break;
            
        case In0:
            on_switch_in_0();
            break;

        case In1:
            on_switch_in_1();
            break;
            
        default:
            break;
    }
}
