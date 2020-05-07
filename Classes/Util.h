#pragma once
// 工具类，也是本程序的全局函数类
#include "cocos2d.h"

using namespace cocos2d;
class CUtil {
public:
    CUtil();
    ~CUtil();
public:
    static Sprite* createAnimationNode(String sourceId, int frameNum);
};

