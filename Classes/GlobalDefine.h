#pragma once
#include "cocos2d.h"
using namespace cocos2d;
// ȫ�ֺ궨��
enum SPRITE_ACTION {
    ACTION_STAND = 0,		// վ��
    ACTION_RUN,
    ACTION_NULL,
};

// ���˳���Ǿ��鶯�������У��ܶ���㶼����������У�
// ���ܸ��ģ���Ȼ������ܶ�Ļ���
enum SPRITE_DIRECTION {
    DIR_RB = 0,		// ����
    DIR_LB,		// ����
    DIR_LU,
    DIR_RU,
    DIR_B,
    DIR_L,
    DIR_U,
    DIR_R,
    DIR_NULL,
};

const String c_strAction[] ={"stand", "run"};
