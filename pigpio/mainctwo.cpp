//컴파일법 g++ -o mainctwo mainctwo.cpp -lpigpio -lrt -lpthread
#include <pigpio.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;
#define SERVO1 14
#define SERVO2 2
#define SERVO3 3
#define SERVO4 15
#define SERVO5 18
#define SERVO6 23

#define M_PI 3.14159265358979323846
#define TIME_DELAY 0.05
#define TIME_DELAY2 0.0001
#define INTERVAL 20
double anglecaculator(double input_angle);
int servoControl1(int servo1, double deg);
int servoControl2(int servo1, int servo2, double deg);
int initialize();
int initialize_during();

int main(){
   int testtype;
   int inputservo;
   double deg;
   double angle_servo1; 
   double angle_servo2;
   double angle_servo3;
   double angle_servo4;
   double angle_servo5;
   double angle_servo6;

   if (gpioInitialise() < 0)
   {
      fprintf(stderr, "pigpio initialisation failed\n");
      return 1;
   }

   /* Set GPIO modes */
   gpioSetMode(SERVO1, PI_OUTPUT);//최대 600
   gpioSetMode(SERVO2, PI_OUTPUT);
   gpioSetMode(SERVO3, PI_OUTPUT);
   gpioSetMode(SERVO4, PI_OUTPUT);
   gpioSetMode(SERVO5, PI_OUTPUT);
   gpioSetMode(SERVO6, PI_OUTPUT);
   initialize();
   while(1){
   printf("Hello servo test!\n 동시작동:1, 단일작동:0 종료:-1 각도계산:3 초기화:4\n");
   scanf("%d", &testtype);

   if(testtype==1){

      for(int i=0; i<10;i++){
         printf("동시작동! 입력방식: (각도)\n");
         printf("1번모터 각도 입력\n");
         scanf("%lf", &angle_servo1);
         fflush(stdin);
         if(angle_servo1==-1) {break;}         
         printf("2,3번모터 각도 입력(2번모터꺼만 입력)\n");
         scanf("%lf", &angle_servo2);
         fflush(stdin);
         if(angle_servo2==-1) {return 0;}
         printf("4번모터 각도 입력\n");
         scanf("%lf", &angle_servo4);
         fflush(stdin);
         if(angle_servo4==-1) {return 0;}
         printf("5번모터 각도 입력\n");
         scanf("%lf", &angle_servo5);
         fflush(stdin);
         if(angle_servo5==-1) {return 0;}
         printf("6번모터 각도 입력(9-120 부탁드려요)\n");//찝는거 67.5     
         scanf("%lf", &angle_servo6);
         fflush(stdin);
         if(angle_servo6==-1) {return 0;}
         if(angle_servo4>60){
         servoControl1(SERVO4, anglecaculator(80));
         servoControl2(SERVO2,SERVO3,anglecaculator(80));
         servoControl1(SERVO1, anglecaculator(angle_servo1));
         servoControl1(SERVO5, anglecaculator(angle_servo5));
         servoControl1(SERVO4, anglecaculator(angle_servo4));
         servoControl2(SERVO2,SERVO3,anglecaculator(angle_servo2));
         time_sleep(1);
         servoControl1(SERVO6, anglecaculator(angle_servo6));
         }
         else if(angle_servo4>50 && angle_servo4<=60){
            servoControl1(SERVO4, anglecaculator(80));
            servoControl2(SERVO2,SERVO3,anglecaculator(80));
            servoControl1(SERVO1, anglecaculator(angle_servo1));
            servoControl1(SERVO5, anglecaculator(angle_servo5));
            servoControl2(SERVO2,SERVO3,anglecaculator(70));
            servoControl1(SERVO4, anglecaculator(angle_servo4));
            servoControl2(SERVO2,SERVO3,anglecaculator(angle_servo2));
            time_sleep(1);
            servoControl1(SERVO6, anglecaculator(angle_servo6));
         }
         else{printf("error:동시작동");}
      }
   }
   else if(testtype==0){
      for(int i=0; i<20;i++){
         printf("단일작동! 입력방식: (모터) (각도)\n");
         scanf("%d %lf", &inputservo, &deg);
         //printf("%lf", deg);
         if(inputservo==-1 || deg==-1){
            break;
         }
         if(inputservo==1){
            servoControl1(SERVO1, anglecaculator(deg));
         }
         else if(inputservo==2){
            servoControl2(SERVO2,SERVO3,anglecaculator(deg));
         }
         else if(inputservo==3){
            printf("서보 3말고 2를 입력해주세요\n");
            i++;
         }
         else if(inputservo==4){
            servoControl1(SERVO4, anglecaculator(deg));
         }
         else if(inputservo==5){
           servoControl1(SERVO5, anglecaculator(deg));
         }
         else if(inputservo==6){
            servoControl1(SERVO6, anglecaculator(deg));
         }
         else{
            printf("올바른 숫자를 입력해주세요\n");
            i++;
         }
      }
   }
   else if(testtype==3){
      	double a = 0;
         double b = 0;
         double L = 0;
         double x = 0;
         double theta1;
         double theta2;
         double theta3;
         printf("%f, %f, %f\n",a, M_PI/2, sin(M_PI/2));
         cout << "a 값 입력";
         cin >> a;
         cout << "b 값 입력";
         cin >> b;
         cout << "L 값 입력";
         cin >> L;
         cout << "x 값 입력";
         cin >> x;
         
         theta1 = M_PI-asin(((pow(a,2) + pow(x, 2)) + pow(L, 2) - pow(b, 2)) / (2*a*sqrt( pow(x, 2) + pow(L, 2)))) - acos(x / sqrt(pow(x, 2) + pow(L, 2)));
         theta2= M_PI-asin(((a*sin(theta1)-x)/b))-theta1;
         int test;
         test=a*cos(M_PI/2+theta1)-b*cos(M_PI/2+theta1+M_PI/2-theta2);
         theta1 = theta1 * 360 / (2 * M_PI);
         theta2 = theta2 * 360 / (2 * M_PI);
         theta3 = (3*M_PI/2) * 360 / (2 * M_PI)-theta1-theta2;
         cout << "test:"<< test<<endl;
         cout << "theta1:"<< theta1<<endl;
         cout << "theta2:"<< theta2<<endl;
         cout << "theta3:"<< theta3<<endl;
   }
   else if(testtype==4){
      initialize_during();
   }
   else{

      printf("종료\n");
      gpioTerminate();
      return 0;

   }
   }
   

}
double anglecaculator(double input_angle){ //각도(180도)를 pigpio pulse로 변경


    if (input_angle<0 || input_angle>180){
        cout<<"error!!!! angle error by anglecaculator"<<endl;
        return 0;
    }

    int output_angle;
    output_angle=(2000*input_angle)/180+500;

    return output_angle;
    

}

