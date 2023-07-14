//컴파일법 g++ -o mainc mainc.cpp -lpigpio -lrt -lpthread(x) -> cmake.. 후 make
#include <pigpio.h>
#include <pigpiod_if.h>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <iostream> 
#include <string>
#include <stdio.h>
#include <cmath>

using namespace std;
using namespace cv;

#define M_PI 3.14159265358979323846 //pi값
#define TIME_DELAY 0.04             //모터 각도 움직이는 딜레이
#define TIME_DELAY2 0.0001          //동시에 움직이는 모터 딜레이(2번,3번)
#define INTERVAL 20                 //한번에 움직이는 각도 

#define SERVO1 14       // 1번 모터(맨아래)
#define SERVO2 2        // 2번 모터(중심부 2개 동시)
#define SERVO3 3        // 3번 모터(중심부 2개 동시)
#define SERVO4 15       // 4번 모터(2,3번 위)
#define SERVO5 18       // 5번 모터(집게 손목)
#define SERVO6 23       // 6번 모터(집게 집는 부분)

double a = 120;         // 2,3번 - 4번 길이
double b = 115;         // 4번 - 5번 길이
double L = 0;           // 로봇팔 몸통 중심 - 물체까지 직선거리
double x = 25;          // 2,3번 모터팔 - 5번모터까지 높이 차
float radius;           // 물체(원) 반지름
double theta1;          // 2,3번 모터 각도 저장소
double theta2;          // 4번 모터 각도 저장소
double theta3;          // 5번 모터 각도 저장소

double angle_servo1;    // 1번 모터 각도
double angle_servo2;    // 2번 모터 각도
double angle_servo3;    // 3번 모터 각도
double angle_servo4;    // 4번 모터 각도
double angle_servo5;    // 5번 모터 각도
double angle_servo6;    // 6번 모터 각도

int errorcode=0;        //에러 발생시 에러 출력부
/*
int now_angle_servo1;
int now_angle_servo2;
int now_angle_servo3;
int now_angle_servo4;
int now_angle_servo5;
int now_angle_servo6;
*/
void servoControl1(int servo1, double deg);                                 // 단일 모터 작동 함수(1,4,5,6번)
void servoControl2(int servo1, int servo2, double deg);                     // 동시 모터 작동 함수(2,3번)
double anglecaculator(double input_angle);                                  // 각도(0-180)-> 모터 pwm변환(1500-2500)
void servo_anglecaculator(double &a,double &b,double &L,double &x);         // L값을 통해 모터들의 각도 계산 
int servo_grip(float radius, int typegrip);                                 // 집게 집는 함수
void servo_move(double theta1,double theta2,double theta3,float radius);    // 모터 움직임 관련 총 지휘 함수
int servo_release();                                                        // 집은 후 놓는 것 함수
int opencv_main();                                                          // opencv 총 지휘 함수
int motor_main();                                                           // 모터 총 지휘 함수 
int initialize();                                                           // 모터 초기화 함수(맨 처음)
int initialize_during();                                                    // 모터 초기화 함수(작동 도중)  

