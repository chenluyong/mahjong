// mj_robot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "mj_robot.h"


// 更新牌局
MJ_ROBOT_API int SRSetGameCardData(const stCardData _inDesktopCard,
	const stCardData _inNotShowCard) {
	return 0;
}

// 更新玩家牌
MJ_ROBOT_API int SRSetPlayerCardData(const stCardData _inPlayer_Left,
	const stCardData _inPlayer_Above,
	const stCardData _inPlayer_Right,
	const stCardData _inPlayer_Myself) {
	return 0;
}


// 询问出牌
MJ_ROBOT_API int SRAskOutCard(unsigned char* _outCard) {
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

