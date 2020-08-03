#pragma once
#include <list>
using std::list;

class Map;
// 玩家类
class Player
{
	friend class Map;
public:
	Player();
	Player(int id);
	~Player();
	// 
	void Init();
	// 随机移动
	// 参数一：移动步长
	// 参数二、参数三：x方向最小和最大移动范围
	// 参数四、参数五：y方向最小和最大移动范围
	// 不可修改
	void RandMove(float fMoveStep, float xMin, float xMax, float yMin, float yMax);
	// 得到视野内玩家ID
	list<int>& GetViewPlayers();
	// 得到视野内新出现的玩家ID	
	list<int>& GetNewViewPlayers();
	// 得到视野内消失的玩家ID
	list<int>& GetLostViewPlayers();
	// 某个玩家是否在自己视野内
	bool IsInView(int otherPlayerID);
	// 得到自己的ID
	int GetID()
	{
		return m_ID;
	}
	// 设置所在地图ID
	void SetMapID(int mapid)
	{
		m_MapID = mapid;
	}
	// 得到所在地图ID
	int GetMapID()
	{
		return m_MapID;
	}
	bool operator ==(Player* other);
	bool operator !=(Player* other);
	float GetX()
	{
		return m_X;
	}
	float GetY()
	{
		return m_Y;
	}
private:
	float m_X = 0, m_Y = 0;			// 当前坐标x, y
	int   m_ID = 0;					// 玩家ID
	float m_LastX = 0, m_LastY = 0; // 本次移动前的坐标
	float m_lastRadian = 0;			// 上次移动的弧度
	list<int> m_ViewList;			// 视野列表
	list<int> m_NewViewList;		// 本帧视野中新增对象的列表
	list<int> m_LostViewList;		// 本帧视野中消失对象的列表
	int	  m_MapID = 0;				// 玩家所在地图ID
};