int main(){

    int inputtype; //입력값 

        // GPIO 설정 관련
    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed\n");
        return 1;
    }


    //모터 설정값 지정
    gpioSetMode(SERVO1, PI_OUTPUT);
    gpioSetMode(SERVO2, PI_OUTPUT);
    gpioSetMode(SERVO3, PI_OUTPUT);
    gpioSetMode(SERVO4, PI_OUTPUT);
    gpioSetMode(SERVO5, PI_OUTPUT);
    gpioSetMode(SERVO6, PI_OUTPUT);
    initialize();  // 시작 전 모터 초기값 넣어주기

    while(1){
        printf("search:1 angle caculate:2 initalize:3 terminate:-1\n"); //입력값 받기

        scanf("%d", &inputtype);

        if(inputtype==1){ // 본 함수 실행

            while(1){

                opencv_main(); 
                motor_main();
                initialize_during();

            }
        }
        else if(inputtype==2){ //각도 계산용

            double a = 0;
            double b = 0;
            double L = 0;
            double x = 0;
            printf("%f, %f, %f\n",a, M_PI/2, sin(M_PI/2));
            cout << "a 값 입력"<<endl;
            cin >> a;
            cout << "b 값 입력"<<endl;
            cin >> b;
            cout << "L 값 입력"<<endl;
            cin >> L;
            cout << "x 값 입력"<<endl;
            cin >> x;
            servo_anglecaculator(a,b,L,x);

        }
        else if(inputtype==3){ //초기화 

            initialize_during();

        }
        else if(inputtype==4){ // L, theta 넣으면 1회용 움직이기

            cout << "L 값 입력"<<endl;
            cin >> L;
            cout << "theta 값 입력"<<endl;
            cin >> angle_servo1;
            radius=12.5;
            motor_main();
            initialize_during();

        }
        else if(inputtype==-1){ //종료
            return 0;
        }
    }



    //----------------------서보 설정 끝

    //main 쓸 곳

    gpioTerminate(); //모터 종료
    return 0;
} //main 끝

double anglecaculator(double input_angle){ // 각도(0-180)-> 모터 pwm변환(1500-2500)


    if (input_angle<0.0 || input_angle>180.0){ //잘못된 각도를 넣었을 경우 종료

        cout<<"error!!!! angle error by anglecaculator"<<endl;
        return 0;
    }

    int output_angle;

    output_angle=(2000*input_angle)/180+500; //각도 변화 계산

    return output_angle; //출력
    

}

void servo_anglecaculator(double &a,double &b,double &L,double &x){ // L값을 통해 모터들의 각도 계산 

    theta1 = M_PI-asin(((pow(a,2) + pow(x, 2)) + pow(L, 2) - pow(b, 2)) / (2*a*sqrt( pow(x, 2) + pow(L, 2)))) - acos(x / sqrt(pow(x, 2) + pow(L, 2)));
    theta2= M_PI-asin(((a*sin(theta1)-x)/b))-theta1;
    theta1 = theta1 * 360 / (2 * M_PI); //라디안 -> 각도 변환
    theta2 = theta2 * 360 / (2 * M_PI); //라디안 -> 각도 변환
    theta3 = (3*M_PI/2) * 360 / (2 * M_PI)-theta1-theta2;
    cout << "theta1:"<< theta1<<endl;
    cout << "theta2:"<< theta2<<endl;
    cout << "theta3:"<< theta3<<endl;
}

void servoControl1(int servo1, double deg){// 단일 모터 작동 함수(1,4,5,6번)
    
    if(deg<500 || deg >2500){ //잘못된 pwm입력시 종료

        printf("ERROR! INVAILD ANGLE-servoControl1");
        return;

    }

    double nowdeg;
    nowdeg=gpioGetServoPulsewidth(servo1); // 현재 pwm값 받기

    if(nowdeg<500 || nowdeg>2500){
        //nowdeg=1500;
    }
   if(nowdeg>500 && nowdeg<2500 && nowdeg<=deg){ //현재 각보다 클 경우

      for(int j=nowdeg;j<=deg;j+=INTERVAL){ //천천히 움직이기 위해

         gpioServo(servo1,j);
         time_sleep(TIME_DELAY);

         if(abs(deg-j)<2*INTERVAL){

            gpioServo(servo1,deg);
            time_sleep(TIME_DELAY);
            j=deg;

         }
         //printf("%d\n",j);
      }
      printf("%d %lf\n",servo1, deg);
   }
   else if(nowdeg>500 && nowdeg<2500 && nowdeg>deg){ // 현재 값보다 작을 경우

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

      printf("%d %lf\n",servo1, deg);

   }
   else{ //이도저도 아니면 오류 출력 후 종료
         printf("test");
        printf("No nowdeg info:servocontrol1\n");
        //gpioServo(servo1,deg);
   }
   time_sleep(0.5);

}

void servoControl2(int servo1, int servo2, double deg){ // 동시 모터 작동 함수(2,3번)
   
    if(deg<500 || deg >2500){
        printf("ERROR! INVAILD ANGLE-servoControl2");
        return;
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
        
    }
    printf("%d %lf\n",servo1, deg);
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
        
    }
    printf("%d %lf\n",servo1, deg);
    }
    else{
        printf("No nowdeg info:servocontrol2\n");
        //gpioServo(servo1,deg);
        time_sleep(TIME_DELAY2);
        //gpioServo(servo2,3000-deg);
    }
    time_sleep(0.5);


}

int servo_grip(float radius, int typegrip){ // 집게 집는 함수
    
    if (typegrip==1){ //집기 넣어줬을때
        servoControl1(SERVO6,650);
        time_sleep(0.5);

        angle_servo6=(600+(4333/1000)*2*radius); // 반지름에 따라 집게 집는 각도 변경
        if (angle_servo6<1750.0){
            angle_servo6+=20;
        }
        printf("grip angle: %lf\n", angle_servo6);
        if(angle_servo6<650.0 || angle_servo6>=1750.0){
            cout<<"error: Can't grip!! by servo_grip"<<endl;
            return 0;
        }
        servoControl1(SERVO6,angle_servo6);
        //gpioServo(SERVO6,angle_servo6);
        time_sleep(TIME_DELAY);
    }

    else if(typegrip==0){
        servoControl1(SERVO6, 650);
    }

    return 0;
    
}

void servo_move(double theta1,double theta2,double theta3,float radius){ // 모터 움직임 관련 총 지휘 함수

    if(L>226 || errorcode==1){ //팔이 닿을 수 없거나 오류 출력시 파손 방지를 위해 함수 종료
        printf("L= %lf, can't move! or errorcode= %d \n", L,errorcode);
        errorcode=1;
        return;
    }
    //angle_servo1+=3;//;
    angle_servo2=theta1;//-5;
    angle_servo3=theta1;//-5;
    angle_servo4=180-theta2;//반대값
    angle_servo5=theta3-3;//보정값

    //계산값 넣어주기
    angle_servo1=anglecaculator(angle_servo1);
    angle_servo2=anglecaculator(angle_servo2);
    angle_servo3=anglecaculator(180-angle_servo3);
    angle_servo4=anglecaculator(angle_servo4);
    angle_servo5=anglecaculator(angle_servo5);

    /*servoControl1(SERVO1, angle_servo1);
    servoControl2(SERVO2,SERVO3,angle_servo2);
    servoControl1(SERVO4, angle_servo4);
    servoControl1(SERVO5, angle_servo5);
    servo_grip(radius, 1);*/

    if(angle_servo4>anglecaculator(60)){ //4번모터 너무 올라가면 위랑 부딪혀서 조심조심 올라가기
         servoControl1(SERVO4, anglecaculator(80));
         servoControl2(SERVO2,SERVO3,anglecaculator(80));
         servoControl1(SERVO1, angle_servo1);
         servoControl1(SERVO5, angle_servo5);
         servoControl1(SERVO4, angle_servo4);
         servoControl2(SERVO2,SERVO3,angle_servo2);
         time_sleep(1);
         servo_grip(radius, 1);
         }
    else if(angle_servo4>anglecaculator(20) && angle_servo4<=anglecaculator(60)){
        servoControl1(SERVO4, anglecaculator(80));
        servoControl2(SERVO2,SERVO3,anglecaculator(80));
        servoControl1(SERVO1, angle_servo1);
        servoControl1(SERVO5, angle_servo5);
        servoControl2(SERVO2,SERVO3,anglecaculator(70));
        servoControl1(SERVO4, angle_servo4);
        servoControl2(SERVO2,SERVO3,angle_servo2);
        time_sleep(1);
        servo_grip(radius, 1);
    }
    else{
        printf("error:servo_move");
        
    }
}

