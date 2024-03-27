#pragma once

enum class ObjectType
{
	// Player 包访 0 ~ 99
	Player = 0,

	// Enemy 包访 100 ~ 199
	Enemy = 100,

	// Missile 包访 200 ~ 299
	Missile = 200,

	// UI 包访 300 ~ 399
	UI = 300,

	// etc 400 ~ 
	ETC = 400
};

class BaseObject
{
public:
	BaseObject(ObjectType objType, stPos pos);
	virtual ~BaseObject() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	inline ObjectType GetObjectType() { return m_ObjType; }
	inline stPos &GetPos() { return m_Pos; }
	inline bool IsActive() { return m_bIsActive; }

protected:
	bool			m_bIsActive = true;
	stPos			m_Pos;
	ObjectType		m_ObjType;
};

