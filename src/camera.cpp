// to run: rosrun camera_sched camera

// read image from camera; transform to sensor_msgs::Image and publish only when indicated by scheduler

#include "ros/ros.h"
#include "camera_sched/Trigger.h"
#include "opencv2/opencv.hpp" 
#include "iostream"
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char **argv) 
{
	ros::init(argc, argv, "camera");

	ros::NodeHandle n; 

	// client
	ros::ServiceClient client = n.serviceClient<camera_sched::Trigger>("trigger_camera"); 

	camera_sched::Trigger srv;

	std::cout << "Running camera node." << std::endl;

	// open the first webcam plugged in the computer
	cv::VideoCapture camera(0);
	if (!camera.isOpened()) {
        	std::cerr << "ERROR: Could not open camera" << std::endl;
        	return 1;
    	}

	// image from the webcam
	cv::Mat frame;
        
	// img to publish 
	cv_bridge::CvImage img_bridge;
	sensor_msgs::Image img_msg;

	// advertise topic
	ros::Publisher pub_img = n.advertise<sensor_msgs::Image>("image", 1);
	
	int counter = 1;

	while(ros::ok())
	{

		if (client.call(srv))
		{
			if (srv.response.trigger == 1)
			{
				std::cout << "Service called successfully." << std::endl;
	    			
				// capture the next frame from the webcam
	    			camera >> frame;
				std::cout << "Frame captured." << std::endl;

				// transform from cv to ros
				std_msgs::Header header; // empty header
			 	header.seq = counter; // user defined counter
				header.stamp = ros::Time::now(); // time
				img_bridge = cv_bridge::CvImage(header, sensor_msgs::image_encodings::RGB8, frame);
				img_bridge.toImageMsg(img_msg); // from cv_bridge to sensor_msgs::Image

				// publish
				pub_img.publish(img_msg); 
				ros::spinOnce();
				std::cout << "Image published." << std::endl;

				counter++;
			}
		}
		else
		{
			ROS_ERROR("Failed to call service trigger_camera");
		}
	}
	return 0;
}
