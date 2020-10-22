cmake_minimum_required(VERSION 3.0.2)
project(camera_sched)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  message_generation
  cv_bridge
)

## Generate services in the 'srv' folder
 add_service_files(
   FILES
   Trigger.srv
 )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
   sensor_msgs
 )

catkin_package()

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Mark other files for installation (e.g. launch and bag files, etc.)
 install(FILES
   launch/params.launch
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
 )

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11

add_executable(scheduler src/scheduler.cpp)
target_link_libraries(scheduler 
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)
add_dependencies(scheduler camera_sched_gencpp)
add_dependencies(scheduler camera_sched_generate_messages_cpp)

add_executable(camera src/camera.cpp)
target_link_libraries(camera 
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)
add_dependencies(camera camera_sched_gencpp)



