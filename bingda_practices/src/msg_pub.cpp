#include "ros/ros.h"
#include "bingda_practices/Student.h"
#include <sstream>
int main(int argc, char **argv)
{
    ros::init(argc, argv, "msg_pub");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<bingda_practices::Student>("student_info", 1000);
    ros::Rate loop_rate(10);
    uint8_t count = 0;
    while (ros::ok())
    {
        bingda_practices::Student student;
        student.first_name = "BingDa";
        student.last_name = "Robot";
        student.age = count;
        student.score = 100;
        chatter_pub.publish(student);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }
    return 0;
}