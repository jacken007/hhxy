#pragma once
#include "cocos2d.h"
#include "string.h"
#include "GlobalDefine.h"
#include <vector>

using namespace cocos2d;

// map�е�NPC��Ϣ
struct MapNpcData {
    String id;
    Point pos;                  // ��ͼ����

    // ��������չ����
    String strModel;          // ������Ӧ�ľ���ID
    String name;                // ����
    SPRITE_DIRECTION dir;       // ����
    SPRITE_ACTION   sprAction;  // ����
};

// ��ͼ�е�monster��Ϣ
struct MapMonsterData {
    String id;
    Point pos;

    // ��չ����
    String strModel;
    String name;                // ����
    SPRITE_DIRECTION dir;       // ����
    SPRITE_ACTION sprAction;    // ����
};

class CMap : public Sprite {
public:
    CREATE_FUNC(CMap);
    virtual bool init();

public:
    Point getRoleBasicPos();    // ��ȡ��ɫ��ʼλ��
    const TMXTiledMap* getTiledMap() { return pTiledMap;}   // ��ȡ��ͼָ��
    const Vec2 getMapSizeByPix();     // ��ȡ��ͼ�����سߴ�
    const std::vector<MapNpcData>& getNpcDataList() { return npcDataList;}
    const std::vector<MapMonsterData>& getMonsterDataList() {
        return monsterDataList;
   }
    bool isUnpassed(const Point& mapPos);   // ���ĳ�����ǲ����ϰ�

private:
    Vec2 convertMapPosToBlockPos(const Point& mapPos);   // ��ĳ����ͼ����ת���ɵ�ͼ������
private:
    TMXTiledMap* pTiledMap;
    Point roleBasicPos;     // ��ͼ�н�ɫ��λ��
    std::vector<MapNpcData> npcDataList;    // npc��Ϣ�б�
    std::vector<MapMonsterData> monsterDataList;    // ������Ϣ�б�
    CCTMXLayer* pUnpassLayer;   // �ϰ���ָ��
};