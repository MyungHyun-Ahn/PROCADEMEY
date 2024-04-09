#pragma once
// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#define debug

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>

// C 런타임 헤더 파일입니다.
#include <cstdlib>
#include <ctime>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <queue>
#include <cmath>
#include <deque>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <iostream>

#include "Define.h"
#include "Setting.h"
#include "SelectGDI.h"
#include "DrawGRID.h"
#include "Node.h"
#include "AStar.h"
#include "JPS.h"
#include "Bresenham.h"
#include "CellularAutomata.h"

#pragma warning(disable : 26495)
#pragma warning(disable : 4267)