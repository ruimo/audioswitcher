#include <gtest/gtest.h>
#include <cassert>
#include "firmware/switch_state.h"
#include "firmware/device.h"

typedef struct {
  Coil coil;
  bool on;
} TurnOnRelayCall;

static std::vector<TurnOnRelayCall> turn_on_relay_calls;

static std::vector<ButtonPressed> button_pressed_returns;

typedef struct {
  Led led;
  bool on;
} TurnOnLedCall;

static std::vector<TurnOnLedCall> turn_on_led_calls;

extern "C" {
  void turn_on_relay(Coil coil, bool on) {
    TurnOnRelayCall call = {
      coil, on,
    };
    turn_on_relay_calls.push_back(call);
  }

  ButtonPressed button_pressed(void) {
    assert(button_pressed_returns.size() != 0);
    ButtonPressed bp = button_pressed_returns[0];
    button_pressed_returns.erase(button_pressed_returns.begin());
    return bp;
  }

  void turn_on_led(Led led, bool on) {
    TurnOnLedCall call = {
      led, on,
    };
    turn_on_led_calls.push_back(call);
  }
}

TEST(TestSwitchState, init) {
  turn_on_relay_calls.clear();
  turn_on_led_calls.clear();
  enter_switch_init();

  EXPECT_EQ(switch_state.id, SwitchInit);
  EXPECT_EQ(turn_on_relay_calls.size(), 0);
  EXPECT_EQ(turn_on_led_calls.size(), 0);
}

TEST(TestSwitchState, enter_in0_after_init) {
  turn_on_relay_calls.clear();
  turn_on_led_calls.clear();
  enter_switch_init();
  tick_switch();

  EXPECT_EQ(switch_state.id, In0);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, RELAY_WAIT);
  EXPECT_EQ(turn_on_relay_calls.size(), 2);

  EXPECT_EQ(turn_on_relay_calls[0].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[0].on, false);
  EXPECT_EQ(turn_on_relay_calls[1].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[1].on, true);

  EXPECT_EQ(turn_on_led_calls.size(), 2);
  EXPECT_EQ(turn_on_led_calls[0].led, Led1);
  EXPECT_EQ(turn_on_led_calls[0].on, false);
  EXPECT_EQ(turn_on_led_calls[1].led, Led0);
  EXPECT_EQ(turn_on_led_calls[1].on, true);

  for (int i = RELAY_WAIT - 1; 0 <= i; --i) {
    button_pressed_returns.push_back(NoButtonPressed);
    tick_switch();
    EXPECT_EQ(switch_state.id, In0);
    EXPECT_EQ(switch_state.StateData.In.relay_wait, i);
    EXPECT_EQ(turn_on_relay_calls.size(), 2);
    EXPECT_EQ(turn_on_led_calls.size(), 2);
  }

  button_pressed_returns.push_back(NoButtonPressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In0);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, 0);
  EXPECT_EQ(turn_on_relay_calls.size(), 3);
  EXPECT_EQ(turn_on_relay_calls[2].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[2].on, false);

  EXPECT_EQ(turn_on_led_calls.size(), 2);

  button_pressed_returns.push_back(NoButtonPressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In0);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, 0);
  EXPECT_EQ(turn_on_relay_calls.size(), 4);
  EXPECT_EQ(turn_on_relay_calls[3].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[3].on, false);
  EXPECT_EQ(turn_on_led_calls.size(), 2);

  button_pressed_returns.push_back(In0Pressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In0);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, 0);
  EXPECT_EQ(turn_on_relay_calls.size(), 5);
  EXPECT_EQ(turn_on_relay_calls[4].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[4].on, false);
  EXPECT_EQ(turn_on_led_calls.size(), 2);

  button_pressed_returns.push_back(In1Pressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In1);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, RELAY_WAIT);
  EXPECT_EQ(turn_on_relay_calls.size(), 7);
  EXPECT_EQ(turn_on_relay_calls[5].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[5].on, false);
  EXPECT_EQ(turn_on_relay_calls[6].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[6].on, true);
  EXPECT_EQ(turn_on_led_calls.size(), 4);
  EXPECT_EQ(turn_on_led_calls[2].led, Led0);
  EXPECT_EQ(turn_on_led_calls[2].on, false);
  EXPECT_EQ(turn_on_led_calls[3].led, Led1);
  EXPECT_EQ(turn_on_led_calls[3].on, true);
}

