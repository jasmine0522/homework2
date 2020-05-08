// include ros library
#include "ros/ros.h"

// include msg library
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>


// include math 
#include <math.h>

using namespace std;

turtlesim::Pose pose;
geometry_msgs::Twist vel_msg;
geometry_msgs::Point goal_point;
//Define a data structure to 3D
struct XYZ{
  float x;
  float y;
  float z;
};
//Declare a variable.Its name is pos_err with XYZ data type
struct XYZ pos_err;

// declare call back function(call back the pose of robot)
void pos_cb(const turtlesim::Pose::ConstPtr& msg)
{
  pose = *msg;
} 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tutorial_1");
  ros::NodeHandle n;

  // declare publisher & subscriber
  ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, pos_cb);
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
  //input your desired position
  ROS_INFO("Please input (x,y). x>0,y>0");
  cout<<"desired_X:";
  cin>>goal_point.x;
  cout<<"desired_Y:";
  cin>>goal_point.y;
  // setting frequency as 10 Hz
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()){

    printf("\ncount : %d\n",count);
    printf("goal x : %f \t y : %f\n",goal_point.x,goal_point.y);
    printf("pose x : %f \t y : %f\n",pose.x,pose.y);
    printf("angular z: %f\n",pose.theta);
    // Calculate position error(feedback term)
    pos_err.x = goal_point.x - pose.x;
    pos_err.y = goal_point.y - pose.y;
    
    //Your error-driven controller design
    float K_linear=0.5;
    float K_angular=4.0;
    float Ex= goal_point.x-pose.x;
    float Ey =  goal_point.y-pose.y;

    //-------------------------------------//
    float distance=abs(sqrt((Ex*Ex)+(Ey*Ey)));
    vel_msg.linear.x =  distance*K_linear;
    

    float desired_angle_goal = atan2f(Ey,goal_point.x-pose.x);
    vel_msg.angular.z = (desired_angle_goal-pose.theta)*K_angular;
    turtlesim_pub.publish(vel_msg);
    if(distance<0.01){
      break;
    }

    count ++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}





