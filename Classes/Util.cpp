#include "Util.h"



CUtil::CUtil()
{
}


CUtil::~CUtil()
{
}

/************************************************************************/
/* 功能：根据传入的资源ID和帧数，创建帧动画
/* 参数：
/*  - String sourceId 资源ID
/*  - int frameNum  帧数
/* 其它：这里，不做资源加载功能，认为需要的资源在之前已经做了合适的加载，如果获取不到资源，会设置断言
/* 作者：jacken
/* 日期：2019/08/08                                                                     */
/************************************************************************/
Sprite* CUtil::createAnimationNode(String sourceId, int frameNum) {
    // 动画资源名都是类似wukong_stand_1 --- wukong_stand_9这样编号的
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
