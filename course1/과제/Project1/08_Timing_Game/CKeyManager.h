#pragma once

enum class KEY_FLAGS
{
	KEY_DOWN = 0,			// Ű�� ����
	KEY_UP,					// Ű�� ������ ����	
	END = 2
};

class CKeyManager
{
private:
	CKeyManager() = default;
	~CKeyManager() = default;

public:
	static CKeyManager		*GetInstance();
	void					Init();
	void					Update();
	inline KEY_FLAGS		GetFlag() { return keyFlag; }



private:
	KEY_FLAGS keyFlag;
};

