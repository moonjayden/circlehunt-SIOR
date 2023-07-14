//컴파일법 g++ -o mainc mainc.cpp -lpigpio -lrt -lpthread
#include <pigpio.h>
#include <iostream>
#include <stdio.h>

using namespace std;
#define SERVO1 14
#define SERVO2 15
#define TIME_DELAY 0.05
#define TIME_DELAY2 0.00001
#define INTERVAL 20

int main(){
   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   /* Set GPIO modes */
   gpioSetMode(SERVO1, PI_OUTPUT);//최대 600
   gpioSetMode(SERVO2, PI_OUTPUT);
   /*gpioServo(SERVO1, 1000);
   time_sleep(0.5);
   gpioServo(SERVO1, 700);
   time_sleep(0.5);*/


   for (int i = 0; i < 20; i++) {
		int deg;
      int nowdeg;
      int typeservo;
      if(i>0){
      nowdeg=gpioGetServoPulsewidth(SERVO1);
      
      }
      printf("Please input degree: \n");
		scanf("%d, %d", &deg, &typeservo);
      //scanf("%d", &deg);
      printf("now deg: %d, deg: %d, i: %d \n", nowdeg,deg,i);
		if (deg == -1) {
			return 0;
		}
		//if (deg == 1000){
		//	servoControl2(SERVO1,SERVO2,90);
			//continue;
		//}
		//servoControl(SERVO,deg);
      if(typeservo==1){
         if(i>0 && nowdeg<deg){
            for(int j=nowdeg;j<=deg;j+=INTERVAL){
               gpioServo(SERVO1,j);
               time_sleep(TIME_DELAY2);
               gpioServo(SERVO2,j);
               time_sleep(TIME_DELAY);
               if(abs(deg-j)<2*INTERVAL){
                  gpioServo(SERVO1,deg);
                  time_sleep(TIME_DELAY2);
                  gpioServo(SERVO2,deg);
                  time_sleep(TIME_DELAY);
                  j=deg;
               }
               printf("%d\n",j);
            }
         }
         else if(i>0 && nowdeg>deg){
            for(int j=nowdeg;j>=deg;j-=INTERVAL){
               gpioServo(SERVO1,j);
               time_sleep(TIME_DELAY2);
               gpioServo(SERVO2,j);
               time_sleep(TIME_DELAY);
                  if(abs(deg-j)<2*INTERVAL){
                  gpioServo(SERVO1,deg);
                  time_sleep(TIME_DELAY2);
                  gpioServo(SERVO2,deg);
                  time_sleep(TIME_DELAY);
                  j=deg;
               }
               printf("%d\n",j);
            }
         }
         else{
            gpioServo(SERVO1,deg);
            time_sleep(TIME_DELAY2);
            gpioServo(SERVO2,deg);
         }
         time_sleep(0.5);
      }
      else{
         gpioServo(SERVO1,deg);
         time_sleep(TIME_DELAY2);
         gpioServo(SERVO2,deg);
         time_sleep(0.5);
      }
      //gpioServo(SERVO2,(2500-deg+600));
      //time_sleep(0.5);
		//servoControl(SERVO2,deg);
		//servoControl(SERVO3,deg);
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