#include <opencv2/opencv.hpp> //opencv 라이브러리
#include <stdio.h>

//namespace에 cv와 std를 사용할 수 있도록 선언
using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	int x, y, radius;
	VideoCapture capture(0);//0은 카메라의 장치번호=노트북 카메라
	if (!capture.isOpened()) { //isOpened()함수는 카메라정보가 있는지 없는지 판단해주는 역할 . 카메라 정보가 있으면 1, 없으면 0반환
		cout << "Error opening video" << endl;
		return -1; //사용자에게 오류가 있음을 알림
	}
	Mat frame, frame1; //frame,frame1선언
	int i = 0; //변수 i 0으로 초기화
	Mat cap;//cap 선언
	while (1) {
		i = i + 1;
		char c = (char)waitKey(25);//waitkey를 호출해 주어야만 정상적으로 이미지가 보여짐
		capture >> frame;//새 frame을 capture로부터 가져온다
		if (frame.empty()) {//자료가 없으면 끝낸다
			break;
		}
		cvtColor(frame, frame1, COLOR_BGR2GRAY);//흑색화
		imshow("HI", frame1);//윈도우창에 표시

		cap = frame1;
		if (i == 50) {
			break;
		}
		if (c == 27) {//esc 키
			break;
		}
	}
	capture.release();

	Mat img = cap;
	medianBlur(img, img, 5);//노이즈 제거
	Mat img_houghC;//검출된 원을 표시할 이미지 생성
	img.copyTo(img_houghC);//원본 이미지의 정보를 복붙
	vector<Vec3f> circles;
	HoughCircles(img_houghC, circles, HOUGH_GRADIENT, 1, 50, 50, 39, 0, 0);//자체적으로 canny엣지 검출 실시. 원 검출
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center(c[0], c[1]);
		radius = c[2];
		circle(img_houghC, center, radius, Scalar(0, 255, 0), 2);
		circle(img_houghC, center, 2, Scalar(0, 0, 255), 3);
		x = c[0];
		y = c[1];
		printf("<Circle number %d>\nx=%d\ny=%d\nradius=%d\n",i+1, x, y, radius);
	}//검출된 원의 정보들을 통해서 이미지에 원과 해당원의 중심 표시


	imshow("img_houghC", img_houghC);
	waitKey(0);
	return 0;
}