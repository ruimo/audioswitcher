#define RELAY_WAIT 200

typedef enum {
    SwitchInit, In0, In1,
} SwitchStateId;

typedef struct {
    SwitchStateId id;
    union {
        struct {
            uint16_t relay_wait;
        } In;
    } StateData;
} SwitchState;

extern SwitchState switch_state;

#ifdef __cplusplus
extern "C" {
#endif

  extern void tick_switch(void);

  extern void enter_switch_init(void);
  extern void on_switch_init_state(void);
  
  extern void enter_switch_in_0(void);
  extern void on_switch_in_0(void);
  
  extern void enter_switch_in_1(void);
  extern void on_switch_in_1(void);
  
#ifdef __cplusplus
}
#endif
