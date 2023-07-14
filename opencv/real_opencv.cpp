#include <opencv2/opencv.hpp> //opencv ���̺귯��
#include <stdio.h>

//namespace�� cv�� std�� ����� �� �ֵ��� ����
using namespace cv;
using namespace std;

int main(int ac, char** av)
{
	int x, y, radius;
	int a = 0, b = 0, k = 0;
	size_t n = 1;
	Mat frame, frame1; //frame,frame1����
	int i = 0; //���� i 0���� �ʱ�ȭ
	Mat cap;//cap ����
	while (1) {

		VideoCapture capture(0);//0�� ī�޶��� ��ġ��ȣ=��Ʈ�� ī�޶�
		if (!capture.isOpened()) { //isOpened()�Լ��� ī�޶������� �ִ��� ������ �Ǵ����ִ� ���� . ī�޶� ������ ������ 1, ������ 0��ȯ
			cout << "Error opening video" << endl;
			return -1; //����ڿ��� ������ ������ �˸�
		}

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
			if (i == 65) {
				break;
			}
			if (c == 27) {//esc Ű
				break;
			}
		}
		capture.release();
		i = 0;
		Mat img = cap;
		medianBlur(img, img, 5);//������ ����
		Mat img_houghC;//����� ���� ǥ���� �̹��� ����
		img.copyTo(img_houghC);//���� �̹����� ������ ����
		vector<Vec3f> circles;
		HoughCircles(img_houghC, circles, HOUGH_GRADIENT, 1, 50, 50, 33, 0, 0);//��ü������ canny���� ���� �ǽ�. �� ����
		for (; n - 1 < circles.size(); n++)
		{
			Vec3i c = circles[n - 1];
			Point center(c[0], c[1]);
			radius = c[2];
			circle(img_houghC, center, radius, Scalar(0, 255, 0), 2);
			circle(img_houghC, center, 2, Scalar(0, 0, 255), 3);
			x = c[0];
			y = c[1];
			printf("<Circle number %d>\nx=%d\ny=%d\nradius=%d\n", n, x, y, radius);

		}//����� ���� �������� ���ؼ� �̹����� ���� �ش���� �߽� ǥ��
		imshow("img_houghC", img_houghC);
		if ((a == x || b == y) && k + 1 == n - 1) {
			printf("Let's move Robotic Arm!");
			break;
		}
		a = x;
		b = y;
		k = 0;
		n = 1;

	}

	return 0;
}