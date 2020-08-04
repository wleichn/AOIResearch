#pragma once

// 得到当前时间
unsigned int GetCurTime();

// 得到时间差
// 参数一：开始时间
// 参数二：结束时间
int GetTimeDiff(unsigned int start, unsigned end);

// 休眠这么长时间
void MySleep(unsigned int sleepTime);