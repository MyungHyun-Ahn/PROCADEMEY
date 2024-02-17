#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define TICK_PER_SECONDS 1000
#define TICK_PER_FRAME 20
#define OVERHEAD 900
extern int a;
int main()
{
	int a;
	timeBeginPeriod(1);
	srand(time(NULL));
	int Logical_FPS = 0;
	int Rendering_FPS = 0;
	int dwOldFrameTick = timeGetTime();
	int dwSecondTick = dwOldFrameTick;
	while (1)
	{
		// --- ���� ---

		int dwTick = timeGetTime();

		if (dwTick - dwSecondTick >= TICK_PER_SECONDS)
		{
			dwSecondTick += TICK_PER_SECONDS;
			printf("Logic FPS : %d, Rendering FPS : %d\n", Logical_FPS, Rendering_FPS);
			Logical_FPS = 0;
			Rendering_FPS = 0;
		}

		// �������� �������� �̻� �и����
		if (dwTick - dwOldFrameTick >= TICK_PER_FRAME)
		{
			dwOldFrameTick += TICK_PER_FRAME;
			++Logical_FPS;
			continue;
		}
		dwTick = timeGetTime();
		Sleep(TICK_PER_FRAME - (dwTick - dwOldFrameTick));

		// ������ �ڵ�
		if (GetAsyncKeyState(VK_RSHIFT) & 0x8001)
			Sleep(OVERHEAD);

		++Rendering_FPS;
		++Logical_FPS;
		dwOldFrameTick += TICK_PER_FRAME;
	}

	return 0;
}