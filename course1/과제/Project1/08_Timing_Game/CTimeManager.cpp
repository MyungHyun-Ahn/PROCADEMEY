#include "pch.h"

CTimeManager *CTimeManager::GetInstance()
{
    static CTimeManager timeManager;
    return &timeManager;
}

void CTimeManager::Init()
{
    m_ullStartTime = GetTickCount64();
    m_stTime = { 0, 0 };
}

void CTimeManager::Update()
{
    calCurTime();
}

void CTimeManager::calCurTime()
{
    ULONGLONG nowMs = GetTickCount64() - m_ullStartTime;
    m_stTime.m_ullSecond = nowMs / MS_PER_SEC;
    m_stTime.m_ullMillsecond = nowMs % MS_PER_SEC;
}
