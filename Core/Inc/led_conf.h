#ifndef _LED_CONF_H_
#define _LED_CONF_H_

#include "main.h"

#define MAX_NUMB    80



#define NUM_START		0
#define NUM_END 		MAX_NUMB - 1

#define NUM_COR     0

// ================================ PORT DEFINE


// ================= Color table
#define COLOR_BLANK   (rgb_color){0x00, 0x00, 0x00}
#define COLOR_RED     (rgb_color){0x20, 0x00, 0x00}
#define COLOR_GREEN   (rgb_color){0x00, 0x20, 0x00}
#define COLOR_BLUE    (rgb_color){0x00, 0x00, 0x20}
#define COLOR_ORANGE  (rgb_color){0x6F, 0x20, 0x00}

#define COLOR_WHITE   (rgb_color){0x20, 0x20, 0x20}
#define COLOR_CYAN    (rgb_color){0x00, 0x19, 0x2B}
#define COLOR_PURPLE  (rgb_color){0x20, 0x00, 0x20}
#define COLOR_RED1    (rgb_color){0x40, 0x00, 0x00}
#define COLOR_GREEN1  (rgb_color){0x00, 0x40, 0x00}

#define COLOR_BLUE1   (rgb_color){0x00, 0x18, 0x56}
#define COLOR_ORANGE1 (rgb_color){0x6F, 0x20, 0x00}
#define COLOR_SOFT    (rgb_color){0x08, 0x08, 0x08}
#define COLOR_YELLOW  (rgb_color){0x20, 0x20, 0x00}

// =================================
typedef enum {BUSY = 0, NOT_BUSY = !BUSY}                    FunctionalBusy;
typedef enum {NO_SHIFT = 0, SHIFT_LEFT = 1, SHIFT_RIGHT = 2} StateDir;                        // Left = small to big number
typedef enum {SINGLE = 0, DOUBLE = !SINGLE}                  StateDirInst;                    // Left = small to big number
typedef enum {MODE_STOP = 0, MODE_RUN = !MODE_STOP}          StateMode;


typedef enum {LED_END = 0, LED_RUN = !LED_END} FunctionalLed;
typedef enum {POS_TOP = 0, POS_BOTTOM = !POS_TOP} StateLed;

typedef enum {FIRST_COND = 0, NORMAL_COND = !FIRST_COND} StateCondition;

typedef enum {MODE_SLOW = 0, MODE_FAST = !MODE_SLOW} StateCircle;


typedef struct rgb_color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} rgb_color;

typedef struct
{
	volatile int16_t wPosStart;             // Start Positiion
	volatile int16_t wPosEnd;               // End Posistion
	volatile int16_t wPosCurr;              // Current Position
	rgb_color rColorOri;        // Original Color
	rgb_color rColorFill;       // Filled color
	StateDir  sDir;             // State Direction 
} LedTypeDef;					        // To determine LEDs position                                                                             // Led Properties





#endif
