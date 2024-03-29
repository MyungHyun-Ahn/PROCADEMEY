#include "pch.h"
#include "GameSetting.h"
#include "UiManager.h"
#include "ConsoleManager.h"

UiManager *g_UiMgr;

UiManager::UiManager() 
	: m_iPlayerMaxHp(dfPLAYER_HP)
	, m_PosHpUi({0, 0})
	, m_PosRemainEnemyUi({1, 0})
{

}

UiManager::~UiManager()
{

}

void UiManager::Render()
{
	char HpBuffer[30];
	sprintf_s(HpBuffer, "HP : %03d / %03d", m_iPlayerHp, m_iPlayerMaxHp);
	g_ConsoleMgr->WriteText(m_PosHpUi, HpBuffer);

	char RemainEnemyBuffer[30];
	sprintf_s(RemainEnemyBuffer, "Remain Enemy : %03d / %03d", m_iCurEnemyCount, m_iMaxEnemyCount);
	g_ConsoleMgr->WriteText(m_PosRemainEnemyUi, RemainEnemyBuffer);
}

void UiManager::SendPlayerHpData(int curHp)
{
	m_iPlayerHp = curHp;
}

void UiManager::SendCurEnemyCountData(int curCount)
{
	m_iCurEnemyCount = curCount;
}

void UiManager::SendMaxEnemyCountData(int maxCount)
{
	m_iMaxEnemyCount = maxCount;
}

