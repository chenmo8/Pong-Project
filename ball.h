/*
 * #ifndef BALL_H
 * #define FONT_H
 */

#include <stdbool.h>
#include "rand.h"
#include "gl.h"

/*
 * Functions for the ball in a game of Pong 
 * Uses graphics library to draw ball on screen
 * 
 * Author: Iddah Mlauzi <iddah&stanford.edu>
 *
 * Last updated: June 2022
 */


/*
 * This struct captures information associated with ball movement 
 * The x and y fields correspond to the current x and y positions of the center of the ball
 * The Vx and Vy fields correspind to the velocity in the x and y directions respectively
 * The radius is the radius of the ball
 */
typedef struct {
    int x, y;
    int vx, vy;
    int radius;
} ball_t;


/*
 *  'ball_init' : Required for initialized ball
 *
 *  Resets the ball for start of new game
 *  Ball starts at center
 *  Ball will be ready to move off with a random speed and direction 
 *  Ball should have an equal chance of moving off to either left or right paddle  
 *  The angle of movement is contricted to an absolute value of pi/4 radians 
 */

ball_t ball_init(void);

/* 
 *  'reset_ball'
 *   
 *   Resets ball
 *   Draws ball in center, ready for start of a new game
 *
 *   @param x   the x location of the center of the ball
 *   @param y   the y location of the center of the ball
 *
 *   @return    initialised ball
 */

ball_t reset_ball(void);

/*
 *  'draw_ball'
 *
 *  Draws a filled ball at location x, y
 *  Ball will have radius r and color c, these are set when initiating the ball
 *  ALl pixels in the ball that lie within the bounds of the framebuffer are drawn 
 *  Any pixel that lies outside is clipped (i.e. not drawn)
 *
 *  @return   Ball at starting position
 */

void draw_ball(int x, int y);

/*
 *  'clear_ball'
 *
 *  Erases the ball at the given x, y location to background color 
 *
 *  @param x   the x location of the center of the ball
 *  @param y   the y location of the center of the ball
 */

void clear_ball(int x, int y);

//I might move these functions to the game to update there instead 
//the idea is to update the ball's x and y positions depending if the ball hit's the edges 

/*
 *  'check_ball_edges'
 *
 *  Checks if the ball has hit the top or bottom edges 
 *  Updates ball's x, y and speed values to reflect this 
 *
 *  @param ball  Current ball
 *  @return      New ball with updated values
 */

ball_t check_ball_edges(ball_t ball); 

/*
 *  'check_ball_hit_right'
 *
 *  Checks if the ball has hit the right edge of the screen 
 *
 *  @return     true if the ball hit the right edge, false otherwise
 */

bool ball_check_hit_right(ball_t ball);

/*
 *  'check_ball_hit_right'
 *
 *  Checks if the ball has hit the right edge of the screen 
 *
 *  @return     true if the ball hit the right edge, false otherwise
 */

bool ball_check_hit_left(ball_t ball);

//I would need to know where the padle is or to know the color of the paddle.

bool ball_check_hit_right_paddle(ball_t ball);

bool ball_check_hit_left_paddle(ball_t ball);