int servoControl1(int servo1, double deg){
    
    if(deg<500 || deg >2500){
       printf("ERROR! INVAILD ANGLE-servoControl1");
        return 0;
    }
    double nowdeg;
    nowdeg=gpioGetServoPulsewidth(servo1);
    if(nowdeg<500 || nowdeg>2500){
        //nowdeg=1500;
    }
   if(nowdeg>500 && nowdeg<2500 && nowdeg<=deg){
      for(int j=nowdeg;j<=deg;j+=INTERVAL){
         gpioServo(servo1,j);
         time_sleep(TIME_DELAY);
         if(abs(deg-j)<2*INTERVAL){
            gpioServo(servo1,deg);
            time_sleep(TIME_DELAY);
            j=deg;
         }
         //printf("%d\n",j);
      }
   }
   else if(nowdeg>500 && nowdeg<2500 && nowdeg>deg){
      for(int j=nowdeg;j>=deg;j-=INTERVAL){
         gpioServo(servo1,j);
         time_sleep(TIME_DELAY);
            if(abs(deg-j)<2*INTERVAL){
            gpioServo(servo1,deg);
            time_sleep(TIME_DELAY);
            j=deg;
         }
         //printf("%d\n",j);
      }
   }
   else{
         printf("test");
        printf("No nowdeg info:servocontrol1\n");
        //gpioServo(servo1,deg);
   }
   time_sleep(0.5);
   
   return 0;
}

int servoControl2(int servo1, int servo2, double deg){
   
    if(deg<500 || deg >2500){
        printf("ERROR! INVAILD ANGLE-servoControl2");
        return 0;
    }
    int nowdeg=1500;
    nowdeg=gpioGetServoPulsewidth(servo1);

    if(nowdeg<500 || nowdeg>2500){
        //nowdeg=1500;
    }

    if(nowdeg>500 && nowdeg<2500 && nowdeg<=deg){
    for(int j=nowdeg;j<=deg;j+=INTERVAL){
        gpioServo(servo1,j);
        time_sleep(TIME_DELAY2);
        gpioServo(servo2,3000-j);
        time_sleep(TIME_DELAY);
        if(abs(deg-j)<2*INTERVAL){
            gpioServo(servo1,deg);
            time_sleep(TIME_DELAY2);
            gpioServo(servo2,3000-deg);
            time_sleep(TIME_DELAY);
            j=deg;
        }
        //printf("%d\n",j);
    }
    }
    else if(nowdeg>500 && nowdeg<2500 && nowdeg>deg){
    for(int j=nowdeg;j>=deg;j-=INTERVAL){
        gpioServo(servo1,j);
        time_sleep(TIME_DELAY2);
        gpioServo(servo2,3000-j);
        time_sleep(TIME_DELAY);
            if(abs(deg-j)<2*INTERVAL){
            gpioServo(servo1,deg);
            time_sleep(TIME_DELAY2);
            gpioServo(servo2,3000-deg);
            time_sleep(TIME_DELAY);
            j=deg;
        }
        printf("%d\n",j);
    }
    }
    else{
        printf("No nowdeg info:servocontrol2\n");
        //gpioServo(servo1,deg);
        time_sleep(TIME_DELAY2);
        //gpioServo(servo2,3000-deg);
    }
    time_sleep(0.5);


}

int initialize(){
   gpioServo(SERVO1,1588);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO5,1500);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO6,700);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO4,1888);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO2,1778);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO3, 3000-1778);

}
int initialize_during(){
   int nowdeg;
   nowdeg=gpioGetServoPulsewidth(SERVO2);
   servoControl2(SERVO2,SERVO3, nowdeg+200);
   servoControl1(SERVO1,1500);
   servoControl1(SERVO5,1500);
   servoControl2(SERVO2,SERVO3,1778);
   servoControl1(SERVO4,1888);
   servoControl1(SERVO6,700);
}