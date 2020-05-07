#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "spine/spine-cocos2dx.h"
#include "DebugMethord.h"
#include "ui/UIText.h"


USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


    // ��ͼ
    pMap = CMap::create();
    this->addChild(pMap, 0);    // ��ͼ�ӵ���0�㣬��ɫ��NPCΪ��1�㣬UIΪ��2��
    // ����
    pRole = CRole::create();
    this->addChild(pRole, 1);
    pRole->setAction(DIR_RB, ACTION_STAND);
    Point rolePos = pMap->getRoleBasicPos();
    // ����ƶ���ʹ�������м�λ��
    moveMap(Point(rolePos.x - visibleSize.width/2, rolePos.y - visibleSize.height/2));
    pRole->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    
    // npc
    npcList.clear();
    std::vector<MapNpcData> npcDataList = pMap->getNpcDataList();
    for (auto npcData : npcDataList) {
        CNpc* pNpc = CNpc::create();
        npcList.push_back(pNpc);
        pNpc->setStrModel(npcData.strModel);
        pMap->addChild(pNpc, 0);    // NPC���Ա��˵�ס
        pNpc->setElementName(npcData.name);
        pNpc->setAction(npcData.dir, npcData.sprAction);
        pNpc->setPosition(npcData.pos);
        // debug
        //DrawNode* drawNode = DrawNode::create();
        //drawNode->drawRect(Point::ZERO, pNpc->getContentSize(), Color4F::BLUE);
        //pNpc->addChild(drawNode, 100);
        //CDebugMethord::getInstance()->DB_DrawRect(Rect(npcData.pos, pNpc->getContentSize()));
    }

    // monster
    monsterList.clear();
    std::vector<MapMonsterData> monsterDataList = pMap->getMonsterDataList();
    for (auto monsterData : monsterDataList) {
        CMonster* pMonster = CMonster::create();
        monsterList.push_back(pMonster);
        pMonster->setStrModel(monsterData.strModel);
        pMap->addChild(pMonster, 0);    // monster���Ա��˵�ס
        pMonster->setElementName(monsterData.name);
        pMonster->setAction(monsterData.dir, monsterData.sprAction);
        pMonster->setPosition(monsterData.pos);
    }
    //UI
    initGameUI();

	// ���ű�������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/dhw.mp3", true);

	// ��������¼�
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

/************************************************************************/
/* ���ܣ�������ϷUI
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/01                                                         */
/************************************************************************/
void GameScene::initGameUI() {
    // Ԥ������Դ
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("ui/game_ui.plist");

    // ��ȡ�豸��Ϣ
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����UI
    auto rootNode = CSLoader::createNode("ui/GameScene.csb");
    this->addChild(rootNode, 2);
    //Vec2 uiPos;
    rootNode->setPosition(origin);
    pTargetPanel = dynamic_cast<Widget*>(rootNode->getChildByName("tar_head_panel"));
    //pMainPanel = dynamic_cast<Widget*>(rootNode->getChildByName("main_ui"));
    //pLoginPanel = dynamic_cast<Widget*>(rootNode->getChildByName("login_ui"));
    //auto pCreatePanel = dynamic_cast<Widget*>(rootNode->getChildByName("create_ui"));

}

// ����ͷ�ƶ���ָ��λ��
void GameScene::moveCamera(float dt, Point offset) {
    // remove��Ŀ�ģ��Ƿ�ֹ�����ʱ�򣬶��ִ���ƶ��������Ӷ�����Ļ���ɡ�
    CCActionManager* actionManager = CCDirector::sharedDirector()->getActionManager();
    actionManager->removeAllActionsFromTarget(this);
    runAction(MoveBy::create(dt, offset));
}

// �ƶ���ͼ��ָ��λ��
// ��������ƶ��е����⣬��ҪΪ�˷����ƶ���ɫ�����Բŷ������ƶ���ͼ
// �������tarPosҲ���������λ��
void GameScene::moveMap(Point tarPos, float speed) {
    Vec2 dis = pRole->getPosition() - tarPos;
    float dt = dis.getLength() / speed;
    pMap->stopAllActions();
    //pMap->runAction(MoveBy::create(dt, dis));

    auto sequence = Sequence::create(
        MoveBy::create(dt, dis),
        CallFunc::create(std::bind(&GameScene::CB_afterRoleMoveEnded, this, pMap)),
        nullptr);
    pMap->runAction(sequence);
}

// Point tarPos ������꣬��Ŀ����ڳ�������µ�����
// �����˶�ʱ��
float GameScene::moveRole(Point tarPos) {
    Vec2 dis = tarPos - pRole->getPosition();
    float dt = dis.getLength()/pRole->getSpeed();
    pRole->stopAllActions();

    //��ʼ�µĶ���
    auto sequence = Sequence::create(
                        MoveTo::create(dt, tarPos), 
                        CallFunc::create(std::bind(&GameScene::CB_afterRoleMoveEnded, this, pRole)),
                        nullptr);
    pRole->runAction(sequence);
    return dt;
}

/************************************************************************/
/* ���ܣ���ɫ����ִ����Ļص������ý�ɫ����Ϊ��Ӧ�����վ��
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/05                                                                     */
/************************************************************************/
void GameScene::CB_afterRoleMoveEnded(Node* pRender) {
    CCLOG("move ended.");
    pRole->setStandAction();
}

