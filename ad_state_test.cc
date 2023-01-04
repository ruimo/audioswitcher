#include <gtest/gtest.h>
#include "firmware/ad_state.h"
#include "firmware/uart_state.h"
#include "firmware/device.h"

#define AD_NOT_FINISHED_VALUE 9999

static std::vector<AdPort> start_ad_calls;

static std::vector<uint16_t> get_ad_returns;

extern "C" {
  void start_ad(AdPort port) {
    start_ad_calls.push_back(port);
  }

  bool get_ad(uint16_t *pValue) {
    assert(get_ad_returns.size() != 0);
    uint16_t value = get_ad_returns[0];
    get_ad_returns.erase(get_ad_returns.begin());
    if (value == AD_NOT_FINISHED_VALUE) {
      return false;
    } else {
      *pValue = value;
      return true;
    }
  }
}

TEST(TestAdState, tx_value) {
  tx_value(1234);
  EXPECT_EQ(remove_from_tx_buffer(), '1');
  EXPECT_EQ(remove_from_tx_buffer(), '2');
  EXPECT_EQ(remove_from_tx_buffer(), '3');
  EXPECT_EQ(remove_from_tx_buffer(), '4');
  EXPECT_EQ(remove_from_tx_buffer(), -1);
}

