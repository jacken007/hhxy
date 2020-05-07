#pragma once
#include "cocos2d.h"
using namespace cocos2d;
// 全局宏定义
enum SPRITE_ACTION {
    ACTION_STAND = 0,		// 站立
    ACTION_RUN,
    ACTION_NULL,
};

// 这个顺序是精灵动作的序列，很多计算都基于这个序列，
// 不能更改，不然会引起很多的混乱
enum SPRITE_DIRECTION {
    DIR_RB = 0,		// 右下
    DIR_LB,		// 左下
    DIR_LU,
    DIR_RU,
    DIR_B,
    DIR_L,
    DIR_U,
    DIR_R,
    DIR_NULL,
};

const String c_strAction[] ={"stand", "run"};
