#pragma once
class SceneManager
{
	SINGLE(SceneManager)

public:
	void Update();
	void Render();

	void RegisterStageInfo();

	// �� �Լ��� ȣ���Ͽ� ���� ��ȯ�Ѵ�.
	void LoadScene(SCENE_CODE code);
	char *GetCurrentSceneName() { return m_StageInfos[m_CurStageNum - 1].m_szFileName; }

	inline SCENE_CODE GetSceneCode() { return m_SceneCode; }

private:
	SCENE_CODE		m_SceneCode;
	Scene			*m_curScene = nullptr;
	int				m_StageCount;
	int				m_CurStageNum = 0;
	stStageInfo		*m_StageInfos = nullptr;
};

extern SceneManager *g_SceneMgr;