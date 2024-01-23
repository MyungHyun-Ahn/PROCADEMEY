#pragma once

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include <strsafe.h>

#include <vector>
#include <map>
#include <queue>


constexpr size_t MAX_STRING_SIZE = 500;
constexpr size_t MAX_DICT_SIZE = 5000;
constexpr WORD TO_UPPER_MASK = 0b00100000;

// MMF 모드 설정
#define MODE_MMF