#include <stdbool.h> 

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum {
    Coil0, Coil1,
  } Coil;

  void turn_on_relay(Coil coil, bool on);

  typedef enum {
    NoButtonPressed, In0Pressed, In1Pressed, BothPressed,
  } ButtonPressed;

  ButtonPressed button_pressed(void);

  typedef enum {
    Led0, Led1,
  } Led;

  void turn_on_led(Led led, bool on);

#ifdef __cplusplus
}
#endif
