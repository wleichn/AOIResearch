// AOIResearch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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

//主逻辑
//不可修改
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
	std::cout << "玩家总数为：" << nPlayerNum << "，要显示的玩家ID是：" << playerID << std::endl;
	std::cout << "开始执行测试用例" << std::endl;
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
			std::cout << "Big Tick：" << timeInterval / G_LOGICINTERVAL << std::endl;	
			nBigTickCount++;
		}
		while (timeInterval >= G_LOGICINTERVAL)
		{
			myServer->Logic();
			timeInterval -= G_LOGICINTERVAL;
			startTime += G_LOGICINTERVAL;
			nFrameCount++;
			std::cout << "第" << nFrameCount << "帧，总"<< endFrameID << "帧，此时" << (nFrameCount >= endFrameID) << std::endl;
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

	//开始进行正确性判断
	ViewListReport report;
	myServer->ConfirmPlayerViewList(report);

	std::cout << "总共跑的时间为：" << testTime << "ms" << std::endl;
	std::cout << "BIGTICK：" << nBigTickCount << std::endl;
	std::cout << "查找视野内玩家耗时" << report.viewFindTime << "ms" << std::endl;
	//将结果写到文件中	
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

//参数1玩家总人数
//参数2视野检查玩家ID
//参数3视野检查帧数
int main(int argc,char** argv)
{
	int args[4];
	std::cin >> args[1] >> args[2] >> args[3];
	InitGlobal();
	std::cout << "<==========cyou.com===========>" << std::endl;
	std::cout << "视野同步算法用例执行开始" << std::endl;
	GameMainLogic((args[1]), (args[2]), (args[3]));
    std::cout << "视野同步算法用例执行完毕..." << std::endl; 
	std::cout << "<==========cyou.com===========>" << std::endl;
	return 0;
}

