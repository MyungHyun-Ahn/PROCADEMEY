#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <deque>
#include <vector>
#include <map>
#include <locale>
#include <process.h>

#include "RingBuffer.h"