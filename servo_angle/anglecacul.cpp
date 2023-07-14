#include <iostream>
#include <cmath>

using namespace std;

#define M_PI 3.14159265358979323846


int main() {
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
	
	
	return 0;
}