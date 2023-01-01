#include <xc.h>
#include <stdbool.h> 

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum {
    Coil0, Coil1,
  } Coil;

  void turn_on_relay(Coil coil, bool on);

  bool in_0_pressed(void);

  bool in_1_pressed(void);

#ifdef __cplusplus
}
#endif
