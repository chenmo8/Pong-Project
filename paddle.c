#include "gl.h"
#include "printf.h"
#include "paddle.h"
#include <stdbool.h>
#include "timer.h"

extern int SCREENWIDTH;
extern int SCREENHEIGHT;
extern color_t PADDLE;             //color of paddle
extern color_t BACKGROUND;         //color of background 

static const int PAD_WIDTH = 10;    //width of paddle
static const int PAD_HEIGHT = 100;  //height of paddle

paddle_object create_paddle(int player)
{

	paddle_object paddle; 
	paddle.y = SCREENHEIGHT / 2;    
        paddle.width = PAD_WIDTH;
        paddle.height = PAD_HEIGHT;

	//player 1 paddle on left side 
	if (player % 2 == 1){
		paddle.x = PAD_WIDTH;
	}
	//player 2 paddle on right side 
	else
	{
		paddle.x = SCREENWIDTH - PAD_WIDTH - 1;
	}

	draw_paddle(paddle);

	return paddle;
}

void draw_paddle(paddle_object paddle)
{
        
	//draw a paddle centered at current paddle x and y 
	int top_left_x = paddle.x - PAD_WIDTH/2;
	int top_left_y = paddle.y - PAD_HEIGHT/2;

	gl_draw_rect(top_left_x, top_left_y, PAD_WIDTH, PAD_HEIGHT, PADDLE);
}

void clear_paddle(paddle_object cur_paddle, paddle_object prev_paddle)
{        
	//height of rectangle to clear
	int height = cur_paddle.y - prev_paddle.y;
        int top_left_x, top_left_y;
	
	//paddle moved down
	if (prev_paddle.y < cur_paddle.y) {
	    top_left_x = prev_paddle.x - PAD_WIDTH/2;
	    top_left_y = prev_paddle.y - PAD_HEIGHT/2;
	}
	
	
	//paddle moved up 
	else if (prev_paddle.y > cur_paddle.y) {
	    top_left_x = cur_paddle.x - PAD_WIDTH/2;
	    top_left_y = cur_paddle.y + PAD_HEIGHT/2;
	    height *= -1;
	}
	
	else {
	    return;
	}

	gl_draw_rect(top_left_x, top_left_y, PAD_WIDTH, height, BACKGROUND);
}

void clear_paddle2(paddle_object paddle)
{        
	//clears the  paddle centered at current paddle x and y 
	int top_left_x = paddle.x - PAD_WIDTH/2;
	int top_left_y = paddle.y - PAD_HEIGHT/2;

	gl_draw_rect(top_left_x, top_left_y, PAD_WIDTH, PAD_HEIGHT, BACKGROUND);
}

bool check_paddle_top(paddle_object paddle)
{
	if (paddle.y < PAD_HEIGHT/2) {
	    return true;
	}
	return false;
}

bool check_paddle_bottom(paddle_object paddle) 
{
	if (paddle.y > (SCREENHEIGHT - (PAD_HEIGHT/2))) {
	    return true;
	}
	return false;
}


