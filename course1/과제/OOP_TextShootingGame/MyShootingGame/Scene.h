#pragma once
enum class SceneType
{
	LOBBY,
	LOAD,
	GAME,
	CLEAR,
	GAMEOVER
};

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Register(char *buffer) = 0;

protected:
	char		m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // 화면 버퍼
};

