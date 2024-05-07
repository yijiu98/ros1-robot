#include "ros/ros.h"
#include "bingda_practices/Student.h"
void chatterCallback(const bingda_practices::Student::ConstPtr& msg)
{
    ROS_INFO("Student Name is %s %s;Age is %d ;Socer is %d.", msg->first_name.c_str(),msg->last_name.c_str() ,msg->age,msg->score);
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "msg_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("student_info", 1000, chatterCallback);
    ros::spin();
    return 0;
}