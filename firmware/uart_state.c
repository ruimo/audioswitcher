#include <stdio.h>
#include <stdint.h>
#include "ringbuffer.h"
#include "uart_state.h"
#include "device.h"

static RingBuffer buf;

UartState uart_state = {
    UartStateInit,
};

void enter_uart_init(void) {
  uart_state.id = UartStateInit;
}

uint8_t buffer_len(void) {
  return buf.len;
}

bool add_to_tx_buffer(uint8_t c) {
  return add_to_buffer(&buf, c);
}

int16_t remove_from_tx_buffer(void) {
  if (buf.len == 0) {
    return -1;
  } else {
    uint8_t c;
    remove_from_buffer(&buf, &c);
    return c;
  }
}

void tick_uart(void) {
  switch (uart_state.id) {
  case UartStateInit:
    init_buffer(&buf);
    uart_state.id = UartStateReady;
    break;

  case UartStateReady:
    if (uart_tx_ready()) {
      uint8_t c;
      if (remove_from_buffer(&buf, &c)) {
        uart_tx(c);
      }
    }
    break;
  }
}
