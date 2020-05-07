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


    // 地图
    pMap = CMap::create();
    this->addChild(pMap, 0);    // 地图加到第0层，角色和NPC为第1层，UI为第2层
    // 主角
    pRole = CRole::create();
    this->addChild(pRole, 1);
    pRole->setAction(DIR_RB, ACTION_STAND);
    Point rolePos = pMap->getRoleBasicPos();
    // 相机移动，使主角在中间位置
    moveMap(Point(rolePos.x - visibleSize.width/2, rolePos.y - visibleSize.height/2));
    pRole->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    
    // npc
    npcList.clear();
    std::vector<MapNpcData> npcDataList = pMap->getNpcDataList();
    for (auto npcData : npcDataList) {
        CNpc* pNpc = CNpc::create();
        npcList.push_back(pNpc);
        pNpc->setStrModel(npcData.strModel);
        pMap->addChild(pNpc, 0);    // NPC可以被人挡住
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
        pMap->addChild(pMonster, 0);    // monster可以被人挡住
        pMonster->setElementName(monsterData.name);
        pMonster->setAction(monsterData.dir, monsterData.sprAction);
        pMonster->setPosition(monsterData.pos);
    }
    //UI
    initGameUI();

	// 播放背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/dhw.mp3", true);

	// 监听点击事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

/************************************************************************/
/* 功能：加载游戏UI
/* 参数：
/* 作者：jacken
/* 日期：2020/05/01                                                         */
/************************************************************************/
void GameScene::initGameUI() {
    // 预加载资源
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("ui/game_ui.plist");

    // 获取设备信息
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载UI
    auto rootNode = CSLoader::createNode("ui/GameScene.csb");
    this->addChild(rootNode, 2);
    //Vec2 uiPos;
    rootNode->setPosition(origin);
    pTargetPanel = dynamic_cast<Widget*>(rootNode->getChildByName("tar_head_panel"));
    //pMainPanel = dynamic_cast<Widget*>(rootNode->getChildByName("main_ui"));
    //pLoginPanel = dynamic_cast<Widget*>(rootNode->getChildByName("login_ui"));
    //auto pCreatePanel = dynamic_cast<Widget*>(rootNode->getChildByName("create_ui"));

}

// 将镜头移动到指定位置
void GameScene::moveCamera(float dt, Point offset) {
    // remove的目的，是防止连点的时候，多次执行移动操作，从而把屏幕整飞。
    CCActionManager* actionManager = CCDirector::sharedDirector()->getActionManager();
    actionManager->removeAllActionsFromTarget(this);
    runAction(MoveBy::create(dt, offset));
}

// 移动地图到指定位置
// 其中这个移动有点奇葩，主要为了方便移动角色，所以才反过来移动地图
// 而这里的tarPos也是鼠标点击的位置
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

// Point tarPos 结点坐标，即目标点在场景结点下的坐标
// 返回运动时间
float GameScene::moveRole(Point tarPos) {
    Vec2 dis = tarPos - pRole->getPosition();
    float dt = dis.getLength()/pRole->getSpeed();
    pRole->stopAllActions();

    //开始新的动作
    auto sequence = Sequence::create(
                        MoveTo::create(dt, tarPos), 
                        CallFunc::create(std::bind(&GameScene::CB_afterRoleMoveEnded, this, pRole)),
                        nullptr);
    pRole->runAction(sequence);
    return dt;
}

/************************************************************************/
/* 功能：角色动作执行完的回调，设置角色动画为相应方向的站立
/* 参数：
/* 作者：jacken
/* 日期：2019/08/05                                                                     */
/************************************************************************/
void GameScene::CB_afterRoleMoveEnded(Node* pRender) {
    CCLOG("move ended.");
    pRole->setStandAction();
}

// 相机随主角移动
// tarPos也是目标点在场景结点下的坐标
// 检测目标点的位置，使移动最终位置，不会露出黑边
// 这里，移动的速度跟主角不一定相同
void GameScene::cameraWithRole(float dt, Point tarPos) {
    // 获取实际应该运动到的位置，做限制处理，不至于出现黑边
    auto winSize = Director::getInstance()->getVisibleSize();   // 屏幕尺寸
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

// 移动的总逻辑，根据需要移动角色或者地图
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

// 触屏事件
bool GameScene::onTouchBegan(Touch *pTouch, Event *pEvent) {
	return true;
}
void GameScene::onTouchMoved(Touch *pTouch, Event *pEvent) {
	return ;
}
void GameScene::onTouchEnded(Touch *pTouch, Event *pEvent) {
    Point localPos = pTouch->getLocation();                     // 点击点的世界坐标（屏幕坐标）
    Point tarNodePos = pMap->convertToNodeSpace(localPos);      // 点击点的地图坐标

    // 点击的是障碍，直接不可通过
    if (pMap->isUnpassed(tarNodePos) == true) {
        return;
    }

    // 点击的是NPC，打开对话框UI
    CNpc* pClickNpc = isNpcClicked(tarNodePos);
    if (pClickNpc != nullptr) {
        openNpcUI(pClickNpc);
        return;
    }

    // 如果点到地图，关闭targetUi
    closeTargetUi();

    // 设置角色方向
    SPRITE_DIRECTION dir = getDirection(localPos);
    pRole->setAction(dir, ACTION_RUN);
    
    // 移动，常用只移动地图，不移动相机和角色的方式
    moveMap(localPos, pRole->getSpeed());
    //float dt = moveRole(tarNodePos);
    //cameraWithRole(dt, tarNodePos);
}

/************************************************************************/
/* 功能：某个NPC被点击,如果没有NPC被点击，返回nullptr
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
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
/* 功能：打开NPCUI
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
/************************************************************************/
void GameScene::openNpcUI(CNpc* pClickNpc) {
    // 1. 打开显示目标的UI,设置相关信息
    openTargetUi(pClickNpc->getElementName());

    // 2. 打开NPCUI
}

/************************************************************************/
/* 功能：打开目标UI
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
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
/* 功能：关闭目标UI
/* 参数：
/* 作者：jacken
/* 日期：2020/05/05                                                         */
/************************************************************************/
void GameScene::closeTargetUi() {
    if (pTargetPanel == nullptr) {
        return;
    }

    pTargetPanel->setVisible(false);
}

