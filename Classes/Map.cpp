#include "Map.h"


bool CMap::init() {
    if (!Sprite::init()){
        return false;
    }

    // ����tiledMap���ݣ����ö��ԣ�����Ͳ�������nullptr���ж���
    pTiledMap = TMXTiledMap::create("./map/dhw.tmx");
    CCASSERT(pTiledMap != nullptr, "map load failed.");
    this->addChild(pTiledMap);

    // ����tiledMap����
    // ��ɫ
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

    // �ϰ���
    pUnpassLayer = pTiledMap->getLayer("unpass");


    return true;
}

// ��ȡ��ɫ��ʼλ��
Point CMap::getRoleBasicPos() {
    return roleBasicPos;
}

// ��ȡ��ͼ�����سߴ�
const Vec2 CMap::getMapSizeByPix() {
    // �����������Ƶĳߴ磬��Ϊmap������󣬻��������ģʽ���޸�size�Ĵ�С��
    // ��contentSize�õ��ľ���ʵ�ʵ����ش�С
    //Vec2 mapSize;
    //mapSize.x = pTiledMap->getMapSize().width * pTiledMap->getTileSize().width;
    //mapSize.y = pTiledMap->getMapSize().height * pTiledMap->getTileSize().height;
    //return mapSize;
    return pTiledMap->getContentSize();
}

/************************************************************************/
/* ���ܣ����ĳ�����ǲ����ϰ�
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/10/28                                                     */
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
/* ���ܣ���ĳ����ͼ����ת���ɵ�ͼ������
/* ������
/* ���ߣ�jacken
/* ���ڣ�2019/10/28                                                     */
/************************************************************************/
cocos2d::Vec2 CMap::convertMapPosToBlockPos(const Point& mapPos) {
    Size tiledSize = pTiledMap->getTileSize();   // ��ȡש���С
    Vec2 blockPos = Vec2::ZERO;
    blockPos.x = mapPos.x/tiledSize.width;
    blockPos.y = mapPos.y/tiledSize.height;
    return blockPos;
    return Vec2::ZERO;
}
