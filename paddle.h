//#ifndef PADDLE_H     


#include "gl.h"
#include <stdbool.h>

/*
 * Functions for the paddle in a game of Pong 
 * Uses graphics library to draw paddle on screen
 * 
 * Authors:
 * Connie Hong  <conniehg@stanford.edu> 
 * Iddah Mlauzi <iddah&stanford.edu>
 *
 * Last updated: June 2022
 */


/*
 * This struct captures information associated with paddle movement 
 * The x and y fields correspond to the current x and y positions 
 * width and hei9ght fiel;ds correspond to the width and height of the paddle
 * of the center of the paddle
 */

typedef struct{

        int x, y;
	int width, height;

} paddle_object;


/*
 *  'create_paddle'
 *
 *  Creates a new paddle for the given player
 *  Paddle starts at center of player's side 
 *
 *  @param  player  Player number (1 or 2)
 *  @return         New paddle
 */

paddle_object create_paddle(int player);

/*
 *  'draw_paddle'
 *
 *  Draws a paddle with center at x, y
 *
 *  @param  paddle  paddle object to draw 
 */

void draw_paddle(paddle_object paddle);

/*
 *  'clear_paddle'
 *
 *  Clears the given paddle
 *
 *  @param paddle  paddle object to clear
 */
void clear_paddle(paddle_object cur_paddle, paddle_object prev_paddle);


/* 

* 'clear_paddle2'
*
* A simplified version of clear_paddle: clears given paddle without position of original paddle.
*/

void clear_paddle2(paddle_object paddle);
/*
 *  'check_paddle_top'
 *
 *  Checks if the current paddle has hit the top of the screen 
 *  
 *  @param  paddle  current paddle to check
 *  @return         true if paddle has hit the top, false otherwise  
 */
bool check_paddle_top(paddle_object paddle);

/*
 *  'check_paddle_bottom'
 *
 *  Checks if the current paddle has hit the bottom of the screen
 **
 *  @param  paddle  current paddle to check
 *  @return         true if paddle has hit the bottom, false otherwise
 */

bool check_paddle_bottom(paddle_object paddle);
