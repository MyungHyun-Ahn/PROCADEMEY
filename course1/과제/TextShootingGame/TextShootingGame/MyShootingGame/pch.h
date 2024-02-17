#pragma once

#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include <wchar.h>
#include <utility>
#include <map>

#include "Define.h"
#include "GameSetting.h"
#include "GlobalVariable.h"

#pragma comment(lib, "winmm.lib")

// 초기화 경고 비활성화
#pragma warning(disable : 26495)

// _s 문자열 함수 경고 끄기
#pragma warning(disable : 28020)