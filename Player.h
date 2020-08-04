#pragma once
#include <list>
#include <set>
using std::list;
using std::set;

class Map;
// �����
class Player
{
	friend class Map;
public:
	Player();
	Player(int id);
	~Player();
	// 
	void Init();
	// ����ƶ�
	// ����һ���ƶ�����
	// ����������������x������С������ƶ���Χ
	// �����ġ������壺y������С������ƶ���Χ
	// �����޸�
	void RandMove(float fMoveStep, float xMin, float xMax, float yMin, float yMax);
	// �õ���Ұ�����ID
	list<int>& GetViewPlayers();
	// �õ���Ұ���³��ֵ����ID	
	list<int>& GetNewViewPlayers();
	// �õ���Ұ����ʧ�����ID
	list<int>& GetLostViewPlayers();
	// ĳ������Ƿ����Լ���Ұ��
	bool IsInView(int otherPlayerID);
	// �õ��Լ���ID
	int GetID()
	{
		return m_ID;
	}
	// �������ڵ�ͼID
	void SetMapID(int mapid)
	{
		m_MapID = mapid;
	}
	// �õ����ڵ�ͼID
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
	float m_X = 0, m_Y = 0;			// ��ǰ����x, y
	int   m_ID = 0;					// ���ID
	float m_LastX = 0, m_LastY = 0; // �����ƶ�ǰ������
	float m_lastRadian = 0;			// �ϴ��ƶ��Ļ���
	list<int> m_ViewList;			// ��Ұ�б�
	list<int> m_NewViewList;		// ��֡��Ұ������������б�
	list<int> m_LostViewList;		// ��֡��Ұ����ʧ������б�
	int	  m_MapID = 0;				// ������ڵ�ͼID
	list<Player* >::iterator x_ListIte;		// ��Ұ���X������ʱ��λ��
	list<Player* >::iterator y_ListIte;		// ��Ұ���Y������ʱ��λ��
	set<int> m_ViewSet;				// ��Ұ���ϣ���m_ViewListһͬά�������������ж϶����Ƿ��ڵ�ǰ�����Ұ�ڵ�Ч�ʣ�
};

