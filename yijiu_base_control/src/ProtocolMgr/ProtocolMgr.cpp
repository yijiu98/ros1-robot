#include "ProtocolMgr.h"

ProtocolMgr::ProtocolMgr():m_serialInf{std::make_shared<SerialInf>()}
{

}

ProtocolMgr::~ProtocolMgr()
{

}

bool ProtocolMgr::init()
{
    m_serialInf->init();
    return true;
}

void ProtocolMgr::sendDataGetCmd(DataGetCmd cmd)
{
    switch (cmd)
    {
        case VERSION:
        {
            std::vector<unsigned char> vec = {0x5a, 0x06, 0x01, 0xf1, 0x00, 0xd7};
            m_serialInf->sendData(vec);
        }
        break;
        case BATTERY:
        {
            std::vector<unsigned char> vec = {0x5a, 0x06, 0x01, 0x07, 0x00, 0xE4};
            m_serialInf->sendData(vec);
        }
        break;
        default:
        break;
    }
}
char ProtocolMgr::crc_1byte(char data) 
{
    char crc = 0; // 初始化CRC值为0
    for (int i = 0; i < 8; i++) 
    { // 对每一位进行处理
        if ((crc ^ data) & 0x01) 
        { // 检查最低位
            crc ^= 0x18; // 与多项式进行异或
            crc >>= 1; // 向右移一位
            crc |= 0x80; // 最高位设为1
        } 
        else 
        {
            crc >>= 1; // 如果条件不成立，只向右移一位
        }
        data >>= 1; // 输入数据向右移位
    }
    return crc;
}

// 计算数组的CRC
char ProtocolMgr::crc_byte(const std::vector<char>& data) 
{
    char ret = 0; // 初始化CRC结果为0
    for (char byte : data) 
    {
        ret = crc_1byte(ret ^ byte); // 更新CRC值
    }
    return ret;
}