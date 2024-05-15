#include "parammgr.h"
#include <iostream>

ParamMgr::ParamMgr()
{

}
ParamMgr::~ParamMgr()
{

}

void ParamMgr::init(ros::NodeHandle& nh)
{
    
    // 使用getParam()方法获取参数值，如果获取失败，则使用默认值
    if (!nh.getParam("yijiu_base_control/odom_topic", odom_topic)) 
    {
        ROS_WARN("Failed to get parameter 'odom_topic'. Using default value.");
    }
    // else
    // {
    //     ROS_INFO("odom_topic: %s", odom_topic.c_str());
    // }
    if (!nh.getParam("yijiu_base_control/battery_topic", battery_topic)) 
    {
        ROS_WARN("Failed to get parameter 'battery_topic'. Using default value.");
    }
    // else
    // {
    //     ROS_INFO("battery_topic: %s", battery_topic.c_str());
    // }
    if (!nh.getParam("yijiu_base_control/cmd_vel_topic", cmd_vel_topic)) 
    {
        ROS_WARN("Failed to get parameter 'cmd_vel_topic'. Using default value.");
    }
    // else
    // {
    //     ROS_INFO("cmd_vel_topic: %s", cmd_vel_topic.c_str());
    // }
}