TEST(TestSwitchState, enter_in1_after_init) {
  turn_on_relay_calls.clear();
  turn_on_led_calls.clear();
  enter_switch_init();
  tick_switch();

  button_pressed_returns.push_back(In1Pressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In1);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, RELAY_WAIT);
  EXPECT_EQ(turn_on_relay_calls.size(), 4);
  EXPECT_EQ(turn_on_relay_calls[0].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[0].on, false);
  EXPECT_EQ(turn_on_relay_calls[1].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[1].on, true);
  EXPECT_EQ(turn_on_relay_calls[2].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[2].on, false);
  EXPECT_EQ(turn_on_relay_calls[3].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[3].on, true);

  EXPECT_EQ(turn_on_led_calls.size(), 4);
  EXPECT_EQ(turn_on_led_calls[0].led, Led1);
  EXPECT_EQ(turn_on_led_calls[0].on, false);
  EXPECT_EQ(turn_on_led_calls[1].led, Led0);
  EXPECT_EQ(turn_on_led_calls[1].on, true);
  EXPECT_EQ(turn_on_led_calls[2].led, Led0);
  EXPECT_EQ(turn_on_led_calls[2].on, false);
  EXPECT_EQ(turn_on_led_calls[3].led, Led1);
  EXPECT_EQ(turn_on_led_calls[3].on, true);

  for (int i = RELAY_WAIT - 1; 0 <= i; --i) {
    button_pressed_returns.push_back(NoButtonPressed);
    tick_switch();
    EXPECT_EQ(switch_state.id, In1);
    EXPECT_EQ(switch_state.StateData.In.relay_wait, i);
    EXPECT_EQ(turn_on_relay_calls.size(), 4);
    EXPECT_EQ(turn_on_led_calls.size(), 4);
  }

  button_pressed_returns.push_back(NoButtonPressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In1);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, 0);
  EXPECT_EQ(turn_on_relay_calls.size(), 5);
  EXPECT_EQ(turn_on_relay_calls[4].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[4].on, false);
}

TEST(TestSwitchState, change_to_in1_at_once) {
  turn_on_relay_calls.clear();
  turn_on_led_calls.clear();
  enter_switch_init();
  tick_switch();

  EXPECT_EQ(switch_state.id, In0);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, RELAY_WAIT);
  EXPECT_EQ(turn_on_relay_calls.size(), 2);

  button_pressed_returns.push_back(In1Pressed);
  tick_switch();
  EXPECT_EQ(switch_state.id, In1);
  EXPECT_EQ(switch_state.StateData.In.relay_wait, RELAY_WAIT);
  EXPECT_EQ(turn_on_relay_calls.size(), 4);
  EXPECT_EQ(turn_on_relay_calls[2].coil, Coil0);
  EXPECT_EQ(turn_on_relay_calls[2].on, false);
  EXPECT_EQ(turn_on_relay_calls[3].coil, Coil1);
  EXPECT_EQ(turn_on_relay_calls[3].on, true);

  EXPECT_EQ(turn_on_led_calls.size(), 4);
  EXPECT_EQ(turn_on_led_calls[2].led, Led0);
  EXPECT_EQ(turn_on_led_calls[2].on, false);
  EXPECT_EQ(turn_on_led_calls[3].led, Led1);
  EXPECT_EQ(turn_on_led_calls[3].on, true);
}
