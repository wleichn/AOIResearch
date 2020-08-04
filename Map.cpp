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
	m_MapID = 1;		// demo��һ��Ĭ��ID����
	return;
}

void Map::Release()
{	
	m_Players.clear();
}

void Map::InitPosition(float& x, float& y)
{
	// ��ʼ��Ϊ�̶����ӵ��������
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
	//��������������ݲ��롣����
	player->x_ListIte = AddToXList(player);
	player->y_ListIte = AddToYList(player);

	player->SetMapID(m_MapID);
	return;
}

void Map::Logic()
{
	// �����������ƶ�
	for (auto onePlayer : m_Players)
	{
		// ����ڵ�ͼ�߽�������ƶ�
		onePlayer->RandMove(G_MOVESTEP, 1.0f, GetWidth(), 1.0f, GetHeight());
		onePlayer->x_ListIte = UpdateToXList(onePlayer);
		onePlayer->y_ListIte = UpdateToYList(onePlayer);
	}
	// ��Ұͬ��
	PlayerViewLogic();
	// ...
	return;
}

// �����ṩ����򵥵���Ұͬ���㷨������Ч�ʺܲ�
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
	return x_Players.insert(x_Players.end(), player);
}

list<Player* >::iterator Map:: AddToYList(Player* player){
	for(auto ite = y_Players.begin();  ite != y_Players.end(); ++ite){
		if((*ite)->m_Y > player->m_Y){
			return y_Players.insert(ite, player);
		}
	}
	return y_Players.insert(y_Players.end(), player);
}

list<Player* >::iterator Map:: UpdateToXList(Player* player){
	/**
	auto ite = player->x_ListIte;

	if(player->m_X == (*(player->x_ListIte))->m_X){ 		// x����δ�仯
		return ite;
	}else if(player->m_X > (*(player->x_ListIte))->m_X ){  // x������
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
	return x_Players.insert(x_Players.end(), player);
}

list<Player* >::iterator Map:: UpdateToYList(Player* player){
	y_Players.erase(player->y_ListIte);
	for(auto ite = y_Players.begin(); ite != y_Players.end(); ++ite){
		if((*ite)->m_Y > player->m_Y){
			return y_Players.insert(ite, player);
		}
	}
	return y_Players.insert(y_Players.end(), player);
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
	set<int> x_ViewSet;		// ����x�����ڿ��ӷ�Χ�ڵĶ���id

	// ��X���꣬��ȡ���뵱ǰ���x����Ϊ��Ұ�뾶���ڵ��������
	++ite_X;
	// ��ǰ������ҿ�
	while(ite_X != x_Players.end()){
		if((*ite_X)->m_X - player->m_X <= G_PLAYERFOV){
			x_ViewSet.insert((*ite_X)->GetID());
		}else{
			break;
		}
		++ite_X;
	}
	// ��ǰ�������
	ite_X =  player->x_ListIte;
	while(ite_X != x_Players.begin()){
		--ite_X;
		if(player->m_X -(*ite_X)->m_X <= G_PLAYERFOV){
			x_ViewSet.insert((*ite_X)->GetID());
		}else{
			break;
		}
	}
	if(ite_X == x_Players.begin() && player->m_X -(*ite_X)->m_X <= G_PLAYERFOV){
		x_ViewSet.insert((*ite_X)->GetID());
	}

	// ��y���꣬��ȡ����y������뵱ǰ���y��������Ұ�ڵ����
	++ite_Y;
	// ��ǰ������Ͽ�
	while(ite_Y != y_Players.end()){
		if((*ite_Y)->m_Y - player->m_Y <= G_PLAYERFOV){
			// ��ͬʱ��Щ�����x��Ŀɼ���Χ��
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
	// ��ǰ������¿�
	ite_Y = player->y_ListIte;

	while(ite_Y != y_Players.begin()){
		--ite_Y;
		if(player->m_Y - (*ite_Y)->m_Y <= G_PLAYERFOV){
			// ��ͬʱ��Щ�����x��Ŀɼ���Χ��
			if(x_ViewSet.find((*ite_Y)->m_ID) != x_ViewSet.end() && JudgeDistanceInView(player, *ite_Y)){
				viewList.push_back((*ite_Y)->m_ID);
				player->m_ViewSet.insert((*ite_Y)->m_ID);
			}
		}else{
			break;
		}
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