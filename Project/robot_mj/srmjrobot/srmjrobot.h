// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SRMJROBOT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SRMJROBOT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef _SHANREN_ROBOT_H_
#define _SHANREN_ROBOT_H_
#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif

#include <iostream>

class SRMahjong;
class SRMahjongPool;

class SRMJROBOT_API SRRobot {

public:

	enum enDirection {
		None = -1,
		South = 0,              //���� ��
		West,					//���� ��
		North,                  //���� ��
		East,					//���� ��
	};

	SRRobot(void);
	virtual ~SRRobot(void);

public:

	// ����
	// direction : ���Ƶķ���ǰ����:0��������:1��  Ĭ��Ϊ0
	int touchCard(int direction = 0);
	
	// ����
	// out_card : ���Ƶ�����
	// out_card_count : ���Ƶ�����
	int getOutCard(unsigned char* out_card, unsigned char* out_card_count);

	// ѯ�ʶ���
	// card : �����������������
	// ����ֵΪ���嶯�������ͷ�ļ��ķ���ֵ����
	int getAction(enDirection drc, unsigned char card);


public:

	// ���õ�ǰ�����˵ķ�λ
	void setDirection(enDirection drc);
	enDirection getDirection(void) const;

	// �����ƾ�
	void setVisibleMahjongPool(SRMahjongPool* pool);
	void setInvisibleMahjongPool(SRMahjongPool* pool);

	// �����齫										  
	const SRMahjong* getMahjong(void) const;
	const SRMahjong* getMahjong(enDirection drc) const;
	void setMahjong(enDirection drc, SRMahjong* mahjong);

#ifdef SRMJROBOT_EXPORTS
private:
	// ���Ƿ�λ
	enDirection direction_;

	// �Ƴ��е��齫
	SRMahjongPool* visibleMahjongPool_;
	// ��ǽ�ϵ��齫
	SRMahjongPool* invisibleMahjongPool_;

	// ���з�λ���齫
	SRMahjong* mahjong_[5];

	// �����Ƿ����
	int isTing_;
	// ������
	unsigned char arrTing_[14];
	// ����������
	unsigned char numTing_;
#endif
};

//
//
//// ��ʼ��
//SRMJROBOT_API int SRInit(void);
//
//// ����
//SRMJROBOT_API int SRResetGame(void);
//
//// �����ƾ�
//SRMJROBOT_API int SRSetGameCardData(void);
//
//// ���������
//SRMJROBOT_API int SRSetPlayerCardData(void);
//
//// �������˹�
//SRMJROBOT_API int SRSetGameProtagonists(void);
//
//
//// ѯ�ʳ���(�Լ�����ʱ)
//SRMJROBOT_API int SRAskOutCard(void);
//
//// ѯ����Ϊ(�����˳���ʱ)
//SRMJROBOT_API int SRAskAction(void);
//
//// ѯ�ʺ���
//SRMJROBOT_API int SRAskWinCard(void);
//
//// ����ʼ��
//SRMJROBOT_API int SRUninit(void);
//



#endif // _SHANREN_ROBOT_H_