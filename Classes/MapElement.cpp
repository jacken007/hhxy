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
    moveSpeed = 80;    // �����ٶ� 100����

    // Ԥ����ͼƬ��Դ
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("npc/niu2.plist");
    pFrameCache->addSpriteFramesWithFile("npc/jxk.plist");
    pFrameCache->addSpriteFramesWithFile("monster/monster_tianbing.plist");

    // �ŵ�ͼƬ
    auto pBottom = Sprite::create("npc/bottom.png");
    this->addChild(pBottom);
    pBottom->setPositionY(pBottom->getPositionY()-10);
    pBottom->setAnchorPoint(Vec2(0.5f, 0.0f));

    // �����յ���������ص�Node��,
    pNameLable = Label::createWithTTF("", "fonts/msyhbd.ttf", 15.0f);
    pNameLable->setColor(Color3B::GREEN);
    this->addChild(pNameLable, 2);
    Point bottomPos = pBottom->getPosition();
    // ����������ʾ��λ��
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
/* ���ܣ�����ģ�͵Ķ�������Ҫģ�͵Ķ����ͳ���
/* ������
    - SPRITE_DIRECTION dir ģ�͵ĳ���
    - SPRITE_ACTION action ģ�͵Ķ�����վ�������С����ȣ�
/* ���ߣ�jacken
/* ���ڣ�2020/05/02                                                         */
/************************************************************************/
void CMapElement::setAction(SPRITE_DIRECTION dir, SPRITE_ACTION action) {
    // �����ʱ�ĳ���Ͷ�����֮ǰһ������������
    if (action == this->curAction && dir == curDirection) {
        return;
    }
    // �Գ���������ĺ������д���
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

    // �Ƴ��ɶ��������ó��¶���
    // ��������һ��tag���Է�����remove����
    CCActionManager* actionManager = CCDirector::sharedDirector()->getActionManager();
    actionManager->removeAllActionsFromTarget(this->getChildByTag(0));
    this->removeChildByTag(0);
    this->addChild(pSpr, 0, 0);
    pSpr->setAnchorPoint(Vec2(0.5f, 0.0f));
}

/************************************************************************/
/* ���ܣ����ý�ɫΪ��ǰ�����վ������,���������ƶ�����
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/04                                                         */
/************************************************************************/
void CMapElement::setStandAction() {
    setAction(curDirection, ACTION_STAND);
}

/************************************************************************/
/* ���ܣ�����ģ����
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
void CMapElement::setStrModel(const String& strModel) {
    this->strModel = strModel;

    // һ��������ģ�����֣���ͨ�������ļ���ȡģ�͵Ĵ�С�������Լ���contentSize
    // ���������ļ��ж�ȡ��Ӧ�Ķ���ID
    auto pDict = CCDictionary::createWithContentsOfFile("string.xml");
    auto* pRoleDict = (CCDictionary*)(pDict->objectForKey(strModel.getCString()));
    // ��
    int width = dynamic_cast<String*>(pRoleDict->objectForKey("width"))->intValue();
    int height = dynamic_cast<String*>(pRoleDict->objectForKey("height"))->intValue();
    this->setContentSize(Size(width, height));
}

/************************************************************************/
/* ���ܣ���⾫���Ƿ����ĳ���㡣�����ĵ��ǵ�ͼ����
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
bool CMapElement::isContainsPoint(const Point& tarMapPos) {
    float x = getPositionX();
    float y = getPositionY();
    float width = getContentSize().width;
    float height = getContentSize().height;
    Vec2 anchorPoint = getAnchorPoint();
    // �����д���к����ص�����,
    // ע�͵���Ӧ������ȷ������,����ʵ�ʵĽ������ܴ�
    // û�и����������������
    // ��ʱ��ôд����˹���,�����кܴ������
    x = x - width;
    //x = x - width * anchorPoint.x;
    //y = y - height * anchorPoint.y;

    Rect rect = Rect(x, y, width, height);
    // DEBUG, ��ʾ��������ĺ���
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawRect(Vec2(x, y), Vec2(x + width, y + height), Color4F::BLUE);
    auto pMap = this->getParent();
    pMap->addChild(drawNode, 100);
    // end debug
    return rect.containsPoint(tarMapPos);
}

/************************************************************************/
/* ���ܣ�����ģ�͵�ID�ͳ���,��ȡ����ID
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/04                                                         */
/************************************************************************/
cocos2d::String CMapElement::getActionStr(SPRITE_DIRECTION dir, SPRITE_ACTION action, int& actionNum) {
    // ���������ļ��ж�ȡ��Ӧ�Ķ���ID
    auto pDict = CCDictionary::createWithContentsOfFile("string.xml");
    auto* pRoleDict = (CCDictionary*)(pDict->objectForKey(strModel.getCString()));
    // ���嶯����վ/�ܵȣ�������֡ID
    auto* pRoleActionDict = dynamic_cast<CCDictionary*>(pRoleDict->objectForKey(c_strAction[action].getCString()));
    String strRoleAction = dynamic_cast<String*>(pRoleActionDict->objectForKey("actionId"))->getCString();
    actionNum = dynamic_cast<String*>(pRoleActionDict->objectForKey("actionNum"))->intValue();

    // ��ϳ�����ϳ���Ҫ��ID������������λ��
    char str[100] = { 0 };
    sprintf(str, "%s-%02d", strRoleAction.getCString(), dir);
    strRoleAction = str;
    return strRoleAction;
}
