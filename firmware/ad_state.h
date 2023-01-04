#include <stdint.h>

#define AD_WAIT 5000
#define AD_THRESHOLD 200

extern uint16_t ad0_value;
extern uint16_t ad1_value;

typedef enum {
  AdStateWait, AdState0, AdState1,
} AdStateId;

typedef struct {
  AdStateId id;
  union {
    struct {
      uint16_t wait;
    } Wait;
  } StateData;
} AdState;

extern AdState ad_state;

#ifdef __cplusplus
extern "C" {
#endif

  extern void tick_ad(void);
  
  extern void enter_ad_wait(void);
  extern void on_ad_wait_state(void);

  extern void enter_ad_0(void);
  extern void on_ad_0(void);

  extern void enter_ad_1(void);
  extern void on_ad_1(void);

  extern void tx_value(uint16_t value);

#ifdef __cplusplus
}
#endif


  
