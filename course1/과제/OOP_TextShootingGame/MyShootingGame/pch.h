#pragma once

#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include <tchar.h>
#include <wchar.h>
#include <string>
#include <utility>
#include <map>
#include <ctime>
#include <list>

#include "List.h"
#include "Define.h"

#include "BaseObject.h"

#include "Profiler.h"

#pragma comment(lib, "winmm.lib")

// 초기화 경고 비활성화
#pragma warning(disable : 26495)

// _s 문자열 함수 경고 끄기
#pragma warning(disable : 28020)
