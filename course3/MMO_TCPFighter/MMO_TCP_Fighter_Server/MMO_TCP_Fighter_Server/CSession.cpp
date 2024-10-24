#include "pch.h"
#include "CSession.h"

std::unordered_map<INT, CSession *> g_Sessions;
CObjectPool<CSession> g_SessionPool(7500, true);