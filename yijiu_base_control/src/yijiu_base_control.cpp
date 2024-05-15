#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/BatteryState.h"
#include "parammgr.h"
#include "serialInterface.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define SPEED B115200
#define PORT "/dev/move_base"
unsigned char hex_data[] = {0x5a, 0x06, 0x01, 0xf1, 0x00, 0xd7};
void *thread_function(void *arg);
int init_serial(void);
int read_datas_tty(int fd,char *rcv_buf,int sec,int usec);
int main(int argc, char **argv)
{
    // int res,fd ,n;
    // pthread_t a_thread;
    // void *thread_result;
    // fd = init_serial();
    // printf("main fd= %d\n",fd);
    // res = pthread_create(&a_thread,NULL,thread_function,(void *)(&fd));
    // if(res!=0){
    //     perror("Thread creation failed.");
    //     exit(EXIT_FAILURE);
    // }
    // printf("Waiting for thread to finish..\n");
    // printf("Send data\n");
    // while(1)
    // {
    //     n = write(fd, hex_data, sizeof(hex_data)); // 发送数据
    //     if (n < 0)
    //     {
    //         perror("Write error");
    //         break;
    //     }
    //     printf("Send:%d\n", n);
    //     sleep(10);
    // }
    
    // printf("close\n");
    // close(fd);
    // exit(EXIT_SUCCESS); 

    // return 0;


    // ros::init(argc, argv, "yijiu_base_control");

    // // 创建节点句柄
    // ros::NodeHandle nh("~");

    // // 声明一个变量来存储参数值，默认值是 "base_footprint"
    // std::string base_id_param = "base_footprint_old";

    // // 使用getParam()方法获取参数值，如果获取失败，则使用默认值
    // if (!nh.getParam("base_id", base_id_param)) {
    //     ROS_WARN("Failed to get parameter 'base_id'. Using default value.");
    // }

    // // 输出参数值
    // ROS_INFO("base_id_param: %s", base_id_param.c_str());

    // // 这里可以继续使用参数值进行后续操作
    // ParamMgr::Instance()->init();
    // SerialInf::Instance()->init();
    // // 进入ROS循环
    // ros::spin();

    // return 0;

    ros::init(argc, argv, "yijiu_base_control");

    // 创建 ROS 节点句柄
    ros::NodeHandle nh;

    // 创建一个发布者，用于发布 sensor_msgs/BatteryState 类型的消息
    ros::Publisher battery_pub = nh.advertise<sensor_msgs::BatteryState>("/battery", 10);

    // 设置循环的频率
    ros::Rate loop_rate(10); // 发布频率为 10Hz

    while (ros::ok()) {
        // 创建一个 sensor_msgs::BatteryState 消息对象
        sensor_msgs::BatteryState msg;

        // 设置消息字段值
        msg.voltage = 12.3; // 电压，单位：伏特
        msg.current = 0.5; // 电流，单位：安培
        msg.charge = 80.0; // 电池剩余电量，单位：百分比
        msg.capacity = 100.0; // 电池总容量，单位：毫安时

        // 发布消息
        battery_pub.publish(msg);

        // 按照设置的频率休眠，保持循环频率
        loop_rate.sleep();
    }

    return 0;
}

void *thread_function(void *arg)
{
    int len,fd;
    char buf[100];
    fd = *((int *)arg);
    printf("Run thread function:%d\n",fd);
    while(1)
    {
        len = read_datas_tty( fd,buf,0,100000);
        if(len == -1)
            break;
        else if(len == 0)
            {}
        else
        {
            printf("Recive buffer:");
            for (int i = 0; i < len; i++)
            {
                printf("%02X ", buf[i]); // 以十六进制形式打印每个字符
            }
            printf("\n");
        }
    }
    // pthread_exit("Thank you for cpu\n");   
}
int read_datas_tty(int fd,char *rcv_buf,int sec,int usec)
{
    int retval;
    char * tempchar2=rcv_buf;
    fd_set rfds;
    struct timeval tv;
    int ret,len=0;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    while(1)
    {
       FD_ZERO(&rfds);
       FD_SET(fd,&rfds);
       retval = select(fd+1,&rfds,NULL,NULL,&tv);
       if(retval ==-1)
       {
        perror("select()\n");
        close(fd);
        //break;
        return -1;
       }
       else if(retval)
       {
        ret= read(fd,tempchar2++,1);
        len++;
       }
       else
       {
        break;
       }
    }
    //printf("11:%s\n",rcv_buf);
    return len; 
}

int init_serial(void)
{
    int fd = -1;// 初始化串口文件描述符为-1，表示未打开的串口
    struct termios tio ;// 创建一个结构体变量，用于保存串口属性
    fd  = open(PORT, O_RDWR | O_NOCTTY);// 打开串口设备文件，读写方式打开，
    if(fd < 0)// 如果打开串口失败
    {
        perror("InitSerial:open port error");
        return -1;
    }
    tcgetattr(fd, &tio);// 获取当前串口属性，并保存到tio变量中
    cfsetospeed(&tio, SPEED);// 设置串口的发送波特率
    cfsetispeed(&tio, SPEED);// 设置串口的接收波特率
    tio.c_cflag |= CLOCAL | CREAD; // 控制模式标志，启用本地连接和接收使能
    tio.c_cflag &= ~PARENB;// 清除奇偶校验位
    tio.c_cflag &= ~CSTOPB;// 清除停止位设置，即使用一个停止位
    tio.c_cflag &= ~CSIZE;// 清除数据位设置
    tio.c_cflag |=    CS8;// 设置数据位为8位
    //tio.c_cflag &= ~CNEW_RTSCTS;
    tio.c_iflag &= ~(IXON | IXOFF | IXANY);// 输入模式标志，禁用软件流控
    tio.c_cc[VTIME] = 1; // 设置读取数据的超时时间，单位为0.1秒
    tio.c_cc[VMIN] = 20;// 设置读取数据的最小字符数
    tio.c_lflag &= ~(ICANON | ECHO | ECHOE); // 控制字符模式标志，禁用规范模式、回显和擦除字符
    tio.c_oflag &= ~OPOST; // 输出模式标志，禁用处理后的输出
    tcflush(fd,TCIFLUSH);// 清空输入缓冲区
    tcsetattr(fd,TCSAFLUSH,&tio);// 设置串口属性
    tcflush(fd, TCOFLUSH);// 清空输出缓冲区
    return fd; // 返回串口文件描述符
}