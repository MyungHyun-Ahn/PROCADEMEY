#include "pch.h"
#include "Session.h"

std::unordered_map<INT, Session *> g_Sessions;
ObjectPool<Session> g_SessionPool(7500, true);