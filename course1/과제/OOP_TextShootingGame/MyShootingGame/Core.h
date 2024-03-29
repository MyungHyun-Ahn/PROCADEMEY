#pragma once
class Core
{
	SINGLE(Core)
public:
	void Init();
	void Start();

	void Input();
	void Update();
	void Render();

private:
	void Exit();

private:
	bool m_bIsGameRunning = false;
};

extern Core *g_Core;
