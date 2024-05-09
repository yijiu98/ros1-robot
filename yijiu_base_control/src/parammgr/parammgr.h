#pragma once
#include "Macro.h"
class ParamMgr
{
public:
    void init();
    ~ParamMgr();
private:
    DECLARE_SINGLETON(ParamMgr)
};

