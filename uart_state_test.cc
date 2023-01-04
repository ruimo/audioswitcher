#include <stdio.h>
#include <gtest/gtest.h>
#include "firmware/uart_state.h"
#include "firmware/device.h"

static std::vector<bool> uart_tx_ready_returns;

static std::vector<unsigned char> uart_tx_calls;

extern "C" {
  bool uart_tx_ready(void) {
    assert(uart_tx_ready_returns.size() != 0);
    bool r = uart_tx_ready_returns[0];
    uart_tx_ready_returns.erase(uart_tx_ready_returns.begin());
    return r;
  }

  void uart_tx(unsigned char c) {
    uart_tx_calls.push_back(c);
  }
}

TEST(TestUartState, init) {
  uart_tx_calls.clear();
  enter_uart_init();

  EXPECT_EQ(uart_state.id, UartStateInit);

  tick_uart();
  EXPECT_EQ(buffer_len(), 0);
  EXPECT_EQ(uart_tx_calls.size(), 0);

  uart_tx_ready_returns.push_back(false);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 0);
}

TEST(TestUartState, tx) {
  uart_tx_calls.clear();
  enter_uart_init();

  tick_uart();

  uart_tx_ready_returns.push_back(false);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 0);

  // TX ready but buffer empty.
  uart_tx_ready_returns.push_back(true);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 0);

  // Buffer is not empty buf TX is not ready.
  uart_tx_ready_returns.push_back(false);
  add_to_tx_buffer('A');
  add_to_tx_buffer('B');
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 0);

  uart_tx_ready_returns.push_back(true);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 1);
  EXPECT_EQ(uart_tx_calls[0], 'A');

  uart_tx_ready_returns.push_back(true);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 2);
  EXPECT_EQ(uart_tx_calls[1], 'B');

  uart_tx_ready_returns.push_back(true);
  tick_uart();
  EXPECT_EQ(uart_tx_calls.size(), 2);
}
