#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

//#define SERVO 1
#define SERVO 1
int servoControl(int deg) {
	int duty;
	if (deg > 180) {
		deg = 180;
	}
	duty = -deg / 9 + 25;
	printf("duty: %d\n", duty);
	softPwmWrite(SERVO1, duty);
	delay(600);
	return 0;
}

int main() {

	if (wiringPiSetup() == -1) {
		printf("error");
		return -1;
	}
	softPwmCreate(SERVO1, 0, 200);
	servoControl(0);

	for (int i = 0; i < 5; i++) {
		int deg;

		printf("Please input degree: \n");
		scanf("%d", &deg);
		if (deg == -1) {
			return 0;
		}
		servoControl(deg);
	}
	return 0;
}
