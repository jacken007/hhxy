#include "Npc.h"
#include "string.h"

bool CNpc::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CMapElement::init())
	{
		return false;
	}

	return true;
}                 