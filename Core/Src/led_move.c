
#include "main.h"
#include "led_conf.h"

/**
 * @brief  Initializes LED strip colors and position for animation
 * @details This function performs two main tasks:
 *          1. Sets the initial position based on the animation direction
 *          2. Fills the entire LED strip with the original color
 *          This is typically called before starting a new animation sequence.
 *
 * @param   rLed    Pointer to an array of rgb_color structures representing the LED strip
 *                  Each element contains RGB values for one LED
 * 
 * @param   lLed    Pointer to LED configuration structure containing:
 *                  - sDir:      Direction of animation (SHIFT_LEFT or SHIFT_RIGHT)
 *                  - wPosCurr:  Current position (will be initialized)
 *                  - wPosStart: First LED position in the animation range
 *                  - wPosEnd:   Last LED position in the animation range
 *                  - rColorOri: Original/background color for all LEDs
 *
 * Example Usage:
 * @code
 * // Initialize LED configuration
 * LedTypeDef myLed = {
 *     .wPosStart = 0,               // First LED position
 *     .wPosEnd = LED_COUNT - 1,     // Last LED position
 *     .rColorOri = {0, 0, 32},      // Dim blue as background
 *     .sDir = SHIFT_LEFT            // Animation direction
 * };
 * 
 * // LED strip array
 * rgb_color ledStrip[LED_COUNT];
 * 
 * // Initialize LED strip
 * led_color_init(ledStrip, &myLed);
 * 
 * // After initialization:
 * // - All LEDs will be set to dim blue (rColorOri)
 * // - Current position (wPosCurr) will be set to wPosStart for SHIFT_LEFT
 * //   or wPosEnd for SHIFT_RIGHT
 * @endcode
 *
 * @note    The function assumes:
 *          - Valid pointers to rLed and lLed
 *          - wPosStart <= wPosEnd
 *          - Sufficient memory allocated for LED array
 */

void led_color_init(rgb_color* rLed, LedTypeDef *lLed)
{
	if (lLed->sDir == SHIFT_LEFT)                     // Check if Direction is SHIFT TO LEFT 
		lLed->wPosCurr = lLed->wPosStart;             // Change Current position with first position
	else
		lLed->wPosCurr = lLed->wPosEnd;               // Change Current position with last position

		
	for (int i=lLed->wPosStart ; i < lLed->wPosEnd +1 ; i++)
		  rLed[i] = lLed->rColorOri;                  // Fill all led from start to end with original color
}

//	************************************************************ SHIFT FROM SMALL NUMBER TO BIG NUMBER LED  SHIFTING 1 LED ************************************************************
/**
 * @brief  Shifts LED colors to the left in an LED strip/array
 * @details This function implements a left-shifting animation effect for LED strips.
 *          It moves a 'running' color through the LED array from left to right,
 *          restoring the original color in its wake. When it reaches the end position,
 *          it can either reset to the start position or stop based on the fOrigin parameter.
 *
 * @param   rLed      Pointer to an array of rgb_color structures representing the LED strip
 *                    Each element contains RGB values for one LED
 * 
 * @param   lLed      Pointer to LED configuration structure containing:
 *                    - wPosCurr:   Current position in the LED array
 *                    - wPosStart:  Starting position (typically 0)
 *                    - wPosEnd:    Ending position (typically LED_COUNT - 1)
 *                    - rColorFill: Color used for the moving LED ('running' color)
 *                    - rColorOri:  Original color to restore after LED passes
 *                    - sDir:       Direction of shift (updated to SHIFT_LEFT)
 *
 * @param   fOrigin   Flag to control behavior when reaching end position:
 *                    - SET:   Reset position to wPosStart and continue
 *                    - RESET: Stop at end position
 *
 * @return  uint8_t   Status of the shift operation:
 *                    - 0: Shift operation in progress
 *                    - 1: Shift operation complete (reached end position)
 *
 * Example Usage:
 * @code
 * // Initialize LED configuration
 * LedTypeDef myLed = {
 *     .wPosCurr = 0,               // Start from leftmost LED
 *     .wPosStart = 0,              // Leftmost position
 *     .wPosEnd = LED_COUNT - 1,    // Rightmost position
 *     .rColorFill = {255, 0, 0},   // Running color (red)
 *     .rColorOri = {0, 0, 0},      // Original color (off)
 *     .sDir = SHIFT_LEFT
 * };
 * 
 * // LED strip array
 * rgb_color ledStrip[LED_COUNT];
 * 
 * // Run animation until complete
 * while(!led_shift_left(ledStrip, &myLed, SET)) {
 *     update_leds(ledStrip);  // Update physical LEDs
 *     delay_ms(100);          // Animation speed
 * }
 * @endcode
 *
 * @note    - Function assumes valid pointers and properly initialized structures
 *          - Array bounds checking should be done before calling this function
 *          - This function is complementary to led_shift_right()
 *          - The volatile return value ensures consistent behavior in interrupt contexts
 */
uint8_t led_shift_left(rgb_color *rLed, LedTypeDef *lLed, FlagStatus fOrigin)
{
	volatile uint8_t uOut = 0;

	if (lLed->wPosCurr <= lLed->wPosEnd) {                // if Current position less than end position
	  rLed[lLed->wPosCurr] = lLed->rColorFill;			  // Change color with Running color
		if (lLed->wPosCurr != lLed->wPosStart){			  // if Position not first
			rLed[(lLed->wPosCurr)-1] = lLed->rColorOri;	  // Change previous color with original color
		}
		lLed->wPosCurr ++;								  // Set Position to next position
	}
	else
	{
	  lLed->wPosCurr --;								  // Set Position to previous position
		rLed[lLed->wPosCurr] = lLed->rColorOri;			  // Change 1st color with original color
		lLed->sDir = SHIFT_LEFT;
		
		if (fOrigin == SET)
			lLed->wPosCurr = lLed->wPosStart;			// Reset Current Position to Start Position

  	    uOut = 1;					        					                // Set flag to complete
	}
	return uOut;
}

// ************************ SHIFT FROM BIG NUMBER TO SMALL NUMBER FOR LED  SHIFTING 1 LED ************************************************************
/* @brief  Shifts LED colors to the right in an LED strip/array
 * @details This function implements a right-shifting animation effect for LED strips.
 *          It moves a 'running' color through the LED array from right to left,
 *          restoring the original color in its wake. When it reaches the start position,
 *          it can either reset to the end position or stop based on the fOrigin parameter.
 *
 * @param   rLed      Pointer to an array of rgb_color structures representing the LED strip
 *                    Each element contains RGB values for one LED
 * 
 * @param   lLed      Pointer to LED configuration structure containing:
 *                    - wPosCurr:   Current position in the LED array
 *                    - wPosStart:  Starting position for the shift effect
 *                    - wPosEnd:    Ending position for the shift effect
 *                    - rColorFill: Color used for the moving LED ('running' color)
 *                    - rColorOri:  Original color to restore after LED passes
 *                    - sDir:       Direction of shift (updated by function)
 *
 * @param   fOrigin   Flag to control behavior when reaching start position:
 *                    - SET:   Reset position to wPosEnd and continue
 *                    - RESET: Stop at start position
 *
 * @return  uint8_t   Status of the shift operation:
 *                    - 0: Shift operation in progress
 *                    - 1: Shift operation complete (reached start position)
 *
 * @note    The function assumes valid pointers and properly initialized structures.
 *          Array bounds checking should be done before calling this function.
 *
 * Example Usage:
 * @code
 * // Initialize LED configuration
 * LedTypeDef myLed = {
 *     .wPosCurr = LED_COUNT - 1,    // Start from rightmost LED
 *     .wPosStart = 0,               // Leftmost position
 *     .wPosEnd = LED_COUNT - 1,     // Rightmost position
 *     .rColorFill = {255, 0, 0},    // Running color (red)
 *     .rColorOri = {0, 0, 0},       // Original color (off)
 *     .sDir = SHIFT_RIGHT
 * };
 * 
 * // LED strip array
 * rgb_color ledStrip[LED_COUNT];
 * 
 * // Run animation until complete
 * while(!led_shift_right(ledStrip, &myLed, SET)) {
 *     update_leds(ledStrip);  // Update physical LEDs
 *     delay_ms(100);          // Animation speed
 * }
 * @endcode
 */
uint8_t led_shift_right (rgb_color *rLed, LedTypeDef *lLed, FlagStatus fOrigin)
{
	uint8_t uOut = 0;

	if (lLed->wPosCurr >= lLed->wPosStart)					    // Check if current position is within valid range 
	{
		rLed[lLed->wPosCurr] = lLed->rColorFill;				// Update current LED with running color
		if (lLed->wPosCurr != lLed->wPosEnd)			        // if Position not last
			rLed[(lLed->wPosCurr)+1] = lLed->rColorOri;		    // Change previous color with original color
	  lLed->wPosCurr --;										// Decrement Position / move to next position
	}
	else
	{
		lLed->wPosCurr ++;										// Reset posistion
		rLed[lLed->wPosCurr] = lLed->rColorOri;			        // Change 1ast color with original color

		if (fOrigin == SET)                                     // If origin flag is set, reset position to end 
			lLed->wPosCurr = lLed->wPosEnd;						// Re INIT Position

		lLed->sDir = SHIFT_RIGHT;                               // update direction indicator
		uOut = 1;					        					// Set flag to complete

	}
	return uOut;
}


// ********************************************************









//	************************************************************ ROTATE LEFT CCW
/**
 * @brief  Rotates LED colors counter-clockwise (left) in an LED strip/array
 * @details This function performs a circular rotation of LED colors to the left, where:
 *          1. First LED color is temporarily stored
 *          2. All other LEDs shift one position left
 *          3. First LED color moves to the last position
 *          This creates a continuous rotation effect without losing any colors.
 *
 * @param   rLed      Pointer to an array of rgb_color structures representing the LED strip
 *                    Each element contains RGB values for one LED
 * 
 * @param   lLed      Pointer to LED configuration structure containing:
 *                    - wPosStart: First LED position in the rotation range
 *                    - wPosEnd:   Last LED position (exclusive) in the rotation range
 *
 * @param   fOrigin   Reserved for future use (maintaining consistent interface with other functions)
 *
 * Example Usage:
 * @code
 * // Initialize LED configuration
 * LedTypeDef myLed = {
 *     .wPosStart = 0,              // First LED position
 *     .wPosEnd = LED_COUNT,        // Last LED position
 *     .rColorFill = {255, 0, 0},   // Not used in rotation
 *     .rColorOri = {0, 0, 0}       // Not used in rotation
 * };
 * 
 * // LED strip array initialization with different colors
 * rgb_color ledStrip[LED_COUNT] = {
 *     {255, 0, 0},    // Red
 *     {0, 255, 0},    // Green
 *     {0, 0, 255},    // Blue
 *     // ... more LEDs ...
 * };
 * 
 * // Rotate colors CCW
 * led_rotate_left(ledStrip, &myLed, SET);
 * // Result after one rotation:
 * // [Red, Green, Blue] -> [Green, Blue, Red]
 * 
 * // For continuous rotation:
 * while(1) {
 *     led_rotate_left(ledStrip, &myLed, SET);
 *     update_leds(ledStrip);    // Update physical LEDs
 *     delay_ms(100);            // Control rotation speed
 * }
 * @endcode
 *
 * @note    - Function performs a true rotation (no colors are lost)
 *          - All colors move one position left, with wraparound
 *          - Range is from wPosStart to (wPosEnd - 1)
 *          - Requires enough memory for temporary color storage
 *          - No bounds checking is performed - ensure valid range
 *          - Different from shift functions as it preserves all colors
 */
void led_rotate_left(rgb_color *rLed, LedTypeDef *lLed) 
{
    rgb_color rTemp;
    
    // Save last LED color into temporary variable
    rTemp = rLed[lLed->wPosStart]; // Save first LED to Temporary

    // Shift each LED to the left
    for (int i = lLed->wPosStart; i < lLed->wPosEnd; i++) {
        if (i < lLed->wPosEnd - 1) { // Shift only until the second to last LED
            rLed[i] = rLed[i + 1]; // Move LED color to the left
        }
    }
    // Place the saved color into the last position
    rLed[lLed->wPosEnd - 1] = rTemp; // Place original first LED color to the last position
}

/*
//	************************************************************ ROTATE RIGHT CW


*/

/**
 * @brief   Rotates LED colors one position to the right within a specified LED strip segment
 * @details This function performs a right rotation of LED colors in a defined segment
 *          of an LED strip. The last LED's color in the segment is moved to the first
 *          position, while all other LEDs shift one position to the right.
 *
 * @param   rLed    Pointer to an array of rgb_color structures containing LED colors
 * @param   lLed    Pointer to LED configuration structure containing:
 *                  - wPosStart: Starting position of the LED segment
 *                  - wPosEnd:   Ending position of the LED segment (exclusive)
 *
 * @note    The function assumes:
 *          - Valid memory allocation for the LED array
 *          - wPosEnd > wPosStart
 *          - Both wPosStart and wPosEnd are within the array bounds
 *
 * Example usage:
 * @code
 *     rgb_color ledStrip[10];    // Array of LED colors
 *     LedTypeDef ledConfig = {
 *         .wPosStart = 0,        // Start at first LED
 *         .wPosEnd = 5           // End at fifth LED
 *     };
 *     led_rotate_right(ledStrip, &ledConfig);
 * @endcode
 */
void led_rotate_right(rgb_color *rLed, LedTypeDef *lLed)  
{
  rgb_color rTemp;

    // Save the last LED color into a temporary variable
  rTemp = rLed[lLed->wPosEnd - 1]; // Save the last LED color

    // Shift each LED to the left (from end to start)
  for (int i = lLed->wPosEnd - 1; i > lLed->wPosStart; i--) 
	{
        rLed[i] = rLed[i - 1]; // Move LED color to the right
  }

    // Place the saved color into the start position
  rLed[lLed->wPosStart] = rTemp; // Place original last LED color to the first position
}




/**
 * @brief Shifts LED colors left across an array of LEDs for a specified number of positions.
 * 
 * This function shifts the colors of an array of RGB LEDs to the left by a specified number (`uNum`).
 * The function updates the LED colors based on the current position (`wPosCurr`) and the defined range
 * (`wPosStart` to `wPosEnd`) in the LedTypeDef structure. When the LED position reaches the end, 
 * it resets the position depending on the value of `fOrigin`.
 *
 * @param rLed         Pointer to the array of RGB LEDs (`rgb_color`). The LED colors will be modified in this array.
 * @param lLed         Pointer to a LedTypeDef structure that holds LED positioning and color information.
 * @param fOrigin      Flag to determine whether to reset the position to the start or end of the range when the 
 *                     shifting reaches the end. Can be SET (reset to start) or RESET (reset to end).
 * @param uNum         Number of positions to shift. If `uNum` is 0, it defaults to 1.
 *
 * @return uint8_t     Returns 1 when the shifting completes and resets the position, otherwise returns 0.
 */
uint8_t led_shift_left_num(rgb_color *rLed, LedTypeDef *lLed, FlagStatus fOrigin, uint8_t uNum)
{
    uint8_t uOut = 0;          // Output flag indicating whether the shifting is complete
    int i;                     // Loop counter
    uint16_t vCal;             // Variable for position calculation
    rgb_color rTemp = lLed->rColorOri; // Temporary storage for the original color

    // Default to 1 shift if uNum is 0
    if (uNum == 0)
        uNum = 1;

		if (lLed->sDir != SHIFT_LEFT)
		{
			lLed->sDir = SHIFT_LEFT;
			lLed->wPosCurr = lLed->wPosStart;
		}

		
    // Check if the current position is less than or equal to the end position
    if (lLed->wPosCurr <= lLed->wPosEnd)
    {
        // Loop to shift LED colors to the left by uNum positions
        for (i = 0; i < uNum; i++)
        {
            vCal = (lLed->wPosCurr) + i;  // Calculate the next position to shift to
            if (vCal <= lLed->wPosEnd)    // Ensure the calculated position is within bounds
                rLed[vCal] = lLed->rColorFill; // Set the new color (rColorFill) at the calculated position
        }

        // Restore previous LED colors if not at the first position
        if ((lLed->wPosCurr) != lLed->wPosStart)
        {
            for (i = 0; i < uNum; i++)
                rLed[lLed->wPosCurr - (uNum - i)] = rTemp; // Reset previous LED color to original (rColorOri)
        }

        // Update the current position after shifting
        lLed->wPosCurr += uNum;
    }
    else // If the current position exceeds the end position
    {
        // Reset the color of the last shifted LEDs to their original color
        for (i = 0; i < uNum; i++)
        {
            vCal = lLed->wPosCurr - (uNum - i);
            if (vCal <= lLed->wPosEnd) // Ensure the position is within bounds
                rLed[vCal] = rTemp;    // Restore the original color
        }

        // Set current position to the end and mark shifting as complete
        lLed->wPosCurr = lLed->wPosEnd;
        uOut = 1;  // Indicate that the shifting is done

        // Set the direction of the shift to left (SHIFT_LEFT)
        lLed->sDir = SHIFT_LEFT;

        // Re-initialize the current position based on the fOrigin flag
        if (fOrigin == SET)
            lLed->wPosCurr = lLed->wPosStart; // Reset to start position
        else
            lLed->wPosCurr = lLed->wPosEnd;   // Reset to end position
    }

    return uOut; // Return status (0 if still shifting, 1 if complete)
}

