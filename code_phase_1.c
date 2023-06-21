#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <esieabot-ai.h>
#include <pigpiod_if2.h>

#define PIN_ENABLE_LEFT 24
#define PIN_FORWARD_LEFT 23
#define PIN_BACKWARD_LEFT 25
#define PIN_ENABLE_RIGHT 4
#define PIN_FORWARD_RIGHT 22
#define PIN_BACKWARD_RIGHT 17
#define PIN_LED 21
#define PIN_SERVO_FRONT 16
#define PIN_SERVO_BACK 12
#define PIN_PUSH_BUTTON 19
//#define PI pigpio_start(NULL,NULL)

void setup(int pi)
{
    set_mode(pi,PIN_LED, PI_OUTPUT);
	set_mode(pi,PIN_PUSH_BUTTON, PI_INPUT);
    set_pull_up_down(pi, PIN_PUSH_BUTTON, PI_PUD_DOWN);

    set_mode(pi,PIN_ENABLE_RIGHT, PI_OUTPUT);
    set_mode(pi,PIN_FORWARD_RIGHT, PI_OUTPUT);
    set_mode(pi,PIN_BACKWARD_RIGHT, PI_OUTPUT);

    set_mode(pi,PIN_ENABLE_LEFT, PI_OUTPUT);
    set_mode(pi,PIN_FORWARD_LEFT, PI_OUTPUT);
    set_mode(pi,PIN_BACKWARD_LEFT, PI_OUTPUT);

    set_mode(pi, PIN_SERVO_BACK, PI_OUTPUT);
    set_mode(pi, PIN_SERVO_FRONT, PI_OUTPUT);

    
}

void move (int PI, int PIN_MOTOR, unsigned int power)
{
	set_PWM_dutycycle(PI,PIN_MOTOR,power);
}


void stop (int PI,int PIN_MOTOR)
{
    gpio_write(PI, PIN_MOTOR,PI_LOW);
}


void fullStop (int PI)
{
    set_PWM_dutycycle(PI,PIN_BACKWARD_LEFT,0);
    set_PWM_dutycycle(PI,PIN_BACKWARD_RIGHT,0);
    set_PWM_dutycycle(PI,PIN_FORWARD_RIGHT,0);
    set_PWM_dutycycle(PI,PIN_FORWARD_LEFT,0);
}


void forwardMove (int PI, unsigned int power, unsigned int time)
{
	move(PI, PIN_FORWARD_RIGHT,power);
	move(PI, PIN_FORWARD_LEFT,power);
	sleep(time);
}


void backwardMove (int PI,unsigned int power, unsigned int time)
{
	move(PI, PIN_BACKWARD_LEFT,power);
	move(PI, PIN_BACKWARD_RIGHT,power);
	sleep(time);
}


void servoTest(int PI, int PIN_SERVO)
{    
    set_servo_pulsewidth(PI, PIN_SERVO, 500);
    sleep(1);
    set_servo_pulsewidth(PI, PIN_SERVO, 1500);
    sleep(1);
    set_servo_pulsewidth(PI, PIN_SERVO, 2500);
    sleep(1);
    set_servo_pulsewidth(PI, PIN_SERVO, 0);
}


void LED_set_status(int PI, int state)
{
    gpio_write(PI, PIN_LED, state);
}


void LED_TEST(int PI)
{
    int i;
    for(i=0;i<5;i++)
    {
        LED_set_status(PI,1);
        sleep(0.1);
        LED_set_status(PI,0);
        sleep(0.1);
    }
}

int btp(int PI)
{ 
    int state = gpio_read(PI, PIN_PUSH_BUTTON);
    while (1) 
    {
        state = gpio_read(PI, PIN_PUSH_BUTTON);
	if (state == PI_HIGH){
	    printf("Le bouton poussoir sur le GPIO 16 est enclenché\n");
		return 1;
	}
        else if (state == PI_LOW) 
        {
            printf("Le bouton poussoir sur le GPIO 16 est relaché\n");
        }
        else 
        {
            printf("Le GPIO 16 est dans un état inconnu\n");
        }
        sleep(1);
    }
}


int main()
{
	
	int pi =pigpio_start(NULL,NULL);
    setup(pi);
	LED_TEST(pi) ;
	printf("led ok\n");
	btp(pi) ;
	printf("btp ok\n");
	servoTest(pi, PIN_SERVO_FRONT) ;
	printf("servo ok\n");
	forwardMove(pi, 255, 1) ;
	printf("fwd\n");
	fullStop(pi) ;
	printf("stop\n");
	backwardMove(pi, 255, 1) ;
	printf("bwd\n");
	fullStop(pi) ;
	printf("stop2\n");

	while (1) 
    {
        marker *markers = get_markers();
        while (markers->id != -1) 
        {
            printf("Marker found: number %d at %dcm distance. x=%d y=%d\n", markers->id, markers->z, markers->x, markers->y);
            markers++;
        }
    }


    return 0 ;
} 
