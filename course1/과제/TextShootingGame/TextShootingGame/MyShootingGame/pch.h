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

#include "Define.h"
#include "GameSetting.h"
#include "GlobalVariable.h"

#include "CProfiler.h"

#pragma comment(lib, "winmm.lib")

// �ʱ�ȭ ��� ��Ȱ��ȭ
#pragma warning(disable : 26495)

// _s ���ڿ� �Լ� ��� ����
#pragma warning(disable : 28020)