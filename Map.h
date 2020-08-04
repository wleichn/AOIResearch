#pragma once
// ��ͼ������

// ����ϵ��
//           �� y
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
	// �����Ҷ���
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
	//��ʼ���������
	//�������޸�
	void InitPosition(float& x, float& y);
	//�������޸� ��Ҫ��˳�������ҵ�����
	void OnAddPlayer(Player* player);
private:
	// ��Ұͬ���㷨��ʾ����
	void PlayerViewLogic();
	// ����X��Y��˳���б�
	list<Player* >::iterator AddToXList(Player* player);
	list<Player* >::iterator AddToYList(Player* player);
	list<Player* >::iterator UpdateToXList(Player* player);
	list<Player* >::iterator UpdateToYList(Player* player);
	// �ж�������ľ����Ƿ��ڿ��ӷ�Χ��
	bool JudgeDistanceInView(Player *onePlayer, Player *otherPlayer);
	// ����X��Y�����ȡĳ����ҵ���Ұ
	list<int> GetViewListByXYList(Player* player);
private:
	int m_Width = 0;			// ��ͼ��
	int m_Height = 0;			// ��ͼ��
	list<Player* > m_Players;//����ǰ��ͼ�е���ң���ȷ��TICK˳�򣬲������޸�
	//�������ṹ
	//.......
	list<Player* > x_Players;   // ����x��˳�򱣴����
	list<Player* > y_Players;   // ����y��˳�򱣴����
	int m_MapID = 0;			// ��ͼID
};

