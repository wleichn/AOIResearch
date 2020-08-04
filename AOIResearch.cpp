// AOIResearch.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#include "pch.h"
#include <iostream>
#include "MyTime.h"
#include "Server.h"
#include "GameDefine.h"
#include <time.h>
#include <cstdlib>
#include <conio.h>
#include "MyBeep.h"

const unsigned int randomSeed0 = 0x77ffbb88;
const unsigned int randomSeed1 = 0x88bbff77;

void InitGlobal()
{
	srand(randomSeed0);
	return;
}

//���߼�
//�����޸�
void GameMainLogic(int nPlayerNum,int playerID,int endFrameID)
{
	if (playerID > nPlayerNum + 1) {
		return;
	}
	if (endFrameID <= 0) {
		return;
	}
	Server* myServer = new Server();
	if (myServer == NULL)
		return;	
	myServer->Init(nPlayerNum);
	myServer->SetPlayerBeMonitored(playerID);
	std::cout << "�������Ϊ��" << nPlayerNum << "��Ҫ��ʾ�����ID�ǣ�" << playerID << std::endl;
	std::cout << "��ʼִ�в�������" << std::endl;
	srand(randomSeed1);	
	unsigned int startTime = GetCurTime();
	unsigned int testTime = startTime;
	bool bRun = true;
	int nFrameCount = 0;
	int nBigTickCount = 0;
	while (bRun)
	{
		unsigned int nowTime = GetCurTime();
		int timeInterval = GetTimeDiff(startTime, nowTime);
		if (timeInterval / G_LOGICINTERVAL > 1)
		{
			std::cout << "Big Tick��" << timeInterval / G_LOGICINTERVAL << std::endl;	
			nBigTickCount++;
		}
		while (timeInterval >= G_LOGICINTERVAL)
		{
			myServer->Logic();
			timeInterval -= G_LOGICINTERVAL;
			startTime += G_LOGICINTERVAL;
			nFrameCount++;
			std::cout << "��" << nFrameCount << "֡����"<< endFrameID << "֡����ʱ" << (nFrameCount >= endFrameID) << std::endl;
			if (nFrameCount >= endFrameID) {
				bRun = false;
				unsigned int lastTime = GetCurTime();
				testTime = GetTimeDiff(testTime, lastTime);				
			}
		}
		nowTime = GetCurTime();
		timeInterval = GetTimeDiff(nowTime, startTime + G_LOGICINTERVAL);
		if (timeInterval >= 0)
		{
			MySleep(timeInterval);
		}
	}

	//��ʼ������ȷ���ж�
	ViewListReport report;
	myServer->ConfirmPlayerViewList(report);

	std::cout << "�ܹ��ܵ�ʱ��Ϊ��" << testTime << "ms" << std::endl;
	std::cout << "BIGTICK��" << nBigTickCount << std::endl;
	std::cout << "������Ұ����Һ�ʱ" << report.viewFindTime << "ms" << std::endl;
	//�����д���ļ���	
	FILE* fpRpt = NULL;
	fopen_s(&fpRpt,"./AOIResearch.report", "wb");
	if (fpRpt != NULL) {
		fwrite(&testTime, sizeof(testTime), 1, fpRpt);
		fwrite(&nBigTickCount, sizeof(nBigTickCount), 1, fpRpt);
		fwrite(&report.viewFindTime, sizeof(report.viewFindTime), 1, fpRpt);
		int viewListCount = (int)report.viewList.size();
		fwrite(&viewListCount, sizeof(viewListCount), 1, fpRpt);
		for (int playerID : report.viewList) {
			fwrite(&playerID, sizeof(playerID), 1, fpRpt);
		}
		fclose(fpRpt);
	}	
	if (myServer != NULL){
		delete myServer;
		myServer = NULL;
	}
	return;
}

//����1���������
//����2��Ұ������ID
//����3��Ұ���֡��
int main(int argc,char** argv)
{
	int args[4];
	std::cin >> args[1] >> args[2] >> args[3];
	InitGlobal();
	std::cout << "<==========cyou.com===========>" << std::endl;
	std::cout << "��Ұͬ���㷨����ִ�п�ʼ" << std::endl;
	GameMainLogic((args[1]), (args[2]), (args[3]));
    std::cout << "��Ұͬ���㷨����ִ�����..." << std::endl; 
	std::cout << "<==========cyou.com===========>" << std::endl;
	return 0;
}

