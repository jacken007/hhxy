#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // 回调
    static void menuCloseCallback(cocos2d::Ref* pSender);
    void enterBtnCallBack(cocos2d::Ref* pSender, TouchEventType type);
    void backBtnCallBack(cocos2d::Ref* pSender, TouchEventType type);
    void createBtnCallBack(Ref* pSender, TouchEventType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);

private:
    void createMainUiAllAnimation();   // 创建主界面上最下面的动画
    void createMainUiAnimation(String name, int frameNum); 

private:
    Widget* pMainPanel;
    Widget* pLoginPanel;
    Widget* pBgActionPanel;
};

#endif // __HELLOWORLD_SCENE_H__
