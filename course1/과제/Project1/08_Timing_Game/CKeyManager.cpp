#include "pch.h"

CKeyManager *CKeyManager::GetInstance()
{
    static CKeyManager keyManager;
    return &keyManager;
}

void CKeyManager::Init()
{
    keyFlag = KEY_FLAGS::KEY_UP;
}

void CKeyManager::Update()
{
    if (_kbhit())
    {
        keyFlag = KEY_FLAGS::KEY_DOWN;
        _gettch();
    }
    else
    {
        keyFlag = KEY_FLAGS::KEY_UP;
    }
}
