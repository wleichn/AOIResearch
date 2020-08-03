#include "pch.h"
#include "Server.h"
#include <iostream>
#include "MyTime.h"
#include <ratio>
#include <chrono>

using std::cout;
using std::endl;

Server::Server()
{
}


Server::~Server()
{
	if (m_Map != NULL)
	{
		delete m_Map;
		m_Map = NULL;
	}
	Release();	
}

int Server::GenPlayerID()
{
	return m_PlayerNum + 1;
}

int Server::Init(int iPlayerNum)
{
	m_Map = new Map();
	m_Map->Init();
	for (int i = 0; i < iPlayerNum; ++i)
	{
		int newID = GenPlayerID();
		Player* tPlayer = CreatePlayer(newID);
		if (tPlayer == NULL)
		{
			return -1;
		}
		m_Players.push_back(tPlayer);
		++m_PlayerNum;
		// 把玩家添加进地图
		m_Map->AddPlayer(newID, tPlayer);
	}
	return 0;
}

void Server::Release()
{
	m_BeMonitored = NULL;
	for(auto onePlayer : m_Players)
	{
		if (onePlayer != NULL)
		{
			delete onePlayer;
			onePlayer = NULL;
		}
	}
	m_Players.clear();
}

Player* Server::CreatePlayer(int iID)
{
	Player* player = new Player(iID);
	return player;
}

int Server::Logic()
{
	m_Map->Logic();	
	return 0;
}

void Server::SetPlayerBeMonitored(int iID)
{
	Player* beMonitored = GetPlayer(iID);
	if (beMonitored != NULL)
	{
		m_BeMonitored = beMonitored;
	}
	else
	{
		m_BeMonitored = NULL;
	}
}

void Server::ConfirmPlayerViewList(ViewListReport& report)
{
	if (m_BeMonitored)
	{
		std::chrono::high_resolution_clock::time_point startViewFindTime = std::chrono::high_resolution_clock::now();
		list<int>& viewPlayers = m_BeMonitored->GetViewPlayers();
		std::chrono::high_resolution_clock::time_point endViewFindTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::ratio<1, 1000>> viewFindTime = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1000>>>(endViewFindTime - startViewFindTime);
		report.viewFindTime = viewFindTime.count();
		//验证对等性
		for (int playerID : viewPlayers)
		{
			Player* player = GetPlayer(playerID);
			if (!player) {
				//数据非法
				return;
			}
			if (player->IsInView(m_BeMonitored->GetID())) {
				report.viewList.push_back(playerID);
			}
			else {
				//非对等性
				return;
			}
		}
	}
}

Player* Server::GetPlayer(int iID)
{
	for (Player* onePlayer : m_Players)
	{
		if (onePlayer->GetID() == iID)
			return onePlayer;
	}
	return NULL;
}

void Server::OutPutPlayerBeMonitored()
{
	if (m_BeMonitored == NULL)
		return;
	cout << "<=============================>" << endl;
	cout << "玩家" << m_BeMonitored->GetID() << "坐标为：(" << m_BeMonitored->GetX() << "," <<
		m_BeMonitored->GetY() << ")，打印玩家" << m_BeMonitored->GetID() << "的视野信息：" << endl;
	auto viewList = m_BeMonitored->GetViewPlayers();
	if (!viewList.empty())
	{
		cout << "玩家视野内共" << viewList.size() << "人：";
		for (auto playerID : viewList)
		{
			cout << playerID << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "视野内共0人" << endl;
	}
	auto newViewList = m_BeMonitored->GetNewViewPlayers();
	if (!newViewList.empty())
	{
		cout << "玩家视野内新出现" << newViewList.size() << "人：";
		for (auto playerID : newViewList)
		{
			cout << playerID << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "视野内新出现0人" << endl;
	}
	auto lostViewList = m_BeMonitored->GetLostViewPlayers();
	if (!lostViewList.empty())
	{
		cout << "玩家视野内消失" << lostViewList.size() << "人：";
		for (auto playerID : lostViewList)
		{
			cout << playerID << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "视野内消失0人" << endl;
	}
	return;
}