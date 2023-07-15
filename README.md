# circle-hunt
Circle Hunting Robotic Arm Using Image Processing
2020.09~2020.11<div align=center>
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/0128872d-4027-45ba-8511-dda0e81023a9)
<div align=left>
It is a robotic arm which is made to find only desired object among several objects and move it to the desired position. Import images of objects from camera module and recognize only circular objects among several objects using Opencv.


 
### [Overall Config]
<div align=center>

![image](https://github.com/moonjayden/circle-hunt/assets/139466574/867ab801-93b9-493b-988a-bc78097bac88)
<div align=left>

 
카메라가 로봇 팔의 가동범위 내에 있는 물체들을 촬영합니다.  촬영한 영상 속 이미지 프레임마다 Opencv로 원형 판별을 하고 해당 원형 물체의 중심 좌표를 라즈베리파이에 전달하게 됩니다. 라즈베리파이에서 각도와 거리 계산을 통한 모터들의 움직일 각을 찾아낸 뒤 각도에 맞게 서보모터를 작동시킵니다. 

[Mechanical Part]  
Autodest Inventor을 이용해서 설계한 뒤, 교내 런닝팩토리에 구비된 3D프린터 Cubicon 310F를 통해 3D프린팅을 했습니다.
다관절을 위해 세개의 팔을 만들어 6축을 구현했습니다.
 
좌우로 돌리는 하단부에 모터1
가장 힘이 많이 받는 관절에 모터2, 모터3
나머지 관절들의 모터4, 모터5
그리퍼 부분의 모터6
총 6개의 모터를 사용하였습니다.
 
그리퍼 부분은 두개의 래크(직선 톱니)와 피니언(톱니바퀴)을 이용해  한개의 모터로 구현했습니다. 
[Hardware]<div align=center>
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/e7dd4199-df17-4497-b40f-f8dc10834bf7)
<div align=left>
[Software]
SSH 원격 명령으로 라즈베리파이를 제어합니다. 
 
raspicam을 이용해 라즈베리파이 카메라모듈로 영상을 촬영하고 opencv에서 하프원 검출방식을 이용해 원을 판별합니다. 현재 프레임과 전 프레임이 동일할 경우일때의 원만을 타겟으로 하고 판별한 원의 정보를 전달합니다.
 
Opencv를 통해 얻게 된 원의 정보를 기반으로 위와 같은 계산 과정을 통해 모터가 움직여야 할 각도를 계산합니다.
Pigpio를 이용해 모터 동시제어를 가능케 했습니다.
 
아래와 같은 알고리즘으로 장치가 실행되도록 코딩하였습니다.(Using C++)<div align=center>
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/498ceaa9-1d85-4c3c-9a9b-bd804cefa63e)

[Video]
https://www.youtube.com/watch?v=YMNExrljENY&feature=youtu.be
 

