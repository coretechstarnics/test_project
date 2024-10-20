#include "main.h"

#define MAX_TIMERS        	4

timers timer_block[MAX_TIMERS];

// ==================================================================================
void update_timers (void)
{
	uint8_t loop;

  for (loop = 0; loop < MAX_TIMERS; loop++)
  {
    if (timer_block[loop].timer_state == TIMER_RUNNING)
    {
      if (timer_block[loop].timer_value){ // if timer value is non-zero
        if (!--timer_block[loop].timer_value)
          timer_block[loop].timer_state = TIMER_TIMEOUT;
      }
    }
  }
}

// ==================================================================================
void load_timer (uint8_t timer_id, uint32_t timer_val){
  timer_block[timer_id].timer_value = timer_val;
  start_timer(timer_id);
}
// ==================================================================================
char check_timer (uint8_t timer_id){
  return (timer_block[timer_id].timer_state);
}
// ==================================================================================
void start_timer (uint8_t timer_id){
  timer_block[timer_id].timer_state = TIMER_RUNNING;
}
// ==================================================================================
void stop_timer (uint8_t timer_id){
  timer_block[timer_id].timer_state = TIMER_STOPPED;
}
// ==================================================================================
void reset_timer (uint8_t timer_id){
  timer_block[timer_id].timer_state = TIMER_IDLE;
  timer_block[timer_id].timer_value = 0;
}

