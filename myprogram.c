#include "sensor.h"
#include "LSM6DS33.h"

#include "pi.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "gpio_interrupts.h"

#include "ball.h"
#include "paddle.h"

#include "gl.h"
#include "uart.h"
#include "timer.h"
#include "printf.h"
#include "i2c.h"
#include "armtimer.h"
#include "strings.h"

#include "timer.h"


//This is 0x6A
//const unsigned ls6mds33_address1 = 0b1101010;
//This is 0x6b
const unsigned lsm6ds33_address2 = 0b1101011; // this is for the gyro/accel;
static const int RESETBUTTON = GPIO_PIN10;

int highestscore = 0;
int currentgamescore = 0;

int signed_to_base(char *buf, 
		size_t bufsize, 
		int val, 
		int base, 
		size_t min_width);

int SCREENWIDTH = 640;
int SCREENHEIGHT = 512;
color_t BOARDER = 0x003366;
color_t BALL = 0x00008b;
color_t PADDLE = 0x000223;
color_t BACKGROUND = 0xb7d2e8;
sensor_object_t person2;
int time_change = 3750;
int threshold = 750;
paddle_object paddle2;

//Function that draw welcome screen.
void display_welcome(void){
	gl_clear(BACKGROUND);
	gl_draw_string(215, 236, "LET'S PLAY PONG", PADDLE);
	gl_draw_rect(215, 256, 14*15, 36, PADDLE);
	gl_draw_string(222, 266, "CLICK TO START", GL_WHITE);
}

//check if the ball has hit the given paddle
bool check_hit_paddle(ball_t ball, paddle_object paddle)
{
	//check ball x and y are top layer of paddle
	int mid_height = paddle.height/2;
	int mid_width = paddle.width/2;
	int multiplier = 1;

	//if ball is moving to right
	if (ball.vx > 0) {
		multiplier = -1;;
	}
	//edge of ball touching paddle
	mid_width *= multiplier;
	int ball_side_x = ball.x - (ball.radius * multiplier);
	int ball_top_y = ball.y - ball.radius;
	int ball_bottom_y = ball.y + ball.radius;

	//if center edge of ball is on paddle
	if (gl_read_pixel(ball_side_x, ball.y) == PADDLE) {
		return true;
	}

	return false;
}

//If you've achieved a high score at the end of a game, this screen is displayed.
void display_highscore(void){

	gl_clear(BACKGROUND);

	//Since our draw_string only incorporates char/string values (and not 'raw' integer values), we utilize snprintf to write our text to a buffer before we call gl_draw_string on it.
	char urscore[50];
	memset(urscore, 0x77, sizeof(urscore));

	char points[5];
	memset(points, 0x77, sizeof(points));
	signed_to_base(points, 5, highestscore, 10, 0);
	snprintf(urscore, 50, "%s: %s!", "THE HIGHEST SCORE IS", points);
	
	gl_draw_string(117, 216, "YOU'VE ACHIEVED A HIGH SCORE!", PADDLE);
	gl_draw_string(159, 236, urscore, PADDLE);

	//gl_draw_rect(, PADDLE);
	gl_draw_rect(100, 260, 30*14+20, 36, PADDLE);
	gl_draw_string(110, 270, "CONTINUE? PRESS IN 10 SECONDS.", GL_WHITE);


}

//If you haven't achieved a high score at the end of a game, this screen is displayed. 
void display_nothighscore(void){

	gl_clear(BACKGROUND);

	char urscore[50];
	memset(urscore, 0x77, sizeof(urscore));

	char points[5];
	memset(points, 0x77, sizeof(points));
	signed_to_base(points, 5, currentgamescore, 10, 0);

	snprintf(urscore, 50, "%s: %s.", "YOU'VE SCORED", points);
	gl_draw_string(194, 216, urscore, PADDLE);

	char printhighestscore[50];
	memset(printhighestscore, 0x77, 50);

	char highscore[5];
	memset(points, 0x77, sizeof(points));
	signed_to_base(highscore, 5, highestscore, 10, 0);

	snprintf(printhighestscore, 50, "%s: %s.", "THE HIGHEST SCORE IS", highscore);
	gl_draw_string(145, 236, printhighestscore, PADDLE);

	gl_draw_rect(100, 260, 30*14+20, 36, PADDLE);
	gl_draw_string(110, 270, "CONTINUE? PRESS IN 10 SECONDS.", GL_WHITE);


}

