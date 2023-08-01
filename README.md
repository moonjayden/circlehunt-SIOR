# circle-hunt
### Circle Hunting Robotic Arm Using Image Processing

@SKKU, 2020<div align=center>
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/15532589-c673-4efe-a4dc-15b6551be17d)

<div align=left>
It is a robotic arm which is made to find only desired object among several objects and move it to the desired position. Import images of objects from camera module and recognize only circular objects among several objects using Opencv.


 
### [Overall Config]
<div align=center>
 
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/1356f2ec-409f-4422-97a9-0431f7fca2d2)

<div align=left>

Camera  
The camera captures objects within the range of the robot's arm. For each image frame, the circle is determined by Opencv and the center coordinate of the circle object is delivered to Raspberry Pi. 

Find the angle of movement of the motors by calculating the angle and distance in the Raspberry Pi and operate the servo motor according to the angle.
 

### [Mechanical Part]  
After designing using Autodest Inventor, we printed through the 3D printer Cubicon 310F in SKKU learning factory.
Use 6 motors -> 4-DOF


### [Hardware]<div align=center>
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/9ee46b1c-4b30-417f-acda-ed2af21ad6ea)

<div align=left>

 
 
### [Software]
Controls Raspberry Pi w. SSH remote control

Using the raspicam, take a video with the Raspberry Pi camera module and use the hough circle detection method on the opencv to determine the circle. 

It targets the smoothness when the current frame and the entire frame are the same, and delivers the information of the determined circle.<div align=center>
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/60005058-27df-40af-a6ba-02cbb579e361)


<div align=left>
Based on the information of the circle obtained through Opencv, through the above calculation process
 
<div align=center>
 
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/136366f7-2e64-414d-b6fe-d2e95ee16588)
![image](https://github.com/moonjayden/circlehunt-SIOR/assets/139466574/067ffd1f-d3b1-4249-9fd6-242e5b5959bd)



<div align=left>
 
### [Video]

[![Video Label](http://img.youtube.com/vi/YMNExrljENY/0.jpg)](https://youtu.be/YMNExrljENY)
 

