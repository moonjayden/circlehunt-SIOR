#include <opencv2/opencv.hpp> //opencv ���̺귯��
#include <stdio.h>

//namespace�� cv�� std�� ����� �� �ֵ��� ����
using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	int x, y, radius;
	VideoCapture capture(0);//0�� ī�޶��� ��ġ��ȣ=��Ʈ�� ī�޶�
	if (!capture.isOpened()) { //isOpened()�Լ��� ī�޶������� �ִ��� ������ �Ǵ����ִ� ���� . ī�޶� ������ ������ 1, ������ 0��ȯ
		cout << "Error opening video" << endl;
		return -1; //����ڿ��� ������ ������ �˸�
	}
	Mat frame, frame1; //frame,frame1����
	int i = 0; //���� i 0���� �ʱ�ȭ
	Mat cap;//cap ����
	while (1) {
		i = i + 1;
		char c = (char)waitKey(25);//waitkey�� ȣ���� �־�߸� ���������� �̹����� ������
		capture >> frame;//�� frame�� capture�κ��� �����´�
		if (frame.empty()) {//�ڷᰡ ������ ������
			break;
		}
		cvtColor(frame, frame1, COLOR_BGR2GRAY);//���ȭ
		imshow("HI", frame1);//������â�� ǥ��

		cap = frame1;
		if (i == 50) {
			break;
		}
		if (c == 27) {//esc Ű
			break;
		}
	}
	capture.release();

	Mat img = cap;
	medianBlur(img, img, 5);//������ ����
	Mat img_houghC;//����� ���� ǥ���� �̹��� ����
	img.copyTo(img_houghC);//���� �̹����� ������ ����
	vector<Vec3f> circles;
	HoughCircles(img_houghC, circles, HOUGH_GRADIENT, 1, 50, 50, 39, 0, 0);//��ü������ canny���� ���� �ǽ�. �� ����
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
	}//����� ���� �������� ���ؼ� �̹����� ���� �ش���� �߽� ǥ��


	imshow("img_houghC", img_houghC);
	waitKey(0);
	return 0;
}