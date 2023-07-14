cmake_minimum_required(VERSION 2.8)

project( servo)

set(CMAKE_MODULE_PATH /home/pi/pigpio-master/util)
find_library(wiringPi_LIB wiringPi)
find_package(pigpio REQUIRED)
find_package( OpenCV REQUIRED )
find_package(raspicam REQUIRED)

include_directories( ${pigpio_INCLUDE_DIRS} )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( servo servo.cpp)

target_link_libraries( servo ${pigpio_LIBRARY} )
target_link_libraries( servo ${pigpiod_if_LIBRARY})
target_link_libraries( servo ${pigpiod_if2_LIBRARY})
target_link_libraries( servo ${OpenCV_LIBS} )
target_link_libraries (servo ${raspicam_LIBS})
target_link_libraries (servo ${raspicam_CV_LIBS})
target_link_libraries(servo ${wiringPi_LIB})