/**
 * @brief Shifts LED colors to the right across an array of LEDs for a specified number of positions.
 * 
 * This function shifts the colors of an array of RGB LEDs to the right by a specified number (`uNum`).
 * It updates the LED colors based on the current position (`wPosCurr`) and the defined range (`wPosStart` 
 * to `wPosEnd`) in the `LedTypeDef` structure. When the position reaches the start (`wPosStart`), 
 * the function resets the position based on the value of `fOrigin`. 
 * 
 * It also handles edge cases like when the shift number (`uNum`) is `0`, defaulting to `1`, 
 * and ensures that the function doesn't access out-of-bound indices.
 *
 * @param rLed     Pointer to the array of RGB LEDs (`rgb_color`). The function modifies the colors in this array.
 * @param lLed     Pointer to a `LedTypeDef` structure that holds the LED positioning and color information.
 *                 It includes the current position (`wPosCurr`), start (`wPosStart`), end (`wPosEnd`), 
 *                 the color to fill (`rColorFill`), and the original color (`rColorOri`).
 * @param fOrigin  Flag that determines whether to reset the position to the start (`SET`) or leave it at the end (`RESET`)
 *                 when the shifting is complete.
 * @param uNum     Number of positions to shift. If `uNum` is `0`, it defaults to `1`.
 *
 * @return uint8_t Returns `1` when the shifting completes and resets the position, otherwise returns `0`.
 */
uint8_t led_shift_right_num(rgb_color *rLed, LedTypeDef *lLed, FlagStatus fOrigin, uint8_t uNum)
{
    uint8_t uOut = 0;  // Output flag indicating whether the shifting is complete
    int i;             // Loop counter
    uint16_t vCal;     // Variable for position calculation
    rgb_color rTemp = lLed->rColorOri;  // Temporary storage for the original color

    // Default to 1 shift if uNum is 0
    if (uNum == 0)
        uNum = 1;
		
		if (lLed->sDir != SHIFT_RIGHT)
		{
			lLed->sDir = SHIFT_RIGHT;
			lLed->wPosCurr = lLed->wPosEnd;
		}

    // Check if the current position is greater than or equal to the start position
    if (lLed->wPosCurr >= lLed->wPosStart)
    {
        // Loop to shift LED colors to the right by uNum positions
        for (i = 0; i < uNum; i++)
        {
            vCal = lLed->wPosCurr - i;  // Calculate the next position to shift to (decremented)
            if (vCal >= lLed->wPosStart)  // Ensure the position is within bounds (>= start)
                rLed[vCal] = lLed->rColorFill;  // Set the new color at the calculated position
        }

        // Restore the previous LED colors if not at the last position
        if (lLed->wPosCurr != lLed->wPosEnd)
        {
            for (i = 0; i < uNum; i++)
                rLed[lLed->wPosCurr + (uNum - i)] = rTemp;  // Reset previous LED color to original
        }

        // Decrement the current position after shifting
        lLed->wPosCurr -= uNum;
    }
    else // If the current position is less than or equal to the start position
    {
        // Reset the color of the last shifted LEDs to their original color
        for (i = 0; i < uNum; i++)
        {
            vCal = lLed->wPosCurr + (uNum - i);
            if (vCal >= lLed->wPosStart)  // Ensure the position is within bounds
                rLed[vCal] = rTemp;       // Restore the original color
        }

        // Set the current position to the start
        lLed->wPosCurr = lLed->wPosStart;

        // Mark the shifting as complete
        uOut = 1;

        // Set the direction of the shift to right (SHIFT_RIGHT)
        lLed->sDir = SHIFT_RIGHT;

        // Re-initialize the position based on the fOrigin flag
        if (fOrigin == SET)
            lLed->wPosCurr = lLed->wPosEnd;  // Reset to end position
    }

    return uOut;  // Return the status (0 if still shifting, 1 if complete)
}








