/************************************************************************/
/* 功能：地图元素类
/*  地图上显示的元素的基类,像怪物 NPC 主角都应该是这一基类的子类
/* 作者：jacken
/* 日期：2020/04/30                                                         */
/************************************************************************/
#pragma once

#include "cocos2d.h"
#include "GlobalDefine.h"

using namespace cocos2d;
class CMapElement : public Sprite {
public:
    //CREATE_FUNC(CMapElement);

public:
    virtual bool init();

    //get set 接口
public:
    void setElementName(const String& name);
    String getElementName() { return name;}
    void setAction(SPRITE_DIRECTION dir, SPRITE_ACTION action);
    void setStandAction();
    float getSpeed() { return moveSpeed; }
    String getStrModel() { return strModel;}
    //测试使用的接口
public:
    Label* getNameLable(){return pNameLable;}

public:
    void setStrModel(const String& strModel);
    bool isContainsPoint(const Point& tarPos);    // 检测精灵是否包含某个点。

public:
    // 根据朝向跟动作，获得动作的string串，这个子类必须实现
    // 返回动作的string串和动作帧数
    virtual String getActionStr(SPRITE_DIRECTION dir, SPRITE_ACTION action, int& actionNum);
private:
    String name;        // 名字
    String strModel;    // 模型名
    Label* pNameLable;  // 显示姓名的lable
    SPRITE_ACTION curAction;// 当前动作
    float moveSpeed;        //速度
    SPRITE_DIRECTION curDirection; // 当前朝向
};

