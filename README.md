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

Camera  
The camera captures objects within the range of the robot's arm. For each image frame, the circle is determined by Opencv and the center coordinate of the circle object is delivered to Raspberry Pi. 

Find the angle of movement of the motors by calculating the angle and distance in the Raspberry Pi and operate the servo motor according to the angle.
 

### [Mechanical Part]  
After designing using Autodest Inventor, we printed through the 3D printer Cubicon 310F in SKKU learning factory.
Use 6 motors -> 4-DOF


### [Hardware]<div align=center>
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/e7dd4199-df17-4497-b40f-f8dc10834bf7)
<div align=left>

 
 
### [Software]
Controls Raspberry Pi w. SSH remote control

Using the raspicam, take a video with the Raspberry Pi camera module and use the hough circle detection method on the opencv to determine the circle. 

It targets the smoothness when the current frame and the entire frame are the same, and delivers the information of the determined circle.<div align=center>
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/79d56f3d-e759-4102-b713-722fb0f89f3d)
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/4664217a-aad3-43f6-8a36-02cdb4682b30)
![Raspberrypi2](https://github.com/moonjayden/circle-hunt/assets/139466574/bef41c04-663b-468b-b209-877e9cfdb09a)

<div align=left>
Based on the information of the circle obtained through Opencv, through the above calculation process
 
<div align=center>
 
![image](https://github.com/moonjayden/circle-hunt/assets/139466574/498ceaa9-1d85-4c3c-9a9b-bd804cefa63e)


<div align=left>
 
### [Video]
https://www.youtube.com/watch?v=YMNExrljENY&feature=youtu.be

 

