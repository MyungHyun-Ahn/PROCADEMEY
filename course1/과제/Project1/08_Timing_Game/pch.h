#pragma once

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>
#include <stdlib.h>

#include "Singleton.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CGameLogic.h"
#include "CGameEngine.h"

constexpr ULONGLONG MS_PER_SEC = 1000;

static int g_Timing[] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };
static TCHAR g_ResStr[][40] = { TEXT("Great"), TEXT("Good"), TEXT("Nogood"), TEXT("Bad"), TEXT("FAIL"), TEXT("") };