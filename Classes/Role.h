#pragma once

#include "cocos2d.h"
#include "MapElement.h"
using namespace cocos2d;

class CRole : public CMapElement {
public:
	CREATE_FUNC(CRole);

public :
	virtual bool init();

};