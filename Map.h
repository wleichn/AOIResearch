#pragma once
// 地图管理器

// 坐标系：
//           ∧ y
//            |
//            |
//            |
// ------------------------>
//            |            x
//            |
//            |
//            |

#include "Player.h"
#include <list>
#include <set>

using std::list;
using std::set;

class Map
{
public:
	Map();
	~Map();
	void Init();
	void Logic();
	// 添加玩家对象
	void AddPlayer(int id, Player* player);	
	float GetWidth() const
	{
		return (float)m_Width;
	}
	float GetHeight() const
	{
		return (float)m_Height;
	}
	void Release();
private:
	//初始化玩家坐标
	//不可以修改
	void InitPosition(float& x, float& y);
	//不可以修改 需要按顺序更新玩家的坐标
	void OnAddPlayer(Player* player);
private:
	// 视野同步算法（示例）
	void PlayerViewLogic();
	// 更新X、Y轴顺序列表
	list<Player* >::iterator AddToXList(Player* player);
	list<Player* >::iterator AddToYList(Player* player);
	list<Player* >::iterator UpdateToXList(Player* player);
	list<Player* >::iterator UpdateToYList(Player* player);
	// 判断两个点的距离是否在可视范围内
	bool JudgeDistanceInView(Player *onePlayer, Player *otherPlayer);
	// 根据X、Y链表获取某个玩家的视野
	list<int> GetViewListByXYList(Player* player);
private:
	int m_Width = 0;			// 地图宽
	int m_Height = 0;			// 地图高
	list<Player* > m_Players;//管理当前地图中的玩家，并确定TICK顺序，不可以修改
	//其余管理结构
	//.......
	list<Player* > x_Players;   // 按照x轴顺序保存玩家
	list<Player* > y_Players;   // 按照y轴顺序保存玩家
	int m_MapID = 0;			// 地图ID
};

