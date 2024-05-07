#include "ros/ros.h"
#include "hello.h"

namespace hello_ns {

void MyHello::run(){
    ROS_INFO("自定义头文件的使用....");
}

}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"test_head_node");
    hello_ns::MyHello helloPub;
    helloPub.run();
    return 0;
}