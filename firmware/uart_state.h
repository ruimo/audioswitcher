#include <stdbool.h>

typedef enum {
    UartStateInit, UartStateReady,
} UartStateId;

typedef struct {
    UartStateId id;
} UartState;

extern UartState uart_state;

#ifdef __cplusplus
extern "C" {
#endif

  extern uint8_t buffer_len(void);
  extern bool add_to_tx_buffer(uint8_t c);
  extern int16_t remove_from_tx_buffer(void);

  extern void enter_uart_init(void);
  extern void tick_uart(void);

#ifdef __cplusplus
}
#endif
