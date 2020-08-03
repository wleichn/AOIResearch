// 一些基本数学公式
#pragma once
#include <math.h>

const float PI = (const float)3.1415926;
const float PI2 = (const float)6.2831852;

// 两个浮点数近似相等
bool FloatApproximate(float A, float B)
{
	if (A - B > -0.001f && A - B < 0.001f)
		return true;

	return false;
}

// 角度化弧度
float AngleToRadian(float Angle)
{
	float Radian = (float)Angle * PI / 180.0f;
	if (Radian > PI)
		Radian = Radian - PI2;
	return Radian;
}

// 弧度化角度
float RadianToAngle(float Radian)
{
	if (Radian < 0)
		Radian += PI2;
	return Radian * 180.0f / PI;
}

