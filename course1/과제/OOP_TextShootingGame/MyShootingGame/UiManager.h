#pragma once
class UiManager
{
	SINGLE(UiManager)

public:
	void Render();

	void SendPlayerHpData(int curHp);
	void SendCurEnemyCountData(int curCount);
	void SendMaxEnemyCountData(int maxCount);

private:
	stPos m_PosHpUi;
	int m_iPlayerMaxHp;
	int m_iPlayerHp;

	stPos m_PosRemainEnemyUi;
	int m_iMaxEnemyCount;
	int m_iCurEnemyCount;
};

extern UiManager *g_UiMgr;

