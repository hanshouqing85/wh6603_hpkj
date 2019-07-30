#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

//��������
const BYTE cbIndexCount=5;

//�˿�����
const BYTE	CGameLogic::m_cbCardData[FULL_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
	0x4E,0x4F,
};

//��������
const BYTE	CGameLogic::m_cbGoodcardData[GOOD_CARD_INDEX][GOOD_CARD_COUTN]=
{
//////////////1
0x4E,0x4F,0x03,0x24,0x25,0x06,0x37,0x28,0x0B,0x1B,0x3B,0x1D,0x2D,0x01,0x21,0x22,0x32,						
//////////////////2
	
0x4E,0x4F,0x13,0x14,0x25,0x36,0x17,0x08,0x09,0x2A,0x33,0x24,0x05,0x06,0x37,0x22,0x32,									

/////////////////3

0x4E,0x4F,0x15,0x06,0x17,0x18,0x39,0x1A,0x23,0x14,0x05,0x36,0x27,0x22,0x02,0x32,0x31,

///////////////////////4

0x03,0x13,0x23,0x33,0x14,0x25,0x06,0x07,0x18,0x39,0x2A,0x2B,0x1C,0x0D,0x4F,0x32,0x02,												

///////////////////////5

0x08,0x19,0x1A,0x18,0x29,0x3A,0x38,0x09,0x0A,0x12,0x32,0x02,0x14,0x15,0x36,0x37,0x28,

///////////////////////6

0x15,0x25,0x35,0x3D,0x0D,0x1D,0x03,0x13,0x39,0x19,0x12,0x32,0x02,0x4F,0x27,0x28,0x33,


///////////////////////7

0x18,0x09,0x1A,0x2B,0x3C,0x0D,0x31,0x04,0x34,0x14,0x27,0x37,0x2C,0x1C,0x02,0x32,0x4F,


///////////////////////8

0x25,0x05,0x36,0x16,0x27,0x07,0x28,0x38,0x31,0x11,0x01,0x22,0x32,0x2B,0x1B,0x4F,0x3C,


///////////////////////9

0x05,0x15,0x35,0x27,0x17,0x07,0x1A,0x3A,0x0A,0x2A,0x22,0x2B,0x4F,0x4E,0x04,0x14,0x38,                                     

///////////////////////10

0x27,0x17,0x29,0x19,0x09,0x39,0x3C,0x0C,0x1C,0x4F,0x1A,0x03,0x33,0x04,0x14,0x25,0x05,                

///////////////////////11

0x06,0x07,0x18,0x39,0x2A,0x2B,0x1C,0x02,0x32,0x09,0x19,0x31,0x11,0x01,0x4E,0x4F,0x03,                                       

///////////////////////12

0x03,0x24,0x25,0x06,0x37,0x3A,0x0A,0x0D,0x3D,0x19,0x09,0x39,0x22,0x32,0x12,0x4E,0x2A,

///////////////////////13

0x03,0x33,0x23,0x24,0x14,0x04,0x32,0x12,0x18,0x28,0x2B,0x0B,0x4F,0x2A,0x06,0x3D,0x39,

//////////////14
0x4E,0x4F,0x33,0x14,0x25,0x16,0x07,0x18,0x09,0x19,0x39,0x0D,0x3D,0x01,0x21,0x22,0x32,


//////////////15
0x4E,0x03,0x14,0x05,0x16,0x07,0x18,0x09,0x19,0x1D,0x0D,0x3D,0x2D,0x01,0x21,0x22,0x32,

//////////////16
0x4E,0x4F,0x23,0x34,0x05,0x16,0x27,0x28,0x09,0x1A,0x3B,0x1D,0x2D,0x01,0x21,0x31,0x32,						
//////////////////17

0x4E,0x13,0x33,0x14,0x04,0x15,0x05,0x36,0x16,0x06,0x26,0x09,0x2A,0x21,0x12,0x22,0x32,									

/////////////////18

0x4E,0x4F,0x33,0x04,0x15,0x05,0x36,0x16,0x07,0x37,0x18,0x38,0x09,0x29,0x1A,0x3A,0x32,

///////////////////////19

0x03,0x23,0x15,0x35,0x05,0x18,0x19,0x3A,0x3B,0x1C,0x2D,0x21,0x31,0x12,0x22,0x32,0x4F,												

///////////////////////20

0x23,0x04,0x18,0x28,0x38,0x09,0x29,0x19,0x3B,0x1C,0x2D,0x3D,0x0D,0x31,0x22,0x4E,0x4F,

///////////////////////21

0x04,0x34,0x07,0x37,0x27,0x17,0x18,0x38,0x09,0x29,0x31,0x21,0x01,0x12,0x22,0x32,0x02,


///////////////////////22

0x23,0x04,0x35,0x36,0x17,0x18,0x09,0x1A,0x0A,0x2A,0x3B,0x1C,0x2D,0x21,0x01,0x11,0x31,


///////////////////////23

0x25,0x36,0x27,0x28,0x39,0x1A,0x0B,0x2B,0x3C,0x2C,0x1D,0x2D,0x3D,0x11,0x31,0x32,0x02,


///////////////////////24

0x26,0x16,0x06,0x36,0x1D,0x3D,0x0D,0x22,0x2B,0x4F,0x04,0x14,0x38,0x09,0x1A,0x3B,0x0C,                       


///////////////////////25

0x39,0x19,0x28,0x18,0x08,0x38,0x4F,0x1A,0x03,0x33,0x04,0x14,0x25,0x05,0x02,0x32,0x1D,           


///////////////////////26

0x03,0x04,0x15,0x36,0x27,0x28,0x19,0x02,0x32,0x09,0x39,0x31,0x1D,0x2D,0x0D,0x4F,0x01,                                       


///////////////////////27

0x03,0x24,0x25,0x06,0x37,0x0A,0x2A,0x1D,0x3D,0x29,0x09,0x39,0x22,0x32,0x12,0x4E,0x08,


///////////////////////28

0x15,0x35,0x25,0x26,0x16,0x06,0x02,0x12,0x17,0x27,0x2B,0x0B,0x4F,0x2A,0x03,0x3D,0x09,


//////////////29
0x4E,0x03,0x34,0x15,0x16,0x07,0x38,0x09,0x13,0x23,0x33,0x3D,0x01,0x21,0x22,0x02,0x1C,


//////////////30
0x4F,0x03,0x14,0x05,0x16,0x07,0x18,0x09,0x19,0x1C,0x0C,0x3C,0x2C,0x22,0x32,0x1A,0x2B



};
//////////////////////////////////////////////////////////////////////////////////



