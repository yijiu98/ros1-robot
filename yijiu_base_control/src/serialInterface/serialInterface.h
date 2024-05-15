#pragma once
#include "Macro.h"
#include "CircularBuffer.hpp"
#define SPEED B115200
#define PORT "/dev/move_base"



class SerialInf
{
public:
    SerialInf();
    void init();
    ~SerialInf();
    void sendData(const std::vector<unsigned char>& vec);
private:
    int init_serial();
    
    
    
private:
    int read_datas_tty(int fd,unsigned char *rcv_buf,int sec,int usec);
    void serial_send_thread_func();
    void serial_read_thread_func(const int& fd);
    void serial_get_thread_func();

private:
    int fd;
    CircularBuffer<std::vector<unsigned char>> m_recv_Cache{20};
    CircularBuffer<std::vector<unsigned char>> m_send_Cache{20};
    
};

