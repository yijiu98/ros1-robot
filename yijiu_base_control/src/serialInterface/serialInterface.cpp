#include "serialInterface.h"
#include <iostream>
#include <thread>

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

SerialInf::SerialInf()
{

}
SerialInf::~SerialInf()
{

}

void SerialInf::init()
{
    fd = init_serial();
    std::thread serial_read_thread = std::thread(&SerialInf::serial_read_thread_func,this,fd);
    if(serial_read_thread.joinable())
    {
        serial_read_thread.detach();
    }
    std::thread serial_get_thread = std::thread(&SerialInf::serial_get_thread_func,this);
    if(serial_get_thread.joinable())
    {
        serial_get_thread.detach();
    }
    std::thread serial_send_thread = std::thread(&SerialInf::serial_send_thread_func,this);
    if(serial_send_thread.joinable())
    {
        serial_send_thread.detach();
    }
}

void SerialInf::sendData(const std::vector<unsigned char>& vec)
{
    m_send_Cache.put(vec);
}
int SerialInf::init_serial()
{
    int fd_ret = -1;// 初始化串口文件描述符为-1，表示未打开的串口
    struct termios tio ;// 创建一个结构体变量，用于保存串口属性
    fd_ret  = open(PORT, O_RDWR | O_NOCTTY);// 打开串口设备文件，读写方式打开，
    if(fd_ret < 0)// 如果打开串口失败
    {
        perror("InitSerial:open port error");
        return -1;
    }
    tcgetattr(fd_ret, &tio);// 获取当前串口属性，并保存到tio变量中
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
    tcflush(fd_ret,TCIFLUSH);// 清空输入缓冲区
    tcsetattr(fd_ret,TCSAFLUSH,&tio);// 设置串口属性
    tcflush(fd_ret, TCOFLUSH);// 清空输出缓冲区
    return fd_ret; // 返回串口文件描述符
}
void SerialInf::serial_read_thread_func(const int& fd)
{
    int serial_fd = fd;
    unsigned char buf[100];
    while(true)
    {
        int len = read_datas_tty( serial_fd,buf,0,100000);
        if(len == -1)
            break;
        else if(len == 0)
        {

        }
        else
        {
            std::vector<unsigned char> cache{};
            // printf("Recive buffer:");
            for (int i = 0; i < len; i++)
            {
                // printf("%02X ", buf[i]); // 以十六进制形式打印每个字符
                cache.push_back(buf[i]);
            }
            m_recv_Cache.put(cache);
            printf("\n");
        }
    } 
}
void SerialInf::serial_get_thread_func()
{
    while(true)
    {
        std::vector<unsigned char> block = m_recv_Cache.get();
        for(unsigned char num : block)
        {
            // std::cout<<num<<" ";
            // std::cout << std::hex << (static_cast<int>(num) & 0xff) << " ";//这句话会很耗时
        }
        std::cout<<std::endl;
    }
    
}
void SerialInf::serial_send_thread_func()
{
    while(true)
    {
        std::vector<unsigned char> block = m_send_Cache.get();
        size_t size = block.size();
        unsigned char* arr = new unsigned char[size];
        std::cout << "send data:";
        for (size_t i = 0; i < size; ++i) 
        {
            arr[i] = block[i];
            // std::cout << std::hex << (static_cast<int>(arr[i]) & 0xff) << " ";//这句话会很耗时
        }
        int n = write(fd,arr,sizeof(arr));
        if (n < 0)
        {
            perror("Write error");
            break;
        }
        delete[] arr;
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));//ms

    }
}
int SerialInf::read_datas_tty(int fd,unsigned char *rcv_buf,int sec,int usec)
{
    int retval;
    unsigned char * tempchar2=rcv_buf;
    fd_set rfds;
    struct timeval tv;
    int ret,len=0;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    while(1)
    {
        /*这部分代码我理解下select部分，是不是select阻塞，如果有数据就read，
        read也是阻塞的，然后没有数据了就超时，然后退出，
        或者一直没有数据就直接等待超时退出*/
        FD_ZERO(&rfds);
        FD_SET(fd,&rfds);
        retval = select(fd+1,&rfds,NULL,NULL,&tv);
        if(retval ==-1)//select函数调用出现了错误，通常是因为文件描述符集合中的某个文件描述符出现了错误，或者其他错误导致select调用失败
        {
            perror("select()\n");
            close(fd);
            return -1;
        }
        else if(retval)
        {
            ret= read(fd,tempchar2++,1);
            len++;
        }
        else
        {
            break;// 超时退出
        }
    }
    return len; 
}

