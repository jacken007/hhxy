#pragma once

#include "cocos2d.h"
#include "MapElement.h"
#include "string.h"
using namespace cocos2d;

class CNpc : public CMapElement {
public:
	CREATE_FUNC(CNpc);

public :
	virtual bool init();

};