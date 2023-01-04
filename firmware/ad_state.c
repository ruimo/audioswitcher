#include "ad_state.h"
#include "uart_state.h"
#include "device.h"
#include <stdint.h>

uint16_t ad0_value = 0;
uint16_t ad1_value = 0;

AdState ad_state = {
  AdStateWait,
  {
    {
      AD_WAIT
    }
  },
};

void enter_ad_wait(void) {
  ad_state.id = AdStateWait;
  ad_state.StateData.Wait.wait = AD_WAIT;
}

void on_ad_wait_state(void) {
  if (ad_state.StateData.Wait.wait != 0) {
    --ad_state.StateData.Wait.wait;
  } else {
    enter_ad_0();
  }
}

void enter_ad_0(void) {
  start_ad(Ad0);
  ad_state.id = AdState0;
}

void tx_value(uint16_t value) {
  uint16_t d = value / 1000;
  add_to_tx_buffer('0' + (uint8_t)d);
  value -= d * 1000;

  d = value / 100;
  add_to_tx_buffer('0' + (uint8_t)d);
  value -= d * 100;

  d = value / 10;
  add_to_tx_buffer('0' + (uint8_t)d);
  value -= d * 10;

  add_to_tx_buffer('0' + (uint8_t)value);
}

void on_ad_0(void) {
  if (get_ad(&ad0_value)) {
    tx_value(ad0_value);
    add_to_tx_buffer('/');

    enter_ad_1();
  }
}

void enter_ad_1(void) {
  start_ad(Ad1);
  ad_state.id = AdState1;
}

void on_ad_1(void) {
  if (get_ad(&ad1_value)) {
    tx_value(ad1_value);
    add_to_tx_buffer('\r');
    add_to_tx_buffer('\n');

    enter_ad_wait();
  }
}

void tick_ad(void) {
  switch (ad_state.id) {
  case AdStateWait:
    on_ad_wait_state();
    break;

  case AdState0:
    on_ad_0();
    break;
  
  case AdState1:
    on_ad_1();
    break;
  }
}
