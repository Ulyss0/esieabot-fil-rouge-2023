#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <esieabot-ai.h>

#define PIN_ENABLE_LEFT 23
#define PIN_FORWARD_LEFT 24
#define PIN_BACKWARD_LEFT 25
#define PIN_ENABLE_RIGHT 17
#define PIN_FORWARD_RIGHT 18
#define PIN_BACKWARD_RIGHT 4
#define PIN_LED 40
#define PIN_SERVO_FRONT 36
#define PIN_SERVO_BACK 32
#define PIN_PUSH_BUTTON 35


void move (int PIN_MOTOR, unsigned int power)
{
    gpio_write(pi, PIN_MOTOR,PI_HIGH);
	set_mode(pi, PIN_MOTOR,PI_OUTPUT);
	set_PWM_dutycycle(pi,PIN_MOTOR,power);
}


void stop (int PIN_MOTOR)
{
    gpio_write(pi, PIN_MOTOR,PI_LOW);
}


void fullStop ()
{
    gpio_write(pi, PIN_BACKWARD_LEFT,PI_LOW);
    gpio_write(pi, PIN_BACKWARD_RIGHT,PI_LOW);
    gpio_write(pi, PIN_FORWARD_RIGHT,PI_LOW);
    gpio_write(pi, PIN_FORWARD_LEFT,PI_LOW);
}


void forwardMove (unsigned int power, unsigned int time)
{
    move(PIN_FORWARD_RIGHT,power);
    move(PIN_FORWARD_LEFT,power);
    sleep(time);
}


void backwardMove (unsigned int power, unsigned int time)
{
    move(PIN_BACKWARD_LEFT,power);
    move(PIN_BACKWARD_RIGHT,power);
    sleep(time);
}


void servoTest(int PIN_SERVO)
{
    set_mode(pi, PIN_SERVO, PI_OUTPUT);
    set_servo_pulsewidth(pi, PIN_SERVO, 500);
    sleep(1);
    set_servo_pulsewidth(pi, PIN_SERVO, 1500);
    sleep(1);
    set_servo_pulsewidth(pi, PIN_SERVO, 2500);
    sleep(1);
    set_servo_pulsewidth(pi, PIN_SERVO, 0);
}


void LED_set_status(bool state)
{
    set_mode(pi, PIN_LED, PI_OUTPUT);
    gpio_write(pi, PIN_LED, state);
}


void LED_TEST()
{
    int i;
    for(i=0,i<5,i++)
    {
        LED_set_status(1);
        sleep(1);
        LED_set_status(0);
        sleep(1);
    }
}


int btp()
{
    set_mode(pi, PIN_PUSH_BUTTON, PI_INPUT);
    set_pull_up_down(pi, PIN_PUSH_BUTTON, PI_PUD_DOWN); 
    while (1) 
    {
        int state = gpio_read(pi, PIN_PUSH_BUTTON);
        if (state == PI_HIGH) 
        {
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


int main(){

    LED_TEST() ;
    btp() ;
    servoTest(32) ;
    forwardMove(100, 5) ;
    fullStop() ;
    backwardMove(100, 5) ;
    fullStop() ;

    while (1) {
        marker *markers = get_markers();
        while (markers->id != -1) {
            printf("Marker found: number %d at %dcm distance. x=%d y=%d\n", markers->id, markers->z, markers->x, markers->y);
            markers++;
        }
    }


    return 0 ;
} 
