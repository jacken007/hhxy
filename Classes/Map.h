#pragma once
#include "cocos2d.h"
#include "string.h"
#include "GlobalDefine.h"
#include <vector>

using namespace cocos2d;

// map中的NPC信息
struct MapNpcData {
    String id;
    Point pos;                  // 地图坐标

    // 下面是扩展属性
    String strModel;          // 动作对应的精灵ID
    String name;                // 名字
    SPRITE_DIRECTION dir;       // 朝向
    SPRITE_ACTION   sprAction;  // 动作
};

// 地图中的monster信息
struct MapMonsterData {
    String id;
    Point pos;

    // 扩展属性
    String strModel;
    String name;                // 名字
    SPRITE_DIRECTION dir;       // 朝向
    SPRITE_ACTION sprAction;    // 动作
};

class CMap : public Sprite {
public:
    CREATE_FUNC(CMap);
    virtual bool init();

public:
    Point getRoleBasicPos();    // 获取角色初始位置
    const TMXTiledMap* getTiledMap() { return pTiledMap;}   // 获取地图指针
    const Vec2 getMapSizeByPix();     // 获取地图的像素尺寸
    const std::vector<MapNpcData>& getNpcDataList() { return npcDataList;}
    const std::vector<MapMonsterData>& getMonsterDataList() {
        return monsterDataList;
   }
    bool isUnpassed(const Point& mapPos);   // 检测某个点是不是障碍

private:
    Vec2 convertMapPosToBlockPos(const Point& mapPos);   // 将某个地图坐标转换成地图块坐标
private:
    TMXTiledMap* pTiledMap;
    Point roleBasicPos;     // 地图中角色的位置
    std::vector<MapNpcData> npcDataList;    // npc信息列表
    std::vector<MapMonsterData> monsterDataList;    // 怪物信息列表
    CCTMXLayer* pUnpassLayer;   // 障碍层指针
};