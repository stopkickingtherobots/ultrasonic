/*
Name:       Benn Hamer
Task:       4.1
Objective:  Introduction to mBed platform using motion and ultrasonic sensors.
Referemce:  https://os.mbed.com/components/HC-SR04/

Terminal Setup for Serial Monitor

Copy pasta this into terminal to get usb port n#
ls /dev/tty.usbmodem*

It will return the end digits for the following command

Then copy pasta:
screen /dev/tty.usbmodem1422
*/

#include "mbed.h"
#include "ultrasonic.h"
 
DigitalOut trigger(p6);
DigitalOut myled(LED1); //monitor trigger
DigitalOut myled2(LED2); //monitor echo
DigitalOut redLed(p29);
DigitalIn  echo(p7);
int distance = 0;
int correction = 0;
int far = 45;
int close = 30;
int closest = 15;
Timer sonar;

// Serial pc(USBTX, USBRX);
 
int main()
{
    sonar.reset();
// measure actual software polling timer delays
// delay used later in time correction
// start timer
    sonar.start();
// min software polling delay to read echo pin
    while (echo==2) {};
    myled2 = 0;
// stop timer
    sonar.stop();
// read timer
    correction = sonar.read_us();
    printf("Approximate software overhead timer delay is %d uS\n\r",correction);
 
//Loop to read Sonar distance values, scale, and print
    while(1) {
// trigger sonar to send a ping
        trigger = 1;
        myled = 1;
        myled2 = 0;
        sonar.reset();
        wait_us(10.0);
        trigger = 0;
        myled = 0;
        // printf("Flag 1");
//wait for echo high
        while (echo==0) {};
        myled2=echo;
        // printf("Flag 2");
//echo high, so start timer
        sonar.start();
//wait for echo low
        while (echo==1) {};
        // printf("Flag 3");
//stop timer and read value
        sonar.stop();
        // printf("Flag 4");
//subtract software overhead timer delay and scale to cm
        distance = (sonar.read_us()-correction)/58.0;
        myled2 = 0;
        // printf("Flag 5");
        // printf(" %d cm \n\r",distance);
        
        if(distance <= far && distance > close)
        {
            redLed = 1;
            wait(1);
            printf(" Far \n\r");
        }
        else if (distance <= close && distance > closest)
        {
            redLed = 1;
            wait(0.5);
            redLed = 0;
            wait(0.5);
            redLed = 1;
            wait(0.5); 
            printf(" Close \n\r");         
        }
        else if (distance <= closest)
        {
            for( int i = 0; i < 5; i++)
            {
                redLed = 1;
                wait(0.1);
                redLed = 0;
                wait(0.1);
            }
            printf(" Closest \n\r");
        }
        else
        {
            redLed = 0;
            printf(" No object \n\r");
        }
        
//wait so that any echo(s) return before sending another ping
        wait(0.2);
    }
}