int initialize(){ //모터 초기화 함수(맨 처음)
   gpioServo(SERVO1,anglecaculator(100));
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO5,1500);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO6,700);
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO4,anglecaculator(120));
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO2,1723);//1778
   time_sleep(TIME_DELAY2);
   gpioServo(SERVO3, 3000-1723);

}
int initialize_during(){ // 모터 초기화 함수(작동 도중) 
    int nowdeg;
    printf("initializing...\n");
    if(errorcode!=1){
        nowdeg=gpioGetServoPulsewidth(SERVO2);
        servoControl2(SERVO2,SERVO3, nowdeg+200);
        servoControl1(SERVO1,anglecaculator(100));
        servoControl1(SERVO5,1500);
        printf("servo value= %lf\n", angle_servo4);
        if(angle_servo4<=anglecaculator(60)){
            servoControl2(SERVO2,SERVO3,anglecaculator(60));
            servoControl1(SERVO4, anglecaculator(70));
        }
    }
    else if(errorcode==1){
        servoControl1(SERVO1,anglecaculator(100));
        servoControl2(SERVO2,SERVO3,1723);
        servoControl1(SERVO4,anglecaculator(120));
        servoControl1(SERVO5,1500);
        servoControl1(SERVO6,700);       
    }
    servoControl2(SERVO2,SERVO3,1723);
    servoControl1(SERVO4,anglecaculator(120));
    servoControl1(SERVO6,700);
    errorcode=0;

    angle_servo1=anglecaculator(100); 
    angle_servo2=1833;
    angle_servo3=1167;
    angle_servo4=anglecaculator(120);
    angle_servo5=1500;
    angle_servo6=700;
}