//Helper function to calculate absolute value.
static int abs(int num) {
	if (num < 0) return -num;
	return num;
}

//handler function --> read gyroscope value 
static void handler(unsigned int pc, void *aux_data) {
	if (armtimer_check_and_clear_interrupt()) {
		short x, y, z;
		//it takes about 3650 microseconds for this function 
		lsm6ds33_read_gyroscope(&x, &y, &z, lsm6ds33_address2);

		//account for noise 
		if (abs(x) < threshold) {
			x = 0;
		}
		int angle = (int) (x * time_change/1000000);
		//Finally updates paddle position.
		paddle2.y -= angle/12;
	}
}

//Handler function for the reset-paddle-to-center-of-screen button.
void handlerreset(unsigned int pc, void *aux_data){

	if (gpio_check_and_clear_event(RESETBUTTON)){
		clear_paddle2(paddle2);
		paddle2.y = SCREENHEIGHT / 2;
		draw_paddle(paddle2);
	}

}

//displays a continue screen after a single player scores.
static void continue_screen(void)
{
	gl_clear(BACKGROUND);
	gl_draw_string(0, 0, "continue?", GL_BLACK);
	while(gpio_read(GPIO_PIN10)==1 || gpio_read(GPIO_PIN9)==1){}
}

//configures the countdown timer for the acceleromter
//operates at 1600 Hz
//so we will sample once every 625 usecs
//but handler function takes about 3650s to operate 
//so we will need to sample once every 6 samples 
//which is every 3750 seconds 
static void configure_timer() {
	//armtimer configures for an interval of 625 usecs between events
	armtimer_init(time_change);
	armtimer_enable();            //enable timer itself
	armtimer_enable_interrupts(); //receive timer events as interrupts
	interrupts_enable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ); //Enable armtimer interrupts      
	interrupts_register_handler(INTERRUPTS_BASIC_ARM_TIMER_IRQ, handler , NULL);
}

void main(void)

