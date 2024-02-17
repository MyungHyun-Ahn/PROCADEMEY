#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

#define dfFPS 50
#define dfFIXED_DELTATIME 1 / dfFPS
#define dfTIME_PERIOD 1

int g_Up = 0;
int g_Re = 0;

int prevTime1;
int prevTime2;
int curTime;

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
void fixedUpdate(void)
{
	curTime = timeGetTime();
	while (curTime - prevTime1 >= (1000 / dfFPS))
	{
		g_Up++;
		prevTime1 += 1000 / dfFPS;
	}
}

int main()
{
	timeBeginPeriod(dfTIME_PERIOD);

	prevTime1 = timeGetTime(); // 시작 시간
	prevTime2 = prevTime1; // 시작 시간
	while (true)
	{
		curTime = timeGetTime();
		if (curTime - prevTime2 >= 1000)
		{
			printf("Update : %d, Render %d\n", g_Up, g_Re);
			prevTime2 += 1000;
			g_Re = 0;
			g_Up = 0;
		}

		update();

		fixedUpdate();


		render();
		curTime = timeGetTime();
		if (curTime - prevTime1 < 1000 / dfFPS)
			Sleep(1000 / dfFPS - (curTime - prevTime1));
		prevTime1 += 1000 / dfFPS;
	}

	return 0;

	timeEndPeriod(dfTIME_PERIOD);
}