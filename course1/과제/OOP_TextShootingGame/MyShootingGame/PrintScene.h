#pragma once
// �ܼ��� ��¸� �ϴ� ��
// LOBBY, LOAD, CLEAR, GAMEOVER
class PrintScene : public Scene
{
public:
	void Update() override;
	void Render() override;

	void Register(char *buffer) override;
};

