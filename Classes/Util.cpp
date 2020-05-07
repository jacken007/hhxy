#include "Util.h"



CUtil::CUtil()
{
}


CUtil::~CUtil()
{
}

/************************************************************************/
/* ���ܣ����ݴ������ԴID��֡��������֡����
/* ������
/*  - String sourceId ��ԴID
/*  - int frameNum  ֡��
/* ���������������Դ���ع��ܣ���Ϊ��Ҫ����Դ��֮ǰ�Ѿ����˺��ʵļ��أ������ȡ������Դ�������ö���
/* ���ߣ�jacken
/* ���ڣ�2019/08/08                                                                     */
/************************************************************************/
Sprite* CUtil::createAnimationNode(String sourceId, int frameNum) {
    // ������Դ����������wukong_stand_1 --- wukong_stand_9������ŵ�
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(frameNum);
    char str[100] = { 0 };
    for (int i = 1; i < frameNum+1; i++) {
        sprintf(str, "%s%d.png", sourceId.getCString(), i);
        CCSpriteFrame *frame = pFrameCache->spriteFrameByName(str);
        CCASSERT(frame != nullptr, "animation %s create failed.", str);
        animFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);
    Sprite* pSpr = Sprite::create();
    pSpr->runAction(RepeatForever::create(animate));
    return pSpr;
}