//��������1
const BYTE	CGameLogic::m_cbGoodcardData1[GOOD_CARD_INDEX1][GOOD_CARD_COUTN1]=
{
	0x02,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x11,0x12,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x21,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x31,0x32,0x3A,0x3B,0x3C,0x3D,0x4E,0x4F,

	0x01,0x02,0x07,0x08,0x09,0x0A,0x0C,0x0D,0x11,0x12,0x17,0x18,0x19,0x1A,0x1C,0x1D,0x21,0x22,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x31,0x32,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,

	0x02,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x11,0x12,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x21,0x22,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x31,0x32,0x4E,0x4F
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//������
	switch (cbCardCount)
	{
	case 0:	//����
		{
			return CT_ERROR;
		}
	case 1: //����
		{
			return CT_SINGLE;
		}
	case 2:	//���ƻ��
		{
			//�����ж�
			if ((cbCardData[0]==0x4F)&&(cbCardData[1]==0x4E)) return CT_MISSILE_CARD;
			if (GetCardLogicValue(cbCardData[0])==GetCardLogicValue(cbCardData[1])) return CT_DOUBLE;

			return CT_ERROR;
		}
	}

	//�����˿�
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardData,cbCardCount,AnalyseResult);

	//�����ж�
	if (AnalyseResult.cbBlockCount[3]>0)
	{
		//�����ж�
		if ((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==4)) return CT_BOMB_CARD;
		if ((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==6)) return CT_FOUR_TAKE_ONE;
		if ((AnalyseResult.cbBlockCount[3]==1)&&(cbCardCount==8)&&(AnalyseResult.cbBlockCount[1]==2)) return CT_FOUR_TAKE_TWO;


		return CT_ERROR;
	}

	//�����ж�
	if (AnalyseResult.cbBlockCount[2]>0)
	{
		//�����ж�
		if (AnalyseResult.cbBlockCount[2]>1)
		{
			//��������
			BYTE cbCardData=AnalyseResult.cbCardData[2][0];
			BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

			//�������
			if (cbFirstLogicValue>=15) return CT_ERROR;

			//�����ж�
			for (BYTE i=1;i<AnalyseResult.cbBlockCount[2];i++)
			{
				BYTE cbCardData=AnalyseResult.cbCardData[2][i*3];
				if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
			}
		}
		else if( cbCardCount == 3 ) return CT_THREE;

		//�����ж�
		if (AnalyseResult.cbBlockCount[2]*3==cbCardCount) return CT_THREE_LINE;
		if (AnalyseResult.cbBlockCount[2]*4==cbCardCount) return CT_THREE_TAKE_ONE;
		if ((AnalyseResult.cbBlockCount[2]*5==cbCardCount)&&(AnalyseResult.cbBlockCount[1]==AnalyseResult.cbBlockCount[2])) return CT_THREE_TAKE_TWO;

		return CT_ERROR;
	}

	//��������
	if (AnalyseResult.cbBlockCount[1]>=3)
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbCardData[1][0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbBlockCount[1];i++)
		{
			BYTE cbCardData=AnalyseResult.cbCardData[1][i*2];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		//�����ж�
		if ((AnalyseResult.cbBlockCount[1]*2)==cbCardCount) return CT_DOUBLE_LINE;

		return CT_ERROR;
	}

	//�����ж�
	if ((AnalyseResult.cbBlockCount[0]>=5)&&(AnalyseResult.cbBlockCount[0]==cbCardCount))
	{
		//��������
		BYTE cbCardData=AnalyseResult.cbCardData[0][0];
		BYTE cbFirstLogicValue=GetCardLogicValue(cbCardData);

		//�������
		if (cbFirstLogicValue>=15) return CT_ERROR;

		//�����ж�
		for (BYTE i=1;i<AnalyseResult.cbBlockCount[0];i++)
		{
			BYTE cbCardData=AnalyseResult.cbCardData[0][i];
			if (cbFirstLogicValue!=(GetCardLogicValue(cbCardData)+i)) return CT_ERROR;
		}

		return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}

//�����˿�
VOID CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;
	if (cbSortType==ST_CUSTOM) return;

	//ת����ֵ
	BYTE cbSortValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//���ñ�־
				bSorted=false;

				//�˿�����
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;

				//����Ȩλ
				cbSwitchData=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	//��Ŀ����
	if (cbSortType==ST_COUNT)
	{
		//��������
		BYTE cbCardIndex=0;

		//�����˿�
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(&cbCardData[cbCardIndex],cbCardCount-cbCardIndex,AnalyseResult);

		//��ȡ�˿�
		for (BYTE i=0;i<CountArray(AnalyseResult.cbBlockCount);i++)
		{
			//�����˿�
			BYTE cbIndex=CountArray(AnalyseResult.cbBlockCount)-i-1;
			CopyMemory(&cbCardData[cbCardIndex],AnalyseResult.cbCardData[cbIndex],AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE));

			//��������
			cbCardIndex+=AnalyseResult.cbBlockCount[cbIndex]*(cbIndex+1)*sizeof(BYTE);
		}
	}

	return;
}

//�����˿�
VOID CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardData)];
	CopyMemory(cbCardData,m_cbCardData,sizeof(m_cbCardData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCardList(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	if (cbDeleteCount!=cbRemoveCount) return false;

	ZeroMemory(cbCardData,cbCardCount*sizeof(cbCardData[0]));
	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	if(cbCardValue<=0 || cbCardValue>(MASK_VALUE&0x4f))
		return 0 ;
	ASSERT(cbCardValue>0 && cbCardValue<=(MASK_VALUE&0x4f));

	//ת����ֵ
	if (cbCardColor==0x40) return cbCardValue+2;
	return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
}

//�Ա��˿�
bool CGameLogic::CompareCard(const BYTE cbFirstCard[], const BYTE cbNextCard[], BYTE cbFirstCount, BYTE cbNextCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextCard,cbNextCount);
	BYTE cbFirstType=GetCardType(cbFirstCard,cbFirstCount);

	//�����ж�
	if (cbNextType==CT_ERROR) return false;
	if (cbNextType==CT_MISSILE_CARD) return true;

	//ը���ж�
	if ((cbFirstType!=CT_BOMB_CARD)&&(cbNextType==CT_BOMB_CARD)) return true;
	if ((cbFirstType==CT_BOMB_CARD)&&(cbNextType!=CT_BOMB_CARD)) return false;

	//�����ж�
	if ((cbFirstType!=cbNextType)||(cbFirstCount!=cbNextCount)) return false;

	//��ʼ�Ա�
	switch (cbNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB_CARD:
		{
			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(cbNextCard[0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(cbFirstCard[0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_THREE_TAKE_ONE:
	case CT_THREE_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbCardData[2][0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbCardData[2][0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	case CT_FOUR_TAKE_ONE:
	case CT_FOUR_TAKE_TWO:
		{
			//�����˿�
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(cbNextCard,cbNextCount,NextResult);
			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);

			//��ȡ��ֵ
			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbCardData[3][0]);
			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbCardData[3][0]);

			//�Ա��˿�
			return cbNextLogicValue>cbFirstLogicValue;
		}
	}
	
	return false;
}

//�����˿�
BYTE CGameLogic::MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex)
{
	return (cbColorIndex<<4)|(cbValueIndex+1);
}

//�����˿�
VOID CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1,cbCardValueTemp=0;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		if(cbSameCount > 4)
		{
			ASSERT(FALSE);
			//���ý��
			ZeroMemory(&AnalyseResult, sizeof(AnalyseResult));
			return;
		}

		//���ý��
		BYTE cbIndex=AnalyseResult.cbBlockCount[cbSameCount-1]++;
		for (BYTE j=0;j<cbSameCount;j++) AnalyseResult.cbCardData[cbSameCount-1][cbIndex*cbSameCount+j]=cbCardData[i+j];

		//��������
		i+=cbSameCount-1;
	}

	return;
}

//�����ֲ�
VOID CGameLogic::AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing)
{
	//���ñ���
	ZeroMemory(&Distributing,sizeof(Distributing));

	//���ñ���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbCardData[i]==0) continue;

		//��ȡ����
		BYTE cbCardColor=GetCardColor(cbCardData[i]);
		BYTE cbCardValue=GetCardValue(cbCardData[i]);

		//�ֲ���Ϣ
		Distributing.cbCardCount++;
		Distributing.cbDistributing[cbCardValue-1][cbIndexCount]++;
		Distributing.cbDistributing[cbCardValue-1][cbCardColor>>4]++;
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�õ�����
void CGameLogic::GetGoodCardData(BYTE cbGoodCardData[NORMAL_COUNT])
{
	srand( (unsigned)time(NULL));
	WORD wGoodIndex=rand()%2;
	
	if(wGoodIndex==0){
		srand( (unsigned)time(NULL));
		//��ȡ��һ�ֺ���
		WORD wIndex=rand()%GOOD_CARD_INDEX;
		//����׼��
		BYTE cbCardData[CountArray(m_cbGoodcardData[wIndex])];
		BYTE cbCardBuffer[CountArray(m_cbGoodcardData[wIndex])];
		CopyMemory(cbCardData,m_cbGoodcardData[wIndex],sizeof(m_cbGoodcardData[wIndex]));

		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		BYTE cbBufferCount=CountArray(m_cbGoodcardData[wIndex]);
		do
		{
			cbPosition=rand()%(cbBufferCount-cbRandCount);
			cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
			cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
		} while (cbRandCount<cbBufferCount);

		//���ƺ���
		CopyMemory(cbGoodCardData, cbCardBuffer, NORMAL_COUNT);
	
	}else{
		srand( (unsigned)time(NULL));
		//��ȡ�ڶ��ֺ���
		WORD wIndex=rand()%GOOD_CARD_INDEX1;
		//����׼��
		BYTE cbCardData[CountArray(m_cbGoodcardData1[wIndex])];
		BYTE cbCardBuffer[CountArray(m_cbGoodcardData1[wIndex])];
		CopyMemory(cbCardData,m_cbGoodcardData1[wIndex],sizeof(m_cbGoodcardData1[wIndex]));

		//�����˿�
		BYTE cbRandCount=0,cbPosition=0;
		BYTE cbBufferCount=CountArray(m_cbGoodcardData1[wIndex]);
		do
		{
			cbPosition=rand()%(cbBufferCount-cbRandCount);
			cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
			cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
		} while (cbRandCount<cbBufferCount);

		//���ƺ���
		CopyMemory(cbGoodCardData, cbCardBuffer, NORMAL_COUNT);
	
	}

	
	
	

}

//ɾ������
bool CGameLogic::RemoveGoodCardData(BYTE cbGoodcardData[NORMAL_COUNT], BYTE cbGoodCardCount, BYTE cbCardData[FULL_COUNT], BYTE cbCardCount) 
{
	//��������
	ASSERT(cbGoodCardCount<=cbCardCount);
	if(cbGoodCardCount>cbCardCount)
		return false ;

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[FULL_COUNT];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbGoodCardCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbGoodcardData[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}
	ASSERT(cbDeleteCount==cbGoodCardCount) ;
	if (cbDeleteCount!=cbGoodCardCount) return false;

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}