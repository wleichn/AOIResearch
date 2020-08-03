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
	player->x_ListIte = AddToXList(player);
	player->y_ListIte = AddToYList(player);

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
		onePlayer->x_ListIte = UpdateToXList(onePlayer);
		onePlayer->y_ListIte = UpdateToYList(onePlayer);
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
		list<int> viewList = GetViewListByXYList(onePlayer);

		for (int otherPlayerId : viewList)
		{
			auto itor = std::find(onePlayer->m_ViewList.begin(), onePlayer->m_ViewList.end(), otherPlayerId);
			if (itor == onePlayer->m_ViewList.end())
			{
				onePlayer->m_NewViewList.push_back(otherPlayerId);
				// onePlayer->m_ViewList.push_back(otherPlayerId);
			}
		}
		for(int otherPlayerId : onePlayer->m_ViewList)
		{
			auto itor = std::find(viewList.begin(), viewList.end(), otherPlayerId);
			if (itor != viewList.end())
			{
				onePlayer->m_LostViewList.push_back(otherPlayerId);
				// onePlayer->m_ViewList.erase(itor);
			}	
		}
		onePlayer->m_ViewList = viewList;
	}
}

list<Player* >::iterator Map:: AddToXList(Player* player){
	for(auto ite = x_Players.begin();  ite != x_Players.end(); ++ite){
		if((*ite)->m_X > player->m_X){
			return x_Players.insert(ite, player);
		}
	}
}

list<Player* >::iterator Map:: AddToYList(Player* player){
	for(auto ite = y_Players.begin();  ite != y_Players.end(); ++ite){
		if((*ite)->m_Y > player->m_Y){
			return y_Players.insert(ite, player);
		}
	}
}

list<Player* >::iterator Map:: UpdateToXList(Player* player){
	/**
	auto ite = player->x_ListIte;

	if(player->m_X == (*(player->x_ListIte))->m_X){ 		// x坐标未变化
		return ite;
	}else if(player->m_X > (*(player->x_ListIte))->m_X ){  // x坐标变大，
		++ite;
		x_Players.erase(player->x_ListIte);
		while(ite != x_Players.end()){
			if((*ite)->m_X > player->m_X){
				return x_Players.insert(ite, player);
			}
			++ite;
		}
	}else if(player->m_X < (*(player->x_ListIte))->m_X ){
		--ite;
		x_Players.erase(player->x_ListIte);
		while(ite != x_Players.begin()){
			if((*ite)->m_X < player->m_X){
				return x_Players.insert(++ite, player);
			}
			--ite;
		}
		return x_Players.insert(x_Players.begin(), player);
	}
	**/
	x_Players.erase(player->x_ListIte);
	for(auto ite = x_Players.begin(); ite != x_Players.end(); ++ite){
		if((*ite)->m_X > player->m_X){
			return x_Players.insert(ite, player);
		}
	}
}

list<Player* >::iterator Map:: UpdateToYList(Player* player){
	y_Players.erase(player->y_ListIte);
	for(auto ite = y_Players.begin(); ite != y_Players.end(); ++ite){
		if((*ite)->m_Y > player->m_Y){
			return y_Players.insert(ite, player);
		}
	}
}

bool Map:: JudgeDistanceInView(Player *onePlayer, Player *otherPlayer){
	return (onePlayer->m_X - otherPlayer->m_X) * (onePlayer->m_X - otherPlayer->m_X) +
				(onePlayer->m_Y - otherPlayer->m_Y) * (onePlayer->m_Y - otherPlayer->m_Y) <=
				G_PLAYERFOV * G_PLAYERFOV;
}

list<int> Map:: GetViewListByXYList(Player* player){
	auto ite_X = player->x_ListIte;
	auto ite_Y = player->y_ListIte;
	list<int> viewList;
	set<int> x_ViewSet;		// 保存x轴上在可视范围内的对象id

	// 看X坐标，获取距离当前玩家x坐标为视野半径以内的所有玩家
	++ite_X;
	// 当前玩家往右看
	while(ite_X != x_Players.end()){
		if((*ite_X)->m_X - player->m_X <= G_PLAYERFOV){
			x_ViewSet.insert((*ite_X)->GetID());
		}else{
			break;
		}
		++ite_X;
	}
	// 当前玩家往左看
	ite_X =  player->x_ListIte;
	--ite_X;
	while(ite_X != x_Players.begin()){
		if(player->m_X -(*ite_X)->m_X <= G_PLAYERFOV){
			x_ViewSet.insert((*ite_X)->GetID());
		}else{
			break;
		}
		--ite_X;
	}
	if(ite_X == x_Players.begin() && player->m_X -(*ite_X)->m_X <= G_PLAYERFOV){
		x_ViewSet.insert((*ite_X)->GetID());
	}

	// 看y坐标，获取所有y坐标距离当前玩家y坐标在视野内的玩家
	++ite_Y;
	// 当前玩家往上看
	while(ite_Y != y_Players.end()){
		if((*ite_Y)->m_Y - player->m_Y <= G_PLAYERFOV){
			// 且同时这些玩家在x轴的可见范围内
			if(x_ViewSet.find((*ite_Y)->m_ID) != x_ViewSet.end() && JudgeDistanceInView(player, *ite_Y)){
				/**
				auto itor = std::find(player->m_ViewList.begin(), player->m_ViewList.end(), (*ite_Y)->m_ID);
				if (itor == player->m_ViewList.end()){

					player->m_NewViewList.push_back((*ite_Y)->m_ID);
				}
				**/
				viewList.push_back((*ite_Y)->m_ID);
				player->m_ViewSet.insert((*ite_Y)->m_ID);
			}
		}else{
			break;
		}
		++ite_Y;
	}
	// 当前玩家往下看
	ite_Y = player->y_ListIte;
	--ite_Y;
	while(ite_Y != y_Players.begin()){
		if(player->m_Y - (*ite_Y)->m_Y <= G_PLAYERFOV){
			// 且同时这些玩家在x轴的可见范围内
			if(x_ViewSet.find((*ite_Y)->m_ID) != x_ViewSet.end() && JudgeDistanceInView(player, *ite_Y)){
				viewList.push_back((*ite_Y)->m_ID);
				player->m_ViewSet.insert((*ite_Y)->m_ID);
			}
		}else{
			break;
		}
		--ite_Y;
	}
	if(ite_Y == y_Players.begin() && 
		player->m_Y - (*ite_Y)->m_Y <= G_PLAYERFOV && 
		x_ViewSet.find((*ite_Y)->m_ID) != x_ViewSet.end() && 
		JudgeDistanceInView(player, *ite_Y)){

		viewList.push_back((*ite_Y)->m_ID);
		player->m_ViewSet.insert((*ite_Y)->m_ID);
			
	}
	return viewList;
}