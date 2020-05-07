#include "Role.h"
#include "string.h"
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

bool CRole::init()
{
	//////////////////////////////
	// 1. super init first
	if (!CMapElement::init())
	{
		return false;
	}

    this->setElementName("再来者");
    this->setStrModel("jianxiake");
   	return true;
}