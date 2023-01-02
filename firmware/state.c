#include <stdint.h>
#include "state.h"
#include "device.h"

#define RELAY_WAIT 10000

typedef enum {
    Init, In0, In1,
} StateId;

struct {
    StateId id;
    union {
        struct {
            uint16_t relay_wait;
        } In;
    } StateData;
} State = {
    Init,
    {},
};

void on_init_state(void) {
    enter_in_0();
}

void enter_in_0(void) {
    State.id = In0;
    State.StateData.In.relay_wait = RELAY_WAIT;
    turn_on_relay(Coil1, false);
    turn_on_relay(Coil0, true);
}

void enter_in_1(void) {
    State.id = In1;
    State.StateData.In.relay_wait = RELAY_WAIT;
    turn_on_relay(Coil0, false);
    turn_on_relay(Coil1, true);
}

void on_in_0(void) {
  switch (button_pressed()) {
  case BothPressed:
    /* TODO */
    break;

  case In1Pressed:
    enter_in_1();
    break;

  default:
    if (State.StateData.In.relay_wait != 0) {
      --State.StateData.In.relay_wait;
    } else {
      turn_on_relay(Coil0, false);
    }
    break;
  }
}

void on_in_1(void) {
  switch (button_pressed()) {
  case BothPressed:
    /* TODO */
    break;

  case In0Pressed:
    enter_in_0();
    break;

  default:
    if (State.StateData.In.relay_wait != 0) {
      --State.StateData.In.relay_wait;
    } else {
      turn_on_relay(Coil1, false);
    }
    break;
  }
}

void tick_switch(int32_t i) {
    switch (State.id) {
        case Init:
            on_init_state();
            break;
            
        case In0:
            on_in_0();
            break;

        case In1:
            on_in_1();
            break;
            
        default:
            break;
    }
}
