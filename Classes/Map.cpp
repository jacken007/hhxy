#include "Map.h"


bool CMap::init() {
    if (!Sprite::init()){
        return false;
    }

    // 加载tiledMap数据，设置断言，后面就不用再做nullptr的判断了
    pTiledMap = TMXTiledMap::create("./map/dhw.tmx");
    CCASSERT(pTiledMap != nullptr, "map load failed.");
    this->addChild(pTiledMap);

    // 解析tiledMap数据
    // 角色
    cocos2d::TMXObjectGroup* roleGroup = pTiledMap->getObjectGroup("role");
    auto role = roleGroup->getObject("role");
    roleBasicPos = Point(role["x"].asFloat(), role["y"].asFloat());
    // npc
    auto npcGroup = pTiledMap->getObjectGroup("npc");
    auto npcs = npcGroup->getObjects();
    for (auto& obj : npcs) {
        ValueMap& dict = obj.asValueMap();
        MapNpcData npc;
        npc.id  = dict["name"].asString();
        npc.pos = Point(dict["x"].asFloat(), dict["y"].asFloat());
        npc.name = dict["strName"].asString();
        npc.strModel = dict["actionName"].asString();
        npc.dir = SPRITE_DIRECTION(dict["direction"].asInt());
        npc.sprAction = SPRITE_ACTION(dict["action"].asInt());
        npcDataList.push_back(npc);
    }

    // monsters
    auto monsterGroup = pTiledMap->getObjectGroup("monster");
    auto monsters = monsterGroup->getObjects();
    for (auto& obj : monsters) {
        ValueMap& dict = obj.asValueMap();
        MapMonsterData monster;
        monster.id = dict["name"].asString();
        monster.pos = Point(dict["x"].asFloat(), dict["y"].asFloat());
        monster.name = dict["strName"].asString();
        monster.strModel = dict["strModel"].asString();
        monster.dir = SPRITE_DIRECTION(dict["direction"].asInt());
        monster.sprAction = SPRITE_ACTION(dict["action"].asInt());
        monsterDataList.push_back(monster);
    }

    // 障碍层
    pUnpassLayer = pTiledMap->getLayer("unpass");


    return true;
}

// 获取角色初始位置
Point CMap::getRoleBasicPos() {
    return roleBasicPos;
}

// 获取地图的像素尺寸
const Vec2 CMap::getMapSizeByPix() {
    // 这里，不能用设计的尺寸，因为map加入结点后，会根据适配模式，修改size的大小。
    // 用contentSize得到的就是实际的像素大小
    //Vec2 mapSize;
    //mapSize.x = pTiledMap->getMapSize().width * pTiledMap->getTileSize().width;
    //mapSize.y = pTiledMap->getMapSize().height * pTiledMap->getTileSize().height;
    //return mapSize;
    return pTiledMap->getContentSize();
}

/************************************************************************/
/* 功能：检测某个点是不是障碍
/* 参数：
/* 作者：jacken
/* 日期：2019/10/28                                                     */
/************************************************************************/
bool CMap::isUnpassed(const Point& mapPos) {
    return false;
    Vec2 blockPos = convertMapPosToBlockPos(mapPos);
    Sprite* pSpr = pUnpassLayer->getTileAt(blockPos);
    if (pSpr != NULL) {
        return true;
    }
    return false;
}


/************************************************************************/
/* 功能：将某个地图坐标转换成地图块坐标
/* 参数：
/* 作者：jacken
/* 日期：2019/10/28                                                     */
/************************************************************************/
cocos2d::Vec2 CMap::convertMapPosToBlockPos(const Point& mapPos) {
    Size tiledSize = pTiledMap->getTileSize();   // 获取砖块大小
    Vec2 blockPos = Vec2::ZERO;
    blockPos.x = mapPos.x/tiledSize.width;
    blockPos.y = mapPos.y/tiledSize.height;
    return blockPos;
    return Vec2::ZERO;
}
