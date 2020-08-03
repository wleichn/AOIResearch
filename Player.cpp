#include "pch.h"
#include "Player.h"
#include "MyMath.h"
#include <cstdlib>

Player::Player()
{
	m_ViewList.clear();
	m_ViewSet.clear();
}

Player::Player(int id)
{
	Player();
	m_ID = id;
}

Player::~Player()
{
	m_ViewList.clear();
	m_NewViewList.clear();
	m_LostViewList.clear();
	m_ViewSet.clear();
}

void Player::Init()
{
	return;
}

void Player::RandMove(float fMoveStep, float xMin, float xMax, float yMin, float yMax)
{
	m_LastX = m_X;
	m_LastY = m_Y;
	int iDir = rand() % 360;
	float radian = AngleToRadian((float)iDir);
	radian = (m_lastRadian * 7 + radian) / 8;	// 保证移动方向相对平缓
	float xStep = cos(radian) * fMoveStep;
	float yStep = sin(radian) * fMoveStep;
	m_X = m_LastX + xStep;
	m_Y = m_LastY + yStep;
	if (m_X < xMin || m_X > xMax)
	{
		m_X = m_LastX - xStep;
		if (radian < 0)
			radian = -PI - radian;
		else
			radian = PI - radian;
	}
	if (m_Y < yMin || m_Y > yMax)
	{
		m_Y = m_LastY - yStep;
		radian = -radian;
	}
	m_lastRadian = radian;
	return;
}

list<int>& Player::GetViewPlayers()
{
	return m_ViewList;
}

list<int>& Player::GetNewViewPlayers()
{
	return m_NewViewList;
}

list<int>& Player::GetLostViewPlayers()
{
	return m_LostViewList;
}

//这个函数的效率很低 需要修改
bool Player::IsInView(int otherPlayerID)
{
	return m_ViewSet.find(otherPlayerID) != m_ViewSet.end();
}

bool Player::operator ==(Player* other)
{
	return m_ID == other->m_ID;
}

bool Player::operator !=(Player* other)
{
	return m_ID != other->m_ID;
}
