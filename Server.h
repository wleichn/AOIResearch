#pragma once
#include "Map.h"
#include "Player.h"
#include <list>
using std::list;

//������⹤����Ҫ����֤����
//�����޸��ڲ��ṹ˳��
struct ViewListReport
{
	double viewFindTime = 0;//���Һ������ܺ�ʱ
	list<int> viewList;
};

// �������ܿ���
class Server
{
public:
	Server();
	~Server();
	int Init(int iPlayerNum = 100);
	void Release();
	int Logic();
	// ����Ҫ���ӵ����
	// �����޸�
	void SetPlayerBeMonitored(int iID);	
	//��ñ�������ҵ���Ұ�б�
	//���ɽ��к���������޸�
	void ConfirmPlayerViewList(ViewListReport& report);
private:
	// ������Ҷ���
	Player* CreatePlayer(int iID);
	// �������ID
	int GenPlayerID();
	// ��ӡ����ص������Ұ��Ϣ
	// �����޸�
	void OutPutPlayerBeMonitored();
	// ������ָ��
	Player* GetPlayer(int iID);
private:
	Map* m_Map = NULL;				// ֻ��һ�ŵ�ͼ
	int m_PlayerNum = 0;			// �������
	list<Player* > m_Players;		// �������
	Player* m_BeMonitored = NULL;	// �����ӵ���ң������޸�
};

