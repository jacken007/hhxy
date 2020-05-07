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

    // Ԥ������Դ
    auto pFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFrameCache->addSpriteFramesWithFile("ui/main_ui.plist");
    
    // ��ȡ�豸��Ϣ
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����UI
	auto rootNode = CSLoader::createNode("MainScene4.csb");
	this->addChild(rootNode);
    pMainPanel = dynamic_cast<Widget*>(rootNode->getChildByName("main_ui"));
    pLoginPanel = dynamic_cast<Widget*>(rootNode->getChildByName("login_ui"));
    auto pCreatePanel = dynamic_cast<Widget*>(rootNode->getChildByName("create_ui"));
    pCreatePanel->setEnabled(false);
	auto enterBtn = dynamic_cast<Button*>(pMainPanel->getChildByName("enter_btn"));
	// ������Ϸ��ť����ص�
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
	
    // ��������漸������Ķ���
    pBgActionPanel = dynamic_cast<Widget*>(rootNode->getChildByName("bg_action"));
    //pBgActionPanel->setEnabled(false);
    if (pBgActionPanel != nullptr) {
        createMainUiAllAnimation();
    }

	// ���ű�������
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("./music/csjw.mp3", true);
    
    return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
//��������仰�����඼��HelloWorldScene.cpp���ƹ����ģ�ʲô��˼���ùܣ���ԭ����end()�������ĳ��л�����replaceScene()������
//CCTransitionMoveInLΪ�������Ч��0.4fΪ��ʱ��Խ��Խ�죬����Ϊ3.0f�ȣ�HelloWorld::scene()����Ҫ�л����ĳ���
    CCDirector::sharedDirector()->replaceScene(GameScene::createScene());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

/************************************************************************/
/* ���ܣ����밴ť����ص�
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/08                                                                     */
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
/* ���ܣ�ѡ��������棬��һ����ť����ص�
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/08                                                                     */
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
/* ���ܣ�������ť����ص������û���ҵ���ص���Դ����ʱ�����ɽ�����Ϸ
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/10                                                                     */
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
/* ���ܣ�������������������ļ����˵Ķ���
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/08                                                                     */
/************************************************************************/
void MainScene::createMainUiAllAnimation() {
    createMainUiAnimation("wukong", 8);
    createMainUiAnimation("bajie", 8);
    createMainUiAnimation("sanzang", 8);
    createMainUiAnimation("wujing", 12);
}

/************************************************************************/
/* ���ܣ�����ĳһ����ɫ����
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/08/08                                                                     */
/************************************************************************/
void MainScene::createMainUiAnimation(String name, int frameNum) {
    // ��ȡUI����Ӧ�Ŀؼ�
    String* uiImgId = String::create("img_");
    uiImgId->append(name.getCString());
    auto roleImg = dynamic_cast<ImageView*>(pBgActionPanel->getChildByName(uiImgId->getCString()));

    // ������Ӧ�Ķ���
    String* sourceId = String::create("main/");
    sourceId->append(name.getCString());
    sourceId->append("_");
    Sprite* pSpr = CUtil::createAnimationNode(sourceId->getCString(), frameNum);
    pBgActionPanel->addChild(pSpr);
    pSpr->setPosition(roleImg->getPosition());

    // ���ض�λͼƬ��Ҳ�ɷŵ�UI����
    roleImg->setVisible(false);
}


