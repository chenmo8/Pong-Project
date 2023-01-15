#include "ball.h"
//#include "math.h"
#include "rand.h"
#include "gl.h"
#include "printf.h"
#include "sine.h"

extern int SCREENWIDTH;          //width of the screen in pixels 
extern int SCREENHEIGHT;         //height of the screen in pixels
extern color_t BALL;             //color of the ball
extern color_t BACKGROUND;       //background color 

int max_speed =4;         //maximum speed of ball in either x or y direction 

int radius = 8;            //radius of the ball

//need a helper function to geenrate a random number 
//figure out a generally non biased way to do this 
//In meantime, I'll just use mod

//generates a random number between min and max 
static int random_randrange(int min, int max) 
{
    return (rand() % (max - min + 1)) + min;
}

ball_t ball_init(void)
{
    ball_t ball;  

    //place ball at center 
    ball.x = SCREENWIDTH / 2;
    ball.y = SCREENHEIGHT / 2;

    
    //int angle = random_randrange(0, 45);

    ball.vx = max_speed;
    ball.vy = max_speed + 1;
    ball.radius = radius;

    return ball;
}

ball_t reset_ball(void) 
{
    ball_t ball = ball_init();
    draw_ball(ball.x, ball.y);

    return ball;
}

void draw_ball(int x, int y) 
{
    //draws a red ball centered at x, y
    //ball is currently a square --> need to figure out circle logic
    gl_draw_rect(x - radius, y - radius, radius*2, radius*2, BALL);
}

void clear_ball(int x, int y) {
    //draws a ball the same color as backgroung, effectively clearing the ball
    gl_draw_rect(x - radius, y - radius, radius*2, radius*2, BACKGROUND);
}

ball_t check_ball_edges(ball_t ball) 
{
    //if ball hits top, change ball direction 
    if ((ball.y < ball.radius) || (ball.y >= (SCREENHEIGHT - ball.radius))) {
        ball.vy *= -1;
	}
    return ball;
}

bool ball_check_hit_right(ball_t ball)
{
    if (ball.x >= SCREENWIDTH - radius) return true;

    return false;
}
    

bool ball_check_hit_left(ball_t ball) 
{
    if (ball.x < radius) return true;

    return false;
}
