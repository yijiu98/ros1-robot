#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/BatteryState.h"
#include "parammgr.h"
// #include "serialInterface.h"
#include "ProtocolMgr.h"

void timerCallback(const ros::TimerEvent&); 
int main(int argc, char **argv)
{

    //get params
    ros::init(argc, argv, "yijiu_base_control");

    // 创建节点句柄
    ros::NodeHandle nh;
    ParamMgr::Instance()->init(nh);

    ROS_INFO("odom_topic: %s", ParamMgr::Instance()->get_odom_topic().c_str());


    ROS_INFO("battery_topic: %s", ParamMgr::Instance()->get_battery_topic().c_str());


    ROS_INFO("cmd_vel_topic: %s", ParamMgr::Instance()->get_cmd_vel_topic().c_str());

    ProtocolMgr::Instance()->init();
    ProtocolMgr::Instance()->sendDataGetCmd(VERSION);
    sleep(5);
    ros::Timer timer = nh.createTimer(ros::Duration(1/1), timerCallback);
    // 进入ROS循环
    ros::spin();

    return 0;





   

    // ros::init(argc, argv, "yijiu_base_control");

    // // 创建 ROS 节点句柄
    // ros::NodeHandle nh("~");

    // // 创建一个发布者，用于发布 sensor_msgs/BatteryState 类型的消息
    // ros::Publisher battery_pub = nh.advertise<sensor_msgs::BatteryState>("/battery", 10);

    // // 设置循环的频率
    // ros::Rate loop_rate(10); // 发布频率为 10Hz

    // while (ros::ok()) {
    //     // 创建一个 sensor_msgs::BatteryState 消息对象
    //     sensor_msgs::BatteryState msg;

    //     // 设置消息字段值
    //     msg.voltage = 12.3; // 电压，单位：伏特
    //     msg.current = 0.5; // 电流，单位：安培
    //     msg.charge = 80.0; // 电池剩余电量，单位：百分比
    //     msg.capacity = 100.0; // 电池总容量，单位：毫安时

    //     // 发布消息
    //     battery_pub.publish(msg);

    //     // 按照设置的频率休眠，保持循环频率
    //     loop_rate.sleep();
    // }

    // return 0;
}

void timerCallback(const ros::TimerEvent&) 
{
    ros::Time current_time = ros::Time::now();

    // 打印当前时间（秒和纳秒）
    std::cout << "Current time: " 
              << current_time.sec  << std::endl;
    ProtocolMgr::Instance()->sendDataGetCmd(BATTERY);
}