// ����������ƶ�
// tarPosҲ��Ŀ����ڳ�������µ�����
// ���Ŀ����λ�ã�ʹ�ƶ�����λ�ã�����¶���ڱ�
// ����ƶ����ٶȸ����ǲ�һ����ͬ
void GameScene::cameraWithRole(float dt, Point tarPos) {
    // ��ȡʵ��Ӧ���˶�����λ�ã������ƴ��������ڳ��ֺڱ�
    auto winSize = Director::getInstance()->getVisibleSize();   // ��Ļ�ߴ�
    float x = MAX(tarPos.x, winSize.width/2);
    float y = MAX(tarPos.y, winSize.height/2);
    const Vec2 mapSize = pMap->getMapSizeByPix();
    x = MIN(x, mapSize.x - winSize.width/2);
    y = MIN(y, mapSize.y - winSize.height/2);

    Point acturalPos = Point(x, y);
    const TMXTiledMap* pTiledMap = pMap->getTiledMap();
    Point curCenter = pTiledMap->convertToNodeSpace(Vec2(winSize.width/2, winSize.height/2));
    Vec2 dis = curCenter - acturalPos;
    moveCamera(dt, dis);
}

// �ƶ������߼���������Ҫ�ƶ���ɫ���ߵ�ͼ
//void GameScene::move(Point tarPos) {
//
//}

SPRITE_DIRECTION GameScene::getDirection(Point tarPos) {  
    Point rolePos = this->convertToWorldSpace(pRole->getPosition());
    Point dis = tarPos - rolePos;
    auto rat = Point(dis.x, dis.y).getAngle() * 180 / 3.14;
    if (rat < 0) {
        rat += 360;
    }

    SPRITE_DIRECTION dir;
    if (rat > 22.5 && rat <= 67.5) {
        dir = DIR_RU;
    }
    else if (rat > 67.5 && rat <= 112.5) {
        dir = DIR_U;
    }
    else if (rat > 112.5 && rat <= 157.5) {
        dir = DIR_LU;
    }
    else if (rat > 157.5 && rat <= 202.5) {
        dir = DIR_L;
    }
    else if (rat > 202.5 && rat <= 247.5) {
        dir = DIR_LB;
    }
    else if (rat > 247.5 && rat <= 292.5) {
        dir = DIR_B;
    }
    else if (rat > 292.5 && rat <= 337.5) {
        dir = DIR_RB;
    }
    else if ((rat > 337.5 && rat <= 360) || (rat >= 0 && rat <= 22.5)) {
        dir = DIR_R;
    }
    return dir;
}

// �����¼�
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent) {
	return true;
}
void GameScene::onTouchMoved(Touch *pTouch, Event *pEvent) {
	return ;
}
void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent) {
    Point localPos = pTouch->getLocation();                     // �������������꣨��Ļ���꣩
    Point tarNodePos = pMap->convertToNodeSpace(localPos);      // �����ĵ�ͼ����

    // ��������ϰ���ֱ�Ӳ���ͨ��
    if (pMap->isUnpassed(tarNodePos) == true) {
        return;
    }

    // �������NPC���򿪶Ի���UI
    CNpc* pClickNpc = isNpcClicked(tarNodePos);
    if (pClickNpc != nullptr) {
        openNpcUI(pClickNpc);
        return;
    }

    // ����㵽��ͼ���ر�targetUi
    closeTargetUi();

    // ���ý�ɫ����
    SPRITE_DIRECTION dir = getDirection(localPos);
    pRole->setAction(dir, ACTION_RUN);
    
    // �ƶ�������ֻ�ƶ���ͼ�����ƶ�����ͽ�ɫ�ķ�ʽ
    moveMap(localPos, pRole->getSpeed());
    //float dt = moveRole(tarNodePos);
    //cameraWithRole(dt, tarNodePos);
}

/************************************************************************/
/* ���ܣ�ĳ��NPC�����,���û��NPC�����������nullptr
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
CNpc* GameScene::isNpcClicked(const Point& tarMapPos) {
    for (auto pNpc : npcList) {
        if (pNpc->isContainsPoint(tarMapPos)) {
            return pNpc;
        }
    }
    return nullptr;
}

/************************************************************************/
/* ���ܣ���NPCUI
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
void GameScene::openNpcUI(CNpc* pClickNpc) {
    // 1. ����ʾĿ���UI,���������Ϣ
    openTargetUi(pClickNpc->getElementName());

    // 2. ��NPCUI
}

/************************************************************************/
/* ���ܣ���Ŀ��UI
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
void GameScene::openTargetUi(const String& name) {
    if (pTargetPanel == nullptr) {
        return;
    }

    pTargetPanel->setVisible(true);

    Text* pNameLable = dynamic_cast<Text*>(pTargetPanel->getChildByName("role_name"));
    pNameLable->setText(name.getCString());
}

/************************************************************************/
/* ���ܣ��ر�Ŀ��UI
/* ������
/* ���ߣ�jacken
/* ���ڣ�2020/05/05                                                         */
/************************************************************************/
void GameScene::closeTargetUi() {
    if (pTargetPanel == nullptr) {
        return;
    }

    pTargetPanel->setVisible(false);
}

