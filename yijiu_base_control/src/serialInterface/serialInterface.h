#pragma once
#include "Macro.h"
class SerialInf
{
public:
    void init();
    ~SerialInf();
private:
    DECLARE_SINGLETON(SerialInf)
};

