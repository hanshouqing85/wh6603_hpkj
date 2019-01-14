#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"
//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�˿�����
#define CT_SINGLE					1									//��������
#define CT_ONE_LONG					2									//��������
#define CT_TWO_LONG					3									//��������
#define CT_THREE_TIAO				4									//��������
#define	CT_SHUN_ZI					5									//˳������
#define CT_TONG_HUA					6									//ͬ������
#define CT_HU_LU					7									//��«����
#define CT_TIE_ZHI					8									//��֧����
#define CT_TONG_HUA_SHUN			9									//ͬ��˳��
#define CT_KING_TONG_HUA_SHUN		10									//�ʼ�ͬ��˳

//////////////////////////////////////////////////////////////////////////

//ʤ����Ϣ�ṹ
struct UserWinList
{
	BYTE bSameCount;
	WORD wWinerList[GAME_PLAYER];
};

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbLONGCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE 							cbFourLogicVolue[1];				//�����б�
	BYTE 							cbThreeLogicVolue[1];				//�����б�
	BYTE 							cbLONGLogicVolue[2];				//�����б�
	BYTE 							cbSignedLogicVolue[5];				//�����б�
	BYTE							cbFourCardData[MAX_CENTERCOUNT];	//�����б�
	BYTE							cbThreeCardData[MAX_CENTERCOUNT];	//�����б�
	BYTE							cbLONGCardData[MAX_CENTERCOUNT];	//�����б�
	BYTE							cbSignedCardData[MAX_CENTERCOUNT];	//������Ŀ
};
//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//��������
private:
	static BYTE						m_cbCardData[FULL_COUNT];				//�˿˶���

	//���ƺ���
public:
	//�����˿�
	static void SortCardList(BYTE cbCardData[], BYTE cbCardCount);
	//�����˿�
	static VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);

	//���ͺ���
public:
	//��ȡ����
	static BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ��ֵ
	static BYTE GetCardValue(BYTE cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	static BYTE GetCardColor(BYTE cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }

	//���ܺ���
public:
	//�߼���ֵ
	static BYTE GetCardLogicValue(BYTE cbCardData);
	//�Ա��˿�
	static BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
	//�����˿�
	static void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult);
	//7��5
	static BYTE FiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount);
	//�������
	static bool SelectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const DZPKSCORE lAddScore[]);
};

//���庯��������������
class sort_tagMadeHandsOrder
{
public:
	sort_tagMadeHandsOrder(){}
	bool operator()(tagMadeHandsOrder& X,tagMadeHandsOrder& Y)const
	{
		BYTE iRet=CGameLogic::CompareCard(X.cbLastCenterCardData,Y.cbLastCenterCardData,MAX_CENTERCOUNT);
		// �ڵ¿���˹�˿�����ֻ�ɫ���ִ�С
		if(iRet==0)
		{
		   // �õ�һ���ȶ��������ϵ
		   BYTE XLogic[5];
		   XLogic[0]=CGameLogic::GetCardLogicValue(X.cbCardData[0]);
		   XLogic[1]=CGameLogic::GetCardLogicValue(X.cbCardData[1]);
		   XLogic[2]=CARD_COLOR(X.cbCardData[0]);
		   XLogic[3]=CARD_COLOR(X.cbCardData[1]);
		   XLogic[4]=(XLogic[0]+XLogic[1]+XLogic[2]+XLogic[3]);
		   BYTE YLogic[5];
		   YLogic[0]=CGameLogic::GetCardLogicValue(Y.cbCardData[0]);
		   YLogic[1]=CGameLogic::GetCardLogicValue(Y.cbCardData[1]);
		   YLogic[2]=CARD_COLOR(Y.cbCardData[0]);
		   YLogic[3]=CARD_COLOR(Y.cbCardData[1]);
		   YLogic[4]=(YLogic[0]+YLogic[1]+YLogic[2]+YLogic[3]);
		   return XLogic[4]>YLogic[4];
		}
		return iRet>1;
	}
};

//////////////////////////////////////////////////////////////////////////

#endif
