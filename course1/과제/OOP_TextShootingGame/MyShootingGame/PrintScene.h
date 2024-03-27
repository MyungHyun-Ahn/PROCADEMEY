#pragma once
// 단순히 출력만 하는 씬
// LOBBY, LOAD, CLEAR, GAMEOVER
class PrintScene : public Scene
{
public:
	void Update() override;
	void Render() override;

	void Register(char *buffer) override;
};

