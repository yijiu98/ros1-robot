#pragma once

#include "serialInterface.h"

enum DataGetCmd
{
    DATAGETNONE,
    VERSION,
    BATTERY,
};
class ProtocolMgr
{
public:
    ~ProtocolMgr();
    bool init();
    void sendDataGetCmd(DataGetCmd cmd);
private:
    char crc_1byte(char data);
    char crc_byte(const std::vector<char>& data); 
private:
    std::shared_ptr<SerialInf> m_serialInf{nullptr};
private:
    DECLARE_SINGLETON(ProtocolMgr)
};