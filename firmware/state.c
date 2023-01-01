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
        } Init;

        struct {
            uint16_t relay_wait;
        } In;
    } StateData;
} State;

void init_state(void) {
    State.id = Init;
}

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
  if (in_1_pressed()) {
    enter_in_1();
  } else {
    if (State.StateData.In.relay_wait != 0) {
      --State.StateData.In.relay_wait;
    } else {
      turn_on_relay(Coil0, false);
    }
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
            break;
            
        default:
            break;
    }
    
    
//    PORTBbits_t portb = PORTBbits;
//    if (portb.RB7 == 0) {
//        LATBbits.LATB5 = 1;
//        LATBbits.LATB3 = 0;
//    } else if (portb.RB6 == 0) {
//        LATBbits.LATB5 = 0;
//        LATBbits.LATB3 = 1;
//    } else {
//        LATBbits.LATB5 = 0;
//        LATBbits.LATB3 = 0;
//    }
}
