// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 MJ_ROBOT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// MJ_ROBOT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef MJ_ROBOT_EXPORTS
#define MJ_ROBOT_API __declspec(dllexport)
#else
#define MJ_ROBOT_API __declspec(dllimport)
#endif

#include "mj_header.h"

MJ_ROBOT_API int SRResetGame(void);
// 更新牌局
MJ_ROBOT_API int SRSetGameCardData(const stCardData _inDesktopCard,
									const stCardData _inNotShowCard);

// 更新玩家牌
MJ_ROBOT_API int SRSetPlayerCardData(enDirection _inDec, stCardData _inCard);

// 设置主人公
MJ_ROBOT_API int SRSetGameProtagonists(enDirection _inDec);


// 询问出牌(自己摸牌时)
MJ_ROBOT_API int SRAskOutCard(unsigned char _inCard, unsigned char* _outCard);
										  
// 询问行为(当别人出牌时)
MJ_ROBOT_API int SRAskAction(unsigned char _inCard);

// 询问胡牌
MJ_ROBOT_API int SRAskWinCard(void);
