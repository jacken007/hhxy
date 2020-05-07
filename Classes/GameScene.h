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

	// �����¼�
	//void registerWithTouchDispatcher();	// ע�ᴥ���¼�
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	void moveCamera(float dt, Point offset);	// ����ͷ�ƶ���ָ��λ��
    float moveRole(Point tarPos);                // �ƶ���Ŀ��㣬Ŀ��������ǽ������
    void moveMap(Point clickPos, float speed =1000000);                 // �ƶ���ͼ��ָ��λ��
    void cameraWithRole(float dt, Point tarPos);    // ����������ƶ�
    //void move(Point tarPos);                    // �ƶ������߼���������Ҫ�ƶ���ɫ���ߵ�ͼ
    SPRITE_DIRECTION getDirection(Point tarPos);  // ��ȡ������ɫ�ĳ��� 

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

    // UI���
private:
    void initGameUI();      // ������ϷUI
    CNpc* isNpcClicked(const Point& tarMapPos);    // ĳ��NPC�����
    void openNpcUI(CNpc* pClickNpc);       // ��NPCUI
    void openTargetUi(const String& name);  // ��Ŀ��UI
    void closeTargetUi();                   // �ر�Ŀ��UI
    CMonster* isMonsterClicked(const Point& tarMapPos);
    // �ص�����
private:
    void CB_afterRoleMoveEnded(Node* pRender);       // ��ɫ�ƶ���Ļص�


private:
    CMap* pMap;
	CRole* pRole;
    std::vector<CNpc*> npcList;     // npc�б�,�洢����NPCָ��
    std::vector<CMonster*> monsterList;     // monster�б�

    // ui���
private:
    Widget* pTargetPanel;           // ��ʾĿ�������
};

