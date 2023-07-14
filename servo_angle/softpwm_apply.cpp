#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 4 //변경가능
#define SERVO1 15
#define SERVO2 16
#define SERVO3 1

int servoControl(int servo,int deg) {
	int duty;
	
	if (deg > 180) {
		deg = 180;
	}
	duty = -deg / 9 + 25;
	
	printf("duty: %d\n", duty);
	softPwmWrite(servo, duty);
	delay(600);
	return 0;
}

int servoControl2(int servo1, int servo2, int deg) {
	int duty;
	int duty2;
	if (deg > 180) {
		deg = 180;
	}
	duty = -deg / 9 + 25;
	duty2= 5+deg/9;
	//printf("duty: %d\n", duty);
	softPwmWrite(servo1, duty);
	softPwmWrite(servo2, duty2);
	delay(600);
	return 0;
}

int main() {
	printf("hello servo! ver2!\n");
	if (wiringPiSetup() == -1) {
		printf("error");
		return -1;
	}
	//pinMode(SERVO, OUTPUT);
	pinMode(SERVO1, OUTPUT);
	pinMode(SERVO2, OUTPUT);
	//pinMode(SERVO3, OUTPUT);

	//digitalWrite(SERVO, LOW);
	digitalWrite(SERVO1, LOW);
	digitalWrite(SERVO2, LOW);
	//digitalWrite(SERVO3, LOW);

	//softPwmCreate(SERVO, 0, 200);
	softPwmCreate(SERVO1, 0, 200);
	softPwmCreate(SERVO2, 0, 200);
	//softPwmCreate(SERVO3, 0, 200);

	//servoControl(SERVO,0);

	//servoControl(SERVO2,0);
	//servoControl(SERVO3,0);

	for (int i = 0; i < 5; i++) {
		int deg;

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
		servoControl2(SERVO1,SERVO2,deg);
		//servoControl(SERVO2,deg);
		//servoControl(SERVO3,deg);
	}
	return 0;
}
