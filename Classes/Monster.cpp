#include "Monster.h"
#include "string.h"

bool CMonster::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CMapElement::init())
	{
		return false;
	}

	return true;
}                 