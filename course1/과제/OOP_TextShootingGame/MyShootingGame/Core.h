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
};

extern Core *g_Core;
