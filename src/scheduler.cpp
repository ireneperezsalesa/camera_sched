// to run: roslaunch camera_sched params.launch first and then rosrun camera_sched scheduler

// read schedule parameters from a yaml file, trigger camera node

#include "ros/ros.h"
#include "camera_sched/Trigger.h"

float t1;
float t2;
float t3;
float last;


bool wait_trigger(camera_sched::Trigger::Request  &req,
         camera_sched::Trigger::Response &res) 
{
	if (last == t3)
	{ 
		ros::Duration(t1).sleep();
		res.trigger = 1;
		last = t1;
		std::cout << "Waited " << t1 << " seconds." << std::endl;
	}
	else if (last == t1)
	{ 
		ros::Duration(t2).sleep();
		res.trigger = 1;
		last = t2;
		std::cout << "Waited " << t2 << " seconds." << std::endl;
	}
	else 
	{ 
		ros::Duration(t3).sleep();
		res.trigger = 1;
		last = t3;
		std::cout << "Waited " << t3 << " seconds." << std::endl;
	}	
	// this might not be the most efficient way of doing this
	return true;
} // returns true if service was successful

int main(int argc, char **argv) 
{
	// Initialization
	ros::init(argc, argv, "scheduler");

	// ROS node handle
	ros::NodeHandle n;

	std::cout << "Running scheduler." << std::endl;

	// get parameters for scheduling
	n.getParam("/time1", t1);
	n.getParam("/time2", t2);
	n.getParam("/time3", t3);

	std::cout << "Parameters: \n" << "t1 = " << t1 << "; t2 = " << t2 << "; t3 = " << t3  << std::endl;

	// advertise service
	ros::ServiceServer service = n.advertiseService("trigger_camera", wait_trigger);

	std::cout << "Waiting for client." << std::endl;
	
	ros::spin();
	
	return 0;
}