{

	uart_init();
	timer_init();
	gpio_init();

	//initialise graphics library
	i2c_init();

	gl_init(SCREENWIDTH, SCREENHEIGHT, GL_SINGLEBUFFER);

	gpio_set_function(GPIO_PIN10, GPIO_FUNC_INPUT);

	//While waiting for start-button click, displays welcome screen.
	display_welcome();
	while(gpio_read(GPIO_PIN10)==1){}

	gl_clear(BACKGROUND);

	const char *aux_data = "Button has interrupt mojo lmao.\n";

	/* Once the game starts, switches our previous start button (PIN 10) to an interrupts(async-falling-edge) enabled pin.*/
	gpio_set_input(RESETBUTTON);
	gpio_set_pullup(RESETBUTTON);

	//Enables our reset handler to fire when button is pressed.*/
	gpio_enable_event_detection(RESETBUTTON, GPIO_DETECT_ASYNC_FALLING_EDGE);

	//Allows for interrupts to start reading.
	interrupts_init();
	gpio_interrupts_init();
	interrupts_register_handler(INTERRUPTS_GPIO3, handlerreset, (void *)aux_data);
	gpio_interrupts_enable();
	interrupts_global_enable();

	//Initialize sensor object under person2.
	person2 = sensor_init(lsm6ds33_address2);
	//ball starts in center
	ball_t ball = reset_ball();

	//creates two paddles for player 1 and 2
	paddle_object paddle1 = create_paddle(1);
	paddle2 = create_paddle(2);
	configure_timer();

	while (1) {

		//FIRST, draw game objects.
		draw_ball(ball.x, ball.y);
		draw_paddle(paddle1);
		draw_paddle(paddle2);

		//NEXT, current positions of ball.
		//Since the right paddle is handled by the interrupt-handler function, we do not explicit change its position here.
		ball_t cur_ball = ball;
		paddle_object cur_pad1 = paddle1;
		paddle_object cur_pad2 = paddle2;

		//move ball
		ball.x += ball.vx;
		ball.y += ball.vy;
		ball = check_ball_edges(ball);

		//THIRD, execute game play.
		//If the ball has hit the right of the screen, the game has ended for you. 
		if (ball_check_hit_right(ball)) {
			//Stop all interrupt "listeners."	
			interrupts_global_disable();
			//If you've scored a high score, display high score screen.
			if (currentgamescore > highestscore){
				highestscore = currentgamescore;
				display_highscore();

				int notclicked = 0;
				int i=0;

				//Timer to display the continue screen: if no button is pressed in about 10 seconds, exit the game.
				while (notclicked == 0 && i<20000000){
					if (gpio_read(GPIO_PIN10)==0){
						//If button is clicked, reset the game (but keep the high score).
						currentgamescore = 0;
						ball.vx = 4;
						ball.vy = 4;
						interrupts_global_enable();

						ball = reset_ball();
						gl_clear(BACKGROUND);
						timer_delay_ms(1000);
						notclicked = 1;
					}
					i++;
				}

				if (notclicked == 0){ 
                                        gl_clear(BACKGROUND);
                                        gl_draw_string(222, 236, "SEE YOU LATER!", PADDLE);
                                        timer_delay_ms(1000);   
                                        uart_putchar(EOT);
                                        pi_reboot();
                                }

			}else {
				//The following is what happens if you don't achieve a high score.
				display_nothighscore();
				int notclicked = 0;
				int i=0;

				//Same logic as above for continue screen.
				while (notclicked == 0 && i <20000000){
					if (gpio_read(GPIO_PIN10)==0){

						currentgamescore = 0;
						ball.vx = 4;
						ball.vy = 4;
						interrupts_global_enable();

						ball = reset_ball();
						gl_clear(BACKGROUND);
						timer_delay_ms(1000);
						notclicked = 1; 
					}
					i++;   
				}
				if (notclicked == 0){ 
					gl_clear(BACKGROUND);
					gl_draw_string(222, 236, "SEE YOU LATER!", PADDLE);
					timer_delay_ms(1000);	
					uart_putchar(EOT);
					pi_reboot();
				}	
			}

		}
		
		/* For if the computer-automated ball ever loses, which, should never happen.
		
		if (ball_check_hit_left(ball)) {
			continue_screen();
			ball = reset_ball();
			gl_clear(BACKGROUND);
			timer_delay_ms(1000);
		}*/

		//move left paddle to follow ball --> right now this is automated
		paddle1.y = ball.y;

		interrupts_enable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ);
		interrupts_disable_source(INTERRUPTS_BASIC_ARM_TIMER_IRQ);
		
		//check if paddles have hit top or bottom
		if(check_paddle_top(paddle1)) {
			paddle1.y = paddle1.height/2;
		}
		if(check_paddle_top(paddle2)) {
			paddle2.y = paddle2.height/2;
		}
		if(check_paddle_bottom(paddle1)) {
			paddle1.y = SCREENHEIGHT - (paddle1.height/2);
		}
		if(check_paddle_bottom(paddle2)) {
			paddle2.y = SCREENHEIGHT - (paddle2.height/2);
		}

		//check if ball hits paddles 
		if (ball.vx < 0) {
			if (check_hit_paddle(ball, paddle1)) {
				ball.vx *= -1;
			}
		}
		else {
			if (check_hit_paddle(ball, paddle2)) {
				ball.vx *= -1;
				currentgamescore +=1;

				if (currentgamescore %2 ==0 && currentgamescore !=0){
					if (ball.vx >=0){
						ball.vx+=1;
					}
					if (ball.vy >=0){ball.vy+=1;}
					else if (ball.vx <0){
						ball.vx-=1;}
					else if (ball.vy<0){
						ball.vy-=1;}
				}
			}

		}	    

		//Clear the current objects (to prepare for the redraw in the next iteration of the while loop).
		clear_ball(cur_ball.x, cur_ball.y);
		clear_paddle(paddle2, cur_pad2);
		clear_paddle(paddle1, cur_pad1);

	}

}


