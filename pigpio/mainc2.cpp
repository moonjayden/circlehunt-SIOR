//컴파일법 g++ -o mainc2 mainc2.cpp -lpigpio -lrt -lpthread -lwiringPi
#include <pigpio.h>
#include <pigpiod_if.h>
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

using namespace std;
#define SERVO1 15//pi
#define SERVO2 16//pi
#define SERVO3 18//pigpio
#define SERVO4 23//pigpio
#define TIME_DELAY 0.05
#define INTERVAL_PIGPIO 20
#define INTERVAL_WIRINGPI 0.2

int servoControl2(int servo1, int servo2, int deg) {
	int duty;
	int duty2;
	if (deg > 180) {
		deg = 180;
	}
   //printf("%d %d", gpioGetServoPulsewidth(SERVO1),gpioGetServoPulsewidth(SERVO2));
	duty = -deg / 9 + 25;
	duty2= 5+deg/9;
	//printf("duty: %d\n", duty);
   /*if(nowdeg<deg){
            for(int j=nowdeg;j<=deg;j+=INTERVAL_WIRINGPI){
               gpioServo(SERVO1,j);
               time_sleep(TIME_DELAY);
               if(abs(deg-j)<2*INTERVAL_WIRINGPI){
                  gpioServo(SERVO1,deg);
                  time_sleep(TIME_DELAY);
                  j=deg;
               }
               printf("%d\n",j);
            }
         }
   else if(nowdeg>deg){
      for(int j=nowdeg;j>=deg;j-=INTERVAL_WIRINGPI){
         gpioServo(SERVO1,j);
         time_sleep(TIME_DELAY);
            if(abs(deg-j)<2*INTERVAL_WIRINGPI){
            gpioServo(SERVO1,deg);
            time_sleep(TIME_DELAY);
            j=deg;
         }
         printf("%d\n",j);
      }
   }*/
	softPwmWrite(servo1, duty);
	softPwmWrite(servo2, duty2);
	delay(600);
	return 0;
}

int main(){

   int inputtype; //작동시킬 서보 1:맨아래 2,3:중간2개 4,5,6


   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }
   if (wiringPiSetup() == -1) {
		printf("error");
		return -1;
	}

   /* Set GPIO modes */
   gpioSetMode(SERVO3, PI_OUTPUT);
   gpioSetMode(SERVO4, PI_OUTPUT);
   //gpioSetMode(SERVO2, PI_OUTPUT);
   pinMode(SERVO1, OUTPUT);
	pinMode(SERVO2, OUTPUT);

   digitalWrite(SERVO1, LOW);
	digitalWrite(SERVO2, LOW);

   softPwmCreate(SERVO1, 0, 200);
	softPwmCreate(SERVO2, 0, 200);

   /*gpioServo(SERVO1, 1000);
   time_sleep(0.5);
   gpioServo(SERVO1, 600);
   time_sleep(0.5);
*/
   for (int i = 0; i < 20; i++) {
		int deg;
      int nowdeg;


		printf("Please input degree: \n");
		scanf("%d", &deg);
		if (deg == -1) {
			return 0;
		}
		//if (deg == 1000){
		//	servoControl2(SERVO1,SERVO2,90);
			//continue;
		//}
		//servoControl(SERVO,deg);

      servoControl2(SERVO1,SERVO2,(180-deg));
      time_sleep(0.5);
      gpioServo(SERVO4,500+11.11*(180-deg));
      time_sleep(0.5);
      gpioServo(SERVO3,500+11.11*(180-deg));
      time_sleep(0.5);
      
		//servoControl(SERVO2,deg);
		//servoControl(SERVO3,deg);
      nowdeg=deg;
	}

 
    gpioTerminate();

    return 0;
}
/*
#include <stdio.h>

#include <pigpio.h>

int main(int argc, char *argv[])
{
   double start;

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   gpioSetMode(4, PI_OUTPUT);
   gpioSetMode(17, PI_OUTPUT);
   gpioSetMode(18, PI_OUTPUT);
   gpioSetMode(23, PI_INPUT);
   gpioSetMode(24, PI_OUTPUT);


   
   gpioServo(4, 1000);

   //gpioPWM(17, 192); 
   time_sleep(0.5);
   gpioServo(4, 2000);
   time_sleep(0.5);
   //start = time_time();

  // while ((time_time() - start) < 60.0)
   {/*
      gpioWrite(18, 1); 

      time_sleep(0.5);

      gpioWrite(18, 0);

      time_sleep(0.5);

      gpioWrite(24, gpioRead(23));
   }


   gpioTerminate();

   return 0;
}*/