#pragma once
class CGameEngine
{
	SINGLE(CGameEngine)

public:
	void Start();

private:
	void Init();
	void GameLogic();
	void Update();
	void Render();

};

