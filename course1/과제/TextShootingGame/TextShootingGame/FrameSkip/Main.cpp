#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

#define dfFPS 50
#define dfFIXED_DELTATIME 1 / dfFPS
#define dfTIME_PERIOD 1

int g_Up = 0;
int g_Re = 0;

DWORD prevTime1;
DWORD prevTime2;
DWORD curTime;

void update(void)
{
	g_Up++;
	Sleep(0);
}

void render(void)
{
	int size = 0;

	g_Re++;
	if (GetAsyncKeyState(VK_SPACE))
	{
		size += 2500;
	}

	Sleep(size);
}

// true 면 skip 된 것
bool Skip(void)
{
	curTime = timeGetTime();
	if (curTime - prevTime1 >= (1000 / dfFPS)) // 1프레임만큼 초과한 상황
	{
		// printf("Frame Skip!\n");
		prevTime1 += 1000 / dfFPS;

		return true;
	}

	return false;
}

int main()
{
	timeBeginPeriod(dfTIME_PERIOD);

	prevTime1 = timeGetTime(); // 시작 시간
	prevTime2 = timeGetTime(); // 시작 시간
	while (true)
	{
		// Update
		update();

		if (!Skip())
		{
			render();
			curTime = timeGetTime();
			if (curTime - prevTime1 < 1000 / dfFPS)
				Sleep(1000 / dfFPS - (curTime - prevTime1));
			prevTime1 += 1000 / dfFPS;
		}

		curTime = timeGetTime();
		if (curTime - prevTime2 >= 1000)
		{
			printf("Update : %d, Render %d\n", g_Up, g_Re);
			prevTime2 += 1000;
			g_Re = 0;
			g_Up = 0;
		}
	}

	return 0;

	timeEndPeriod(dfTIME_PERIOD);
}