<<<<<<< HEAD
## Project Title

Let's Play Pong!

## Team Members

Iddah Mlauzi<br />
Connie Hong


## Project Description

Our final project for CS107E was to recreate Pong with a physical paddle. Specifically, we created a game of virtual pong in which a player can interact with their onscreen paddle with a physical “paddle.” The paddle contains an accelerometer that measures angular acceleration data that is translated in our code into the change in position values to inform the paddle position on the screen.<br />

Our program centers on three components/files: <br />
1. Handling our hardware in sensor.h functions.<br />
2. Updating our game objects with our functions from paddle.h and ball.h.<br />
3. Our main gameplay in myprogram.c.<br />

Our hardware consists of two main peripherals: <br />
1. The LSM6DS33 accelerometer/gyroscope. <br />
2. A tactile switch button. <br />

We utilized an interrupts approach to read from both of these inputs. For the gyroscope, we implemented our handler function to fire on a set interval of time, which we were able to set with functions from “armtimer.h”. In other words, our handler function for the gyroscope, which read values from the sensor and correspondingly updated the paddle position, was essentially registered to fire on every (x) amount of time. <br />

Our button served a variety of functions in our gameplay: it was switched between reading data based on an interrupts approach to a simple gpio-read input status depending on its usage. Specifically, we first utilized gpio_read() to determine a press of the button to initiate the gameplay. Once we enter the game, the button then acts as a “reset” button for the player’s paddle: every time it is pressed, the paddle is reset towards the center of the screen. To allow for this dual functionality, we only enable interrupts for this input when the game is currently running, and disable it while we are displaying other game information (i.e. the welcome message, our points screen, our end screen, etc.).<br />

## Member Contribution

Our project was mainly split into two facets: graphics and handling sensor input. <br />

For our first milestone, Iddah implemented the functions behind paddle.c and ball.c/ She also worked on outlining the general layout of the gameplay (display screen, read sensor inputs, execute game logic). During this time, Connie worked on learning how to read from the accelerometer sensor, and also configure two sensors on the same I2C bus.<br />

Iddah then worked on implementing the interrupt-functionality behind reading from the gyroscope on the sensor, while Connie worked on making the one button on our paddle prototype “multifunctional”. Connie also worked on the further aspects of the game functionality: specifically, maintaining a score count, a timed end-screen, as well as the different displayed screens throughout the game. Furthermore, Iddah also polished the drawing/update mechanisms behind our game objects to minimize glitching/lags. <br />

## References

To integrate our sensors with the graphical display on our game screen, we utilized a variety of resources and manuals for the LSM6DS33 accelerometer/gyroscope.<br />

First of all, our sensor.h functions utilized the lsm6ds33_read_gyrscope() and lsm6ds33_init() functions from Pat’s sensor lecture here: https://github.com/cs107e/cs107e.github.io/tree/master/lectures/Sensors/code/accel.<br />

While adopting our project to account for two paddles (in other words, two accelerometers), we also referenced the manual for https://www.pololu.com/product/2736 to learn how we could change the address of our sensor on the I2C bus to have two of the same lsm6ds33 sensors on the same bus, although with unique addresses.<br />

When designing our gameplay, we also referenced existing online versions of pong to deduce the exact functionality of our game(i.e. update score, how the ball bounces, etc): https://www.ponggame.org/.<br />

## Self-evaluation

Our main challenge was trying to accurately translate values we read from the gyroscope and accelerometer into precise position change values for our on-screen paddle. In our final implementation, utilizing interrupts on manually enabled events with the armtimer.h functions allowed us to take our angular acceleration inputs, and multiply them by the amount of time that had passed between interrupts, to determine our position change values. We also set a bottom threshold to reduce noise in our sensor: specifically, we would only update the ball position if we saw our position change above a certain value.<br />

We also faced challenges with having a game screen that did not glitch frequently. When we first started the project, we utilized a double buffering system to render our graphics. However, as reading values from our gyroscope significantly slowed down the rate at which our game refreshed, we then decided to revert to a single buffer mode.<br />

Furthermore, rather than redrawing the game every iteration, we also needed to devise ways to clear only the graphics we were drawing (paddle, ball) to minimize the number of background jobs.  <br />

Overall, our final product matched our initial expectations for the game. Through the course of building this up, we learned how to first start by confirming the basic functionality of our game components independently (i.e. making sure the sensor reads accurate values or displaying the screen smoothly), before then integrating them and adding further complexity. We also enjoyed applying our knowledge of hardware - specifically interrupts - we had gained from the class to make our game faster. <br />

Looking forward, if we had extra time, our main two areas for improvement would be to find the hardware necessary to have two players (see twoplayers.dev for our work in progress in this aspect), and perhaps also update the game to contain more graphics functionality (i.e. displaying a continuous score count, and making the ball bounce at angles that better replicate what happens in real-life scenarios). Please refer to our TwoPlayerVersion-ROUGHDRAFT and Trignometery folders for our work in progress towards these goals!<br />

## Photos
Please refer to the “Pictures” folder in the repo!
>>>>>>> e6635459d38336aef049d9af68b1123b5688a64b
