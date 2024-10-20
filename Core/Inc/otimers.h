#ifndef _OTIMERS_H_
#define _OTIMERS_H_

#include "main.h"


// ========== TIMERS



// ============= Timer Name ===============
//#define TIMER_SER			0

// ============= Timer Perioda ===============

typedef enum
{ 	TIMER_STOPPED,
	TIMER_RUNNING,
	TIMER_TIMEOUT,
	TIMER_IDLE
}TM_ModeDef;



// ================ Times variable declaration ==================
  typedef struct
  {
	  uint32_t timer_value; // counts miliseconds
	  TM_ModeDef timer_state;
  } timers;



//extern  timers timer_block[];


// =============== Timers functions declaration ======================

void update_timers            (void);
void load_timer               (uint8_t, uint32_t);
char check_timer              (uint8_t);
void start_timer              (uint8_t);
void stop_timer               (uint8_t);
void reset_timer              (uint8_t);



#endif
