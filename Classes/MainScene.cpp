#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "GameScene.h"
#include "Util.h"


USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) {
        return false;
    }

    // 预加载资源
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("ui/main_ui.plist");
    
    // 获取设备信息
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载UI
	auto rootNode = CSLoader::createNode("MainScene4.csb");
	this->addChild(rootNode);
    pMainPanel = dynamic_cast<Widget*>(rootNode->getChildByName("main_ui"));
    pLoginPanel = dynamic_cast<Widget*>(rootNode->getChildByName("login_ui"));
    auto pCreatePanel = dynamic_cast<Widget*>(rootNode->getChildByName("create_ui"));
    pCreatePanel->setEnabled(false);
	auto enterBtn = dynamic_cast<Button*>(pMainPanel->getChildByName("enter_btn"));
	// 进入游戏按钮点击回调
    //pMainPanel->setEnabled(true);
    pLoginPanel->setVisible(false);
    pCreatePanel->setVisible(false);
    if (enterBtn != nullptr) {
		//btn->addClickEventListener(Widget::ccWidgetClickCallback(MainScene::menuCloseCallback));
        enterBtn->addTouchEventListener(this, toucheventselector(MainScene::createBtnCallBack));
	}
    auto backBtn = dynamic_cast<Button*>(pLoginPanel->getChildByName("back_btn"));
    if (backBtn != nullptr) {
        backBtn->addTouchEventListener(this, toucheventselector(MainScene::backBtnCallBack));
    }
    auto createBtn = dynamic_cast<Button*>(pLoginPanel->getChildByName("create_btn"));
    if (createBtn != nullptr) {
        createBtn->addTouchEventListener(this, toucheventselector(MainScene::createBtnCallBack));
    }
	
    // 添加最下面几个人物的动画
    pBgActionPanel = dynamic_cast<Widget*>(rootNode->getChildByName("bg_action"));
    //pBgActionPanel->setEnabled(false);
    if (pBgActionPanel != nullptr) {
        createMainUiAllAnimation();
    }

	// 播放背景音乐
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/csjw.mp3", true);
    
    return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
//核心在这句话，其余都是HelloWorldScene.cpp复制过来的，什么意思不用管，把原本的end()方法，改成切换场景replaceScene()方法。
//CCTransitionMoveInL为左进入特效，0.4f为耗时，越少越快，可以为3.0f等，HelloWorld::scene()就是要切换到的场景
    CCDirector::sharedDirector()->replaceScene(GameScene::createScene());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

/************************************************************************/
/* 功能：进入按钮点击回调
/* 参数：
/* 作者：jacken
/* 日期：2019/08/08                                                                     */
/************************************************************************/
void MainScene::enterBtnCallBack(cocos2d::Ref* pSender, cocos2d::ui::TouchEventType type) {
    switch (type)
    {
    case TouchEventType::TOUCH_EVENT_BEGAN:
        break;
    case TouchEventType::TOUCH_EVENT_ENDED: {
        pMainPanel->setVisible(false);
        pLoginPanel->setVisible(true);
        break;
    }        
    default:
        break;
    }
}

/************************************************************************/
/* 功能：选择人物界面，上一步按钮点击回调
/* 参数：
/* 作者：jacken
/* 日期：2019/08/08                                                                     */
/************************************************************************/
void MainScene::backBtnCallBack(cocos2d::Ref* pSender, TouchEventType type) {
    switch (type)
    {
    case TouchEventType::TOUCH_EVENT_BEGAN:
        break;
    case TouchEventType::TOUCH_EVENT_ENDED: {
        pMainPanel->setVisible(true);
        pLoginPanel->setVisible(false);
        break;
    }
    default:
        break;
    }
}

/************************************************************************/
/* 功能：创建按钮点击回调，这个没有找到相关的资源，暂时先做成进入游戏
/* 参数：
/* 作者：jacken
/* 日期：2019/08/10                                                                     */
/************************************************************************/
void MainScene::createBtnCallBack(Ref* pSender, TouchEventType type) {
    switch (type)
    {
    case TouchEventType::TOUCH_EVENT_BEGAN:
        break;
    case TouchEventType::TOUCH_EVENT_ENDED: {
        CCDirector::sharedDirector()->replaceScene(GameScene::createScene());
        break;
    }
    default:
        break;
    }
}

/************************************************************************/
/* 功能：创建主界面上最下面的几个人的动画
/* 参数：
/* 作者：jacken
/* 日期：2019/08/08                                                                     */
/************************************************************************/
void MainScene::createMainUiAllAnimation() {
    createMainUiAnimation("wukong", 8);
    createMainUiAnimation("bajie", 8);
    createMainUiAnimation("sanzang", 8);
    createMainUiAnimation("wujing", 12);
}

/************************************************************************/
/* 功能：创建某一个角色动画
/* 参数：
/* 作者：jacken
/* 日期：2019/08/08                                                                     */
/************************************************************************/
void MainScene::createMainUiAnimation(String name, int frameNum) {
    // 获取UI上相应的控件
    String* uiImgId = String::create("img_");
    uiImgId->append(name.getCString());
    auto roleImg = dynamic_cast<ImageView*>(pBgActionPanel->getChildByName(uiImgId->getCString()));

    // 创建相应的动画
    String* sourceId = String::create("main/");
    sourceId->append(name.getCString());
    sourceId->append("_");
    Sprite* pSpr = CUtil::createAnimationNode(sourceId->getCString(), frameNum);
    pBgActionPanel->addChild(pSpr);
    pSpr->setPosition(roleImg->getPosition());

    // 隐藏定位图片，也可放到UI中做
    roleImg->setVisible(false);
}


