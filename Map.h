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
using std::list;

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
private:
	int m_Width = 0;			// 地图宽
	int m_Height = 0;			// 地图高
	list<Player* > m_Players;//管理当前地图中的玩家，并确定TICK顺序，不可以修改
	//其余管理结构
	//.......
	int m_MapID = 0;			// 地图ID
};

