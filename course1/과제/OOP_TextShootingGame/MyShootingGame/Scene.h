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

	inline SceneType GetSceneType() { return m_sceneType; }

protected:
	SceneType	m_sceneType;
	char		m_szConsoleBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH]; // 화면 버퍼
};

