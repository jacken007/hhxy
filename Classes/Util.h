#pragma once
// �����࣬Ҳ�Ǳ������ȫ�ֺ�����
#include "cocos2d.h"

using namespace cocos2d;
class CUtil {
public:
    CUtil();
    ~CUtil();
public:
    static Sprite* createAnimationNode(String sourceId, int frameNum);
};

