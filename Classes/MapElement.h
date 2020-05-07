/************************************************************************/
/* ���ܣ���ͼԪ����
/*  ��ͼ����ʾ��Ԫ�صĻ���,����� NPC ���Ƕ�Ӧ������һ���������
/* ���ߣ�jacken
/* ���ڣ�2020/04/30                                                         */
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

    //get set �ӿ�
public:
    void setElementName(const String& name);
    String getElementName() { return name;}
    void setAction(SPRITE_DIRECTION dir, SPRITE_ACTION action);
    void setStandAction();
    float getSpeed() { return moveSpeed; }
    String getStrModel() { return strModel;}
    //����ʹ�õĽӿ�
public:
    Label* getNameLable(){return pNameLable;}

public:
    void setStrModel(const String& strModel);
    bool isContainsPoint(const Point& tarPos);    // ��⾫���Ƿ����ĳ���㡣

public:
    // ���ݳ������������ö�����string��������������ʵ��
    // ���ض�����string���Ͷ���֡��
    virtual String getActionStr(SPRITE_DIRECTION dir, SPRITE_ACTION action, int& actionNum);
private:
    String name;        // ����
    String strModel;    // ģ����
    Label* pNameLable;  // ��ʾ������lable
    SPRITE_ACTION curAction;// ��ǰ����
    float moveSpeed;        //�ٶ�
    SPRITE_DIRECTION curDirection; // ��ǰ����
};

