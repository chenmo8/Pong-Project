/*
#include "ball.h"
#include "gl.h"
#include "uart.h"
#include "mymodule.h"
#include "timer.h"

int width = 640;
int height = 512;
color_t BALL = GL_RED;
color_t BACKGROUND = GL_WHITE;

void main(void)

{
    uart_init();

    //initialise graphics library
    gl_init(width, height, GL_SINGLEBUFFER);
    gl_clear(GL_WHITE); //background is white

    //ball starts in center
    ball_t ball = reset_ball();

    while (1) {

        draw_ball(ball.x, ball.y);
	timer_delay_ms(10);
	int cur_y = ball.y;
	int cur_x = ball.x;
	ball.x += ball.vx;
	ball.y += ball.vy;
	ball = check_ball_edges(ball);
	if (ball_check_hit_right(ball)) {
	    ball.vx *= -1;
	}
	if (ball_check_hit_left(ball)) {
	    ball.vx *= -1;
	}
	clear_ball(cur_x, cur_y);
    }

}
*/