int servo_release(){ // 집은 후 놓는 것 함수

    //일정 위치로 이동
    printf("releasing\n");
    angle_servo1=anglecaculator(10); //각도 바꿔줘야함!
    /*angle_servo2=anglecaculator(90);
    angle_servo3=anglecaculator(90);
    angle_servo4=anglecaculator(90);
    angle_servo5=anglecaculator(90);*/

    servoControl2(SERVO2,SERVO3,anglecaculator(80));
    servoControl1(SERVO4, anglecaculator(70));
    
    servoControl1(SERVO5, anglecaculator(90));
    servoControl1(SERVO1, angle_servo1);
    servoControl2(SERVO2,SERVO3,anglecaculator(60));
    servo_grip(0, 0);
    
}
int opencv_main(){ // opencv 총 지휘 함수
    raspicam::RaspiCam_Cv Camera;

	int x, y, opencv_radius;
	int a = 0, b = 0, k = 0;
	float real_x,real_y, real_r, opencv_L, theta,r;
	size_t n = 1;
	Mat frame, frame1; //frame,frame1����
	int i = 0; //���� i 0���� �ʱ�ȭ
	Mat cap;//cap ����
	while (1) {
        Camera.set( CAP_PROP_FORMAT, CV_8UC3);
        Camera.set( CAP_PROP_FRAME_WIDTH, 640 );
        Camera.set( CAP_PROP_FRAME_HEIGHT, 480 );
		if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}


		while (1) {
			i = i + 1;
			char c = (char)waitKey(25);//waitkey�� ȣ���� �־�߸� ���������� �̹����� ������
			Camera.grab();
            Camera.retrieve ( frame);
			if (frame.empty()) {//�ڷᰡ ������ ������
				break;
			}
			cvtColor(frame, frame1, COLOR_BGR2GRAY);//���ȭ
			imshow("HI", frame1);//������â�� ǥ��

			cap = frame1;
			if (i == 65) {
				break;
			}
			if (c == 27) {//esc Ű
				break;
			}
		}
		Camera.release();
		i = 0;
		Mat img = cap;
		medianBlur(img, img, 5);//������ ����
		Mat img_houghC;//����� ���� ǥ���� �̹��� ����
		img.copyTo(img_houghC);//���� �̹����� ������ ����
		vector<Vec3f> circles;
		HoughCircles(img_houghC, circles, HOUGH_GRADIENT, 1, 50, 50, 40, 0, 0);//��ü������ canny���� ���� �ǽ�. �� ����
		for (; n - 1 < circles.size(); n++)
		{
			Vec3i c = circles[n - 1];
			Point center(c[0], c[1]);
			opencv_radius = c[2];
			circle(img_houghC, center, opencv_radius, Scalar(0, 255, 0), 2);
			circle(img_houghC, center, 2, Scalar(0, 0, 255), 3);
			x = c[0];
			y = c[1];
			printf("<Circle number %d>\nx=%d\ny=%d\nradius=%d\n", n, x, y, opencv_radius);
			r=opencv_radius;

		}//����� ���� �������� ���ؼ� �̹����� ���� �ش���� �߽� ǥ��
		imshow("img_houghC", img_houghC);
		real_x=x*(400.0/640.0);
		real_y=(480.0-y)*(305.0/480.0);
		real_r=(r*12.5)/20;
        if(real_x>=208.0){

            if(180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=60){//0~30
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+5;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-3;
            }
            else if(180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<60 && 180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=50){//30~40
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+20;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-4;
            }
            else if(180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<50 && 180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=40){//40~50
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+12;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-7;
            }
            else if(180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<40 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=30){//50~60
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+12;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-13;
            }
            else if(180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<30 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=20){//60~70
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+17;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-7;
            }
            else{//70~90
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+8;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-0.5;
            }
            
        }
        else if(real_x>=177.5 && real_x<208.0){
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2));
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654+1;
        }
        else if(real_x<177.5){
            
            if(-180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=60){
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))-3;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654+3;
            }
            else if(-180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<60 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=50){
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))-20;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654+2.4;
            }
            else if(-180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<50 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=40){
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))-13;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654+1.3;
            }
            else if(-180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<40 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=30){//120~130
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))-4;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654;
            }
            else if(-180*atan((real_x-208.0)/(real_y+77.9))/3.141592654<30 && -180*atan((real_x-208.0)/(real_y+77.9))/3.141592654>=20){//110~120
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))-1;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-1.25;
            }
            else{
                opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2))+2;
                theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654-2;
            }
            
        }
        if(opencv_L<=160){
            opencv_L-=1;
            theta+=1.5;
        }

		//opencv_L=sqrt(pow(real_x-208.0,2)+pow(real_y+77.9,2));
		//theta=180*atan((real_x-208.0)/(real_y+77.9))/3.141592654;
		if ((a == x || b == y) && k + 1 == n - 1) {
			printf("real_r;%f, real x:%f , real y:%f, L=%f, theta=%f\n",real_r, real_x,real_y,opencv_L,90-theta);
			printf("Let's move Robotic Arm!\n");
			break;
		}
		a = x;
		b = y;
		k = 0;
		n = 1;
        if(x==0 || y==0){
            errorcode=1;
        }
        L=opencv_L;
        radius=real_r;
        angle_servo1=90-theta;

		

		

	}

	return 0;
}

int motor_main(){ // 모터 총 지휘 함수 


    

    servo_anglecaculator(a,b,L,x);
    //2,3,4,5번모터 각도계산 종료

    //모터 작동 시작
    servo_move(theta1, theta2, theta3, radius);
    if(angle_servo4<anglecaculator(40) || L>226){
        printf("angle_servo4 is too low/ L is too far - servo_main\n");
        angle_servo1=anglecaculator(95); 
        angle_servo2=1833;
        angle_servo3=1167;
        angle_servo4=anglecaculator(120);
        angle_servo5=1500;
        angle_servo6=700;
        return 0;
    }
    printf("hi!");
    
    
    servo_release();
    



}