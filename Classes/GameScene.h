#pragma once

#include <vector>
#include "cocos2d.h"
#include "Role.h"
#include "Map.h"
#include "Npc.h"
#include "Monster.h"
#include "ui/UIWidget.h"

using namespace cocos2d;
using namespace ui;

class GameScene : public cocos2d::Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// 触屏事件
	//void registerWithTouchDispatcher();	// 注册触摸事件
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	void moveCamera(float dt, Point offset);	// 将镜头移动到指定位置
    float moveRole(Point tarPos);                // 移动到目标点，目标点坐标是结点坐标
    void moveMap(Point clickPos, float speed =1000000);                 // 移动地图到指定位置
    void cameraWithRole(float dt, Point tarPos);    // 相机随主角移动
    //void move(Point tarPos);                    // 移动的总逻辑，根据需要移动角色或者地图
    SPRITE_DIRECTION getDirection(Point tarPos);  // 获取点击后角色的朝向 

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

    // UI相关
private:
    void initGameUI();      // 加载游戏UI
    CNpc* isNpcClicked(const Point& tarMapPos);    // 某个NPC被点击
    void openNpcUI(CNpc* pClickNpc);       // 打开NPCUI
    void openTargetUi(const String& name);  // 打开目标UI
    void closeTargetUi();                   // 关闭目标UI
    CMonster* isMonsterClicked(const Point& tarMapPos);
    // 回调函数
private:
    void CB_afterRoleMoveEnded(Node* pRender);       // 角色移动后的回调


private:
    CMap* pMap;
	CRole* pRole;
    std::vector<CNpc*> npcList;     // npc列表,存储所有NPC指针
    std::vector<CMonster*> monsterList;     // monster列表

    // ui相关
private:
    Widget* pTargetPanel;           // 显示目标的容器
};

