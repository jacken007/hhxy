#pragma once

#include "cocos2d.h"
#include "MapElement.h"
#include "string.h"
using namespace cocos2d;

class CMonster : public CMapElement {
public:
	CREATE_FUNC(CMonster);

public :
	virtual bool init();

};