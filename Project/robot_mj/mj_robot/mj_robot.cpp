// mj_robot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "mj_robot.h"
#include "SRAnalyzeCard.h"

static SRAnalyzeCard gAnalyzeCard;

// 更新牌局
MJ_ROBOT_API int SRSetGameCardData(const stCardData _inDesktopCard,
	const stCardData _inNotShowCard) {
	gAnalyzeCard.setGameCardForShow(_inDesktopCard);
	gAnalyzeCard.setGameCardForHide(_inNotShowCard);
	return 0;
}

// 更新玩家牌
MJ_ROBOT_API int SRSetPlayerCardData(enDirection _dec, stCardData _data) {
	gAnalyzeCard.setGameCardForPlayer(_dec, _data);
	return 0;
}

MJ_ROBOT_API int SRSetGameProtagonists(enDirection _inDec) {
	gAnalyzeCard.setGameProtagonists(_inDec);
	return 0;
}


// 询问出牌
MJ_ROBOT_API int SRAskOutCard(unsigned char* _outCard) {
	// 查看是否有牌可跟

	// 分析三家牌型

	// 分析自己的劣牌
	return 0;
}

// 询问碰牌
MJ_ROBOT_API int SRAskAction(unsigned char _inCard) {
	return 0;
}

// 询问胡牌
MJ_ROBOT_API int SRAskWinCard(void) {
	return 0;
}

