#pragma once
#include "Map.h"
#include "Player.h"
#include <list>
using std::list;

//导出检测工具需要的验证数据
//不可修改内部结构顺序
struct ViewListReport
{
	double viewFindTime = 0;//查找函数的总耗时
	list<int> viewList;
};

// 服务器总控类
class Server
{
public:
	Server();
	~Server();
	int Init(int iPlayerNum = 100);
	void Release();
	int Logic();
	// 设置要监视的玩家
	// 不可修改
	void SetPlayerBeMonitored(int iID);	
	//获得被监视玩家的视野列表
	//不可进行函数本意的修改
	void ConfirmPlayerViewList(ViewListReport& report);
private:
	// 创建玩家对象
	Player* CreatePlayer(int iID);
	// 生成玩家ID
	int GenPlayerID();
	// 打印被监控的玩家视野信息
	// 不可修改
	void OutPutPlayerBeMonitored();
	// 获得玩家指针
	Player* GetPlayer(int iID);
private:
	Map* m_Map = NULL;				// 只有一张地图
	int m_PlayerNum = 0;			// 玩家总数
	list<Player* > m_Players;		// 玩家链表
	Player* m_BeMonitored = NULL;	// 被监视的玩家，不可修改
};

