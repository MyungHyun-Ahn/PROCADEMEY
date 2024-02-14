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

// 초기화 경고 비활성화
#pragma warning(disable : 26495)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <assert.h>

//#define DEBUG_MALLOC(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__)
//
//#define malloc DEBUG_MALLOC

#ifdef malloc
#undef  malloc
#endif
#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))

#ifdef free
#undef free
#endif
#define free(p) (_free_dbg(p, _NORMAL_BLOCK))