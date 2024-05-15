#pragma once
#include "Macro.h"
#include "ros/ros.h"
class ParamMgr
{
public:
    void init(ros::NodeHandle& nh);
    ~ParamMgr();
    inline std::string get_odom_topic(){return odom_topic;}
    inline std::string get_battery_topic(){return battery_topic;}
    inline std::string get_cmd_vel_topic(){return cmd_vel_topic;}
private:
    std::string odom_topic{"odom_topic"};
    std::string battery_topic{"battery_topic"};
    std::string cmd_vel_topic{"cmd_vel_topic"};
private:
    DECLARE_SINGLETON(ParamMgr)
};

