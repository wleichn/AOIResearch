#include "pch.h"
#include "Map.h"
#include "GameDefine.h"

Map::Map()
{
}


Map::~Map()
{
	Release();
}

void Map::Init()
{
	m_Width = G_MAPWIDTH;
	m_Height = G_MAPHEIGHT;
	m_MapID = 1;		// demo用一个默认ID即可
	return;
}

void Map::Release()
{	
	m_Players.clear();
}

void Map::InitPosition(float& x, float& y)
{
	// 初始化为固定种子的随机坐标
	int posX = rand() % (m_Width + 1);
	if (posX == 0)
		posX = 1;
	x = (float)posX;
	int posY = rand() % (m_Height + 1);
	if (posY == 0)
		posY = 1;
	y = (float)posY;
}

void Map::OnAddPlayer(Player* player)
{
	m_Players.push_back(player);
}

void Map::AddPlayer(int id, Player* player)
{
	if (player == NULL)
		return;
	InitPosition(player->m_X, player->m_Y);		
	OnAddPlayer(player);
	//其余管理器的数据插入。。。
	player->SetMapID(m_MapID);
	return;
}

void Map::Logic()
{
	// 所有玩家随机移动
	for (auto onePlayer : m_Players)
	{
		// 玩家在地图边界内随机移动
		onePlayer->RandMove(G_MOVESTEP, 1.0f, GetWidth(), 1.0f, GetHeight());
	}
	// 视野同步
	PlayerViewLogic();
	// ...
	return;
}

// 这里提供了最简单的视野同步算法，但是效率很差
void Map::PlayerViewLogic()
{
	for (Player* onePlayer : m_Players)
	{
		onePlayer->m_NewViewList.clear();
		onePlayer->m_LostViewList.clear();
		for (Player* otherPlayer : m_Players)
		{
			if (onePlayer->GetID() == otherPlayer->GetID())	// 不能把自己放入自己的视野范围
				continue;
			if ((onePlayer->m_X - otherPlayer->m_X) * (onePlayer->m_X - otherPlayer->m_X) +
				(onePlayer->m_Y - otherPlayer->m_Y) * (onePlayer->m_Y - otherPlayer->m_Y) <=
				G_PLAYERFOV * G_PLAYERFOV)
			{
				auto itor = std::find(onePlayer->m_ViewList.begin(), onePlayer->m_ViewList.end(), otherPlayer->GetID());
				if (itor == onePlayer->m_ViewList.end())
				{
					onePlayer->m_NewViewList.push_back(otherPlayer->GetID());
					onePlayer->m_ViewList.push_back(otherPlayer->GetID());
				}
			}
			else
			{
				auto itor = std::find(onePlayer->m_ViewList.begin(), onePlayer->m_ViewList.end(), otherPlayer->GetID());
				if (itor != onePlayer->m_ViewList.end())
				{
					onePlayer->m_LostViewList.push_back(otherPlayer->GetID());
					onePlayer->m_ViewList.erase(itor);
				}
			}
		}
	}
}