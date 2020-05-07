#include "MapElement.h"

bool CMapElement::init() {
    //////////////////////////////
    // 1. super init first
    if (!Sprite::init()) {
        return false;
    }
    this->setAnchorPoint(Vec2(0.5f, .0f));

    curAction = ACTION_NULL;
    curDirection = DIR_NULL;
    moveSpeed = 80;    // 正常速度 100左右

    // 预加载图片资源
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("npc/niu2.plist");
    pFrameCache->addSpriteFramesWithFile("npc/jxk.plist");
    pFrameCache->addSpriteFramesWithFile("monster/monster_tianbing.plist");

    // 脚底图片
    auto pBottom = Sprite::create("npc/bottom.png");
    this->addChild(pBottom);
    pBottom->setPositionY(pBottom->getPositionY()-10);
    pBottom->setAnchorPoint(Vec2(0.5f, 0.0f));

    // 创建空的姓名板挂载到Node上,
    pNameLable = Label::createWithTTF("", "fonts/msyhbd.ttf", 15.0f);
    pNameLable->setColor(Color3B::GREEN);
    this->addChild(pNameLable, 2);
    Point bottomPos = pBottom->getPosition();
    // 设置名字显示的位置
    pNameLable->setPosition(Point(bottomPos.x, bottomPos.y + 120));
    pNameLable->setAnchorPoint(Vec2(0.5f, 0.0f));
    return true;
}

void CMapElement::setElementName(const String& name) {
    this->name = name;
    if (pNameLable != nullptr) {
        pNameLable->setString(this->name.getCString());
    }
}

/************************************************************************/
/* 功能：设置模型的动作，需要模型的动作和朝向
/* 参数：
    - SPRITE_DIRECTION dir 模型的朝向
    - SPRITE_ACTION action 模型的动作（站、立、行、死等）
/* 作者：jacken
/* 日期：2020/05/02                                                         */
/************************************************************************/
void CMapElement::setAction(SPRITE_DIRECTION dir, SPRITE_ACTION action) {
    // 如果当时的朝向和动作和之前一样，不做处理
    if (action == this->curAction && dir == curDirection) {
        return;
    }
    // 对朝向跟动作的合理化进行处理
    CCAssert(dir != DIR_NULL, "direction is error");
    CCAssert(action != ACTION_NULL, "action is error");
    curDirection = dir;
    curAction = action;

    int actionNum = 0;
    String strActionId = this->getActionStr(dir, action, actionNum);

    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    Vector<SpriteFrame*> animFrames;
    animFrames.reserve(actionNum);
    char str[100] = { 0 };
    for (int i = 0; i < actionNum; i++) {
        sprintf(str, "%s%03d.png", strActionId.getCString(), i);
        CCSpriteFrame *frame = pFrameCache->spriteFrameByName(str);
        if (frame == nullptr) {
            CCLOG("frame %s is null", str);
            CCASSERT(frame != nullptr, "");
        }
        animFrames.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    Animate* animate = Animate::create(animation);
    Sprite* pSpr = Sprite::create();
    pSpr->runAction(RepeatForever::create(animate));

    // 移除旧动画，设置成新动画
    // 动画设置一个tag，以方便作remove操作
    CCActionManager* actionManager = CCDirector::sharedDirector()->getActionManager();
    actionManager->removeAllActionsFromTarget(this->getChildByTag(0));
    this->removeChildByTag(0);
    this->addChild(pSpr, 0, 0);
    pSpr->setAnchorPoint(Vec2(0.5f, 0.0f));
}

/************************************************************************/
/* 功能：设置角色为当前朝向的站立动作,经常用作移动结束
/* 参数：
/* 作者：jacken
/* 日期：2020/05/04                                                         */
/************************************************************************/
void CMapElement::setStandAction() {
    setAction(curDirection, ACTION_STAND);
}

/************************************************************************/
/* 功能：设置模型名
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
/************************************************************************/
void CMapElement::setStrModel(const String& strModel) {
    this->strModel = strModel;

    // 一旦设置了模型名字，就通过配置文件获取模型的大小，设置自己的contentSize
    // 先在配置文件中读取相应的动画ID
    auto pDict = CCDictionary::createWithContentsOfFile("string.xml");
    auto* pRoleDict = (CCDictionary*)(pDict->objectForKey(strModel.getCString()));
    // 宽
    int width = dynamic_cast<String*>(pRoleDict->objectForKey("width"))->intValue();
    int height = dynamic_cast<String*>(pRoleDict->objectForKey("height"))->intValue();
    this->setContentSize(Size(width, height));
}

/************************************************************************/
/* 功能：检测精灵是否包含某个点。给定的点是地图坐标
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
/************************************************************************/
bool CMapElement::isContainsPoint(const Point& tarMapPos) {
    float x = getPositionX();
    float y = getPositionY();
    float width = getContentSize().width;
    float height = getContentSize().height;
    Vec2 anchorPoint = getAnchorPoint();
    // 这里的写法有很严重的问题,
    // 注释掉的应该是正确的姿势,但是实际的结果问题很大
    // 没有搞明白哪里出了问题
    // 暂时这么写完成了功能,但是有很大的问题
    x = x - width;
    //x = x - width * anchorPoint.x;
    //y = y - height * anchorPoint.y;

    Rect rect = Rect(x, y, width, height);
    // DEBUG, 显示检测点击与否的盒子
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawRect(Vec2(x, y), Vec2(x + width, y + height), Color4F::BLUE);
    auto pMap = this->getParent();
    pMap->addChild(drawNode, 100);
    // end debug
    return rect.containsPoint(tarMapPos);
}

/************************************************************************/
/* 功能：根据模型的ID和朝向,获取动作ID
/* 参数：
/* 作者：jacken
/* 日期：2020/05/04                                                         */
/************************************************************************/
cocos2d::String CMapElement::getActionStr(SPRITE_DIRECTION dir, SPRITE_ACTION action, int& actionNum) {
    // 先在配置文件中读取相应的动画ID
    auto pDict = CCDictionary::createWithContentsOfFile("string.xml");
    auto* pRoleDict = (CCDictionary*)(pDict->objectForKey(strModel.getCString()));
    // 具体动作（站/跑等）的序列帧ID
    auto* pRoleActionDict = dynamic_cast<CCDictionary*>(pRoleDict->objectForKey(c_strAction[action].getCString()));
    String strRoleAction = dynamic_cast<String*>(pRoleActionDict->objectForKey("actionId"))->getCString();
    actionNum = dynamic_cast<String*>(pRoleActionDict->objectForKey("actionNum"))->intValue();

    // 结合朝向，组合成需要的ID（不包含后两位）
    char str[100] = { 0 };
    sprintf(str, "%s-%02d", strRoleAction.getCString(), dir);
    strRoleAction = str;
    return strRoleAction;
}
