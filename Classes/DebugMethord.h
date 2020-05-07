#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class CDebugMethord {
public:
    CDebugMethord();
    static CDebugMethord* getInstance() {
        if (pInstance == nullptr) {
            pInstance = new CDebugMethord();
        }
        return pInstance;
    }
public:
    void DB_DrawRect(Rect rect);
    
private:
    static CDebugMethord* pInstance;
};