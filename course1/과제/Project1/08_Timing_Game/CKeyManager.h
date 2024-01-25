#pragma once

enum class KEY_FLAGS
{
	KEY_DOWN = 0,			// 키가 눌림
	KEY_UP,					// 키가 눌리지 않음	
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

