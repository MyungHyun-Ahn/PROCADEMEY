#include "pch.h"
#include "GameSetting.h"
#include "BaseObject.h"

BaseObject::BaseObject(ObjectType objType, stPos pos)
	: m_ObjType(objType)
	, m_Pos(pos)
{
}
