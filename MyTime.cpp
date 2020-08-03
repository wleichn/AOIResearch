#include "pch.h"
#include "MyTime.h"
#include <time.h>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "Winmm.lib")

unsigned int GetCurTime()
{
	timeBeginPeriod(1);
	unsigned int time = timeGetTime();
	timeEndPeriod(1);
	return time;
}

int GetTimeDiff(unsigned int start, unsigned end)
{
	if (start > 0xfffa0000 && end <= 60000) // 处理时间回绕，设置2分钟阈值
		return (int)((long long)end + 0x100000000 - (long long)start);
	return (int)((long long)end - (long long)start);
}

void MySleep(unsigned int sleepTime)
{
	timeBeginPeriod(1);
	Sleep((DWORD)sleepTime);
	timeEndPeriod(1);
}