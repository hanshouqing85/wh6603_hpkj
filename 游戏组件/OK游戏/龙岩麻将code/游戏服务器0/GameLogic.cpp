#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//构造函数
CChiHuRight::CChiHuRight()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );

	if( !m_bInit )
	{
		m_bInit = true;
		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( 0 == i )
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (DWORD(pow((float)2,(float)(i-1))))<<28;
		}
	}
}

//赋值符重载
CChiHuRight & CChiHuRight::operator = ( DWORD dwRight )
{
	DWORD dwOtherRight = 0;
	//验证权位
	if( !IsValidRight( dwRight ) )
	{
		//验证取反权位
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//与等于
CChiHuRight & CChiHuRight::operator &= ( DWORD dwRight )
{
	bool bNavigate = false;
	//验证权位
	if( !IsValidRight( dwRight ) )
	{
		//验证取反权位
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		//调整权位
		DWORD dwHeadRight = (~dwRight)&0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight|dwTailRight;
		bNavigate = true;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if( !bNavigate )
			m_dwRight[i] = 0;
	}

	return *this;
}

//或等于
CChiHuRight & CChiHuRight::operator |= ( DWORD dwRight )
{
	//验证权位
	if( !IsValidRight( dwRight ) ) return *this;

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
		{
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
			break;
		}
	}

	return *this;
}

//与
CChiHuRight CChiHuRight::operator & ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//相等
bool CChiHuRight::operator == ( DWORD dwRight ) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)==chr;
}

//相等
bool CChiHuRight::operator == ( const CChiHuRight chr ) const
{
	for( WORD i = 0; i < CountArray( m_dwRight ); i++ )
	{
		if( m_dwRight[i] != chr.m_dwRight[i] ) return false;
	}
	return true;
}

//不相等
bool CChiHuRight::operator != ( DWORD dwRight ) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)!=chr;
}

//不相等
bool CChiHuRight::operator != ( const CChiHuRight chr ) const
{
	return !((*this)==chr);
}

//是否权位为空
bool CChiHuRight::IsEmpty()
{
	for( BYTE i = 0; i < CountArray(m_dwRight); i++ )
		if( m_dwRight[i] ) return false;
	return true;
}

//设置权位为空
void CChiHuRight::SetEmpty()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	return;
}

//获取权位数值
BYTE CChiHuRight::GetRightData( DWORD dwRight[], BYTE cbMaxCount )
{
	ASSERT( cbMaxCount >= CountArray(m_dwRight) );
	if( cbMaxCount < CountArray(m_dwRight) ) return 0;

	CopyMemory( dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight) );
	return CountArray(m_dwRight);
}

//设置权位数值
bool CChiHuRight::SetRightData( const DWORD dwRight[], BYTE cbRightCount )
{
	ASSERT( cbRightCount <= CountArray(m_dwRight) );
	if( cbRightCount > CountArray(m_dwRight) ) return false;

	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	CopyMemory( m_dwRight,dwRight,sizeof(DWORD)*cbRightCount );
	
	return true;
}

//检查仅位是否正确
bool CChiHuRight::IsValidRight( DWORD dwRight )
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		if( m_dwRightMask[i] == dwRightHead ) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////

//静态变量

//扑克数据
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//筒子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//筒子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//筒子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//筒子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//万子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//条子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//条子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//条子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//条子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
		//56  57   58   59    60   61   62   63
	0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f								//春夏秋冬梅兰菊竹(index = 34~41)
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = SwitchToCardIndex(CAI_SHEN_CARD);
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//混乱准备
	cbMaxCount-=1;
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray)-1);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);
	cbCardData[cbRandCount] = cbCardData[0];
	cbCardData[0] = 0x37;
	return;
}

//混乱扑克
void CGameLogic::RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[])
{
	if( cbCardCount == 0 ) return;

	//混乱准备
	BYTE cbCardDataTemp[MAX_COUNT];
	CopyMemory(cbCardDataTemp,cbCardData,sizeof(BYTE)*cbCardCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbCardCount-cbRandCount);
		cbRandData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbCardCount-cbRandCount];
	} while (cbRandCount<cbCardCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);
		if(!IsValidCard(cbRemoveCard[i]))
			break;
		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//检验数据
	ASSERT(cbCardCount<=MAX_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
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

	//成功判断
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=0x0f)&&(cbColor==3)));
}

//扑克数目
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard-2;

			return 3;
		}
	case WIK_CENTER:	//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//碰牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//杠牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//杠牌等级
	if (cbUserAction&WIK_GANG) { return 3; }

	//碰牌等级
	if (cbUserAction&WIK_PENG) { return 2; }

	//上牌等级
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//胡牌等级
WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight,bool bSecondMagic)
{
	WORD wFanShu = 1;
	//手上有没有花牌
	bool bHandHasMagic=false;
	if( !(ChiHuRight&CHR_HAS_MAGIC).IsEmpty() )
		bHandHasMagic=true;
	
	////特殊番型
	//if( !(ChiHuRight&CHR_QI_DUI).IsEmpty() )
	//	wFanShu *= 2;
	//else if( !(ChiHuRight&CHR_QI_DUI1).IsEmpty() )
	//	wFanShu *= 4;
	//else if( !(ChiHuRight&CHR_QI_DUI2).IsEmpty() )
	//	wFanShu *= 8;
	//else if( !(ChiHuRight&CHR_QI_DUI3).IsEmpty() )
	//	wFanShu *= 16;

	if( !(ChiHuRight&CHR_GANG_MAGIC).IsEmpty() )
		wFanShu *= bSecondMagic?8:4;		//16:8
	else if( !(ChiHuRight&CHR_MAGIC_GANG).IsEmpty() )
		wFanShu *= bSecondMagic?8:4;		//16:8
	else if( !(ChiHuRight&CHR_MAGIC_HU3).IsEmpty() )//多飘花
		wFanShu *= bSecondMagic?8:4;
	else if( !(ChiHuRight&CHR_MAGIC_HU2).IsEmpty() )
		wFanShu *= bSecondMagic?8:4;		
	else if( !(ChiHuRight&CHR_GANG_KAI_MAGIC).IsEmpty() )
		wFanShu *= 4;
	else if( !(ChiHuRight&CHR_MAGIC_HU).IsEmpty() )
		wFanShu *= 2;
	else if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() )	//2:4
	{
		wFanShu *= 2;
	}
	//有花大吊=4
	if( !(ChiHuRight&CHR_MAGIC_DADIAO).IsEmpty())
		wFanShu = 4;	
	//无花大吊=8； 无花大吊接炮：10（对方出）
	else if( !(ChiHuRight&CHR_NO_MAGIC_DADIAO).IsEmpty())
		wFanShu = 8;
	//自摸
	if( !(ChiHuRight&CHR_HU_ZIMO).IsEmpty())
	{
		if(bHandHasMagic)		//有花自摸
			wFanShu *= 1;
		else 
			wFanShu *= 2;		// 无花自摸：2
	}
	//横翻翻倍
	if( !(ChiHuRight&CHR_HENG_FAN).IsEmpty() )
		wFanShu *= 2;
	//三花翻倍
	if( !(ChiHuRight&CHR_THREE_MAGIC).IsEmpty() )
		wFanShu *= 2;
	//16倍封顶
	if(wFanShu >= 16)
		wFanShu=16;

	return wFanShu;
}

//吃牌判断
BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
		return WIK_NULL;

	//变量定义
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//吃牌判断
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//吃牌判断
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//吃牌不能包含有王霸
			if( m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
				continue;

			//设置类型
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//碰牌判断
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard) ) 
		return WIK_NULL;

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//杠牌判断
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard) ) 
		return WIK_NULL;

	//杠牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//杠牌分析
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//手上杠牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex ) continue;
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

bool CGameLogic::IsDaDiao(const BYTE cbCardIndex[MAX_INDEX])
{
	BYTE cbCardCount=GetCardCount(cbCardIndex);
	return (cbCardCount <= 2);
}

//吃胡分析//*
BYTE CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	//变量定义
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//设置变量
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCard一定不为0			!!!!!!!!!
	ASSERT( cbCurrentCard != 0 );
	if( cbCurrentCard == 0 ) return WIK_NULL;

	/*
	//	特殊番型
	*/
	//七小对牌
	BYTE cbValue = IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard);
	//if(cbValue==4) ChiHuRight |= CHR_QI_DUI3;
	//else if(cbValue==3) ChiHuRight |= CHR_QI_DUI2;
	//else if(cbValue==2) ChiHuRight |= CHR_QI_DUI1;
	//else if(cbValue==1) ChiHuRight |= CHR_QI_DUI;

	//插入扑克
	if (cbCurrentCard!=0) 
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	//分析扑克
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//胡牌分析
	if (AnalyseItemArray.GetCount()>0)
	{
		ChiHuRight |= CHR_WU_FAN;
	}

	if( !ChiHuRight.IsEmpty() ) 
	{
		//三花
		if(getMagicCardCount(cbCardIndex) == 3)
			ChiHuRight |= CHR_THREE_MAGIC;

		//计算数目
		BYTE cbCardCount=GetCardCount(cbCardIndex);
		if(cbCardCount <= 2 && getMagicCardCount(cbCardIndex)==0)
			ChiHuRight |= CHR_NO_MAGIC_DADIAO;				//无花大吊(分为自摸8，接炮10)
		else if(cbCardCount <= 2 )							
			ChiHuRight |= CHR_MAGIC_DADIAO;					//有花大吊

		if(getMagicCardCount(cbCardIndexTemp) > 0)
			ChiHuRight |= CHR_HAS_MAGIC;

		//判断财神吊牌
		if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]--;
		BYTE j=0;
		for(  j = MAX_INDEX-9; j < MAX_INDEX-2; j++ )
		{
			if(IsQiXiaoDui(cbCardIndexTemp,WeaveItem,cbWeaveCount,SwitchToCardData(j)))
				continue;

			//胡牌分析
			cbCardIndexTemp[j]++;
			AnalyseItemArray.RemoveAll();
			AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);
			if (AnalyseItemArray.GetCount()==0)
				break;
			cbCardIndexTemp[j]--;
		}

		//财神吊牌
		if(j == MAX_INDEX-2)
		{
			ChiHuRight |= CHR_MAGIC_EYE;
			//暴头条件：除了将牌其他都已经成刻字或者顺子；剩下2张牌，当前牌是普通牌，另外一张牌是财神牌
			if(!IsMagicCard(cbCurrentCard) && getMagicCardCount(cbCardIndex) > 0)
			{
				OutputDebugString(_T("暴头了！！！！！！！！！！\n"));
				ChiHuRight |= CHR_MAGIC_HU;
			}
		}
		cbChiHuKind = WIK_CHI_HU;
	}

	return cbChiHuKind;
}


//听牌分析
BYTE CGameLogic::AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	//复制数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					return WIK_LISTEN;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
				return WIK_LISTEN;
		}
	}

	return WIK_NULL;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	if( cbCardIndex < 27 )
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	else return (0x30|(cbCardIndex-27+1));
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//转换扑克
	BYTE cbPosition=0;
	//钻牌
	if( m_cbMagicIndex != MAX_INDEX )
	{
		for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex && m_cbMagicIndex != INDEX_REPLACE_CARD ) 
		{
			//如果财神有代替牌，则代替牌代替财神原来的位置
			if( INDEX_REPLACE_CARD != MAX_INDEX )
			{
				for( BYTE j = 0; j < cbCardIndex[INDEX_REPLACE_CARD]; j++ )
					cbCardData[cbPosition++] = SwitchToCardData(INDEX_REPLACE_CARD);
			}
			continue;
		}
		if( i == INDEX_REPLACE_CARD ) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], std::vector<BYTE>& cbCardData)
{
	//转换扑克
	//钻牌
	if( m_cbMagicIndex != MAX_INDEX )
	{
		for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
			cbCardData.push_back(SwitchToCardData(m_cbMagicIndex)) ;
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex ) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbCardData.size()<MAX_COUNT);
				cbCardData.push_back(SwitchToCardData(i));
			}
		}
	}

	return cbCardData.size();
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

BYTE CGameLogic::SwitchToCardIndex(const std::vector<BYTE>& cbCardData, BYTE cbCardIndex[MAX_INDEX] )
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	int cbCardCount = cbCardData.size();
	//转换扑克
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}


BYTE CGameLogic::GetCardColor(BYTE cbCardDat)
{
	ASSERT(IsValidCard(cbCardDat));
	return ((cbCardDat&MASK_COLOR)>>4)+1;
}

BYTE CGameLogic::GetCardValue(BYTE cbCardDat)
{
	ASSERT(IsValidCard(cbCardDat));
	return (cbCardDat&MASK_VALUE);
}
//分析扑克
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//计算数目
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//效验数目
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+28];
	ZeroMemory(KindItem,sizeof(KindItem));

	//需求判断
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==MAX_WEAVE);

	//单吊判断
	if (cbLessKindItem==0)
	{
		//效验参数
		ASSERT((cbCardCount==2)&&(cbWeaveCount==MAX_WEAVE));

		//牌眼判断
		for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
		{
			if (cbCardIndex[i]==2 || 
				( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					CopyMemory( AnalyseItem.cbCardData[j],WeaveItem[j].cbCardData,sizeof(WeaveItem[j].cbCardData) );
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分分析
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	//如果有财神
	BYTE cbMagicCardCount = 0;
	if( m_cbMagicIndex != MAX_INDEX )
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//如果财神有代替牌，财神与代替牌转换
		if( INDEX_REPLACE_CARD != MAX_INDEX )
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD-1] = cbMagicCardCount;
		}
	}
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
		{
			//同牌判断
			//如果是财神,并且财神数小于3,则不进行组合
			if( cbMagicCardIndex[i] >= 3 || ( cbMagicCardIndex[i]+cbMagicCardCount >= 3 &&
				( ( INDEX_REPLACE_CARD!=MAX_INDEX && i != INDEX_REPLACE_CARD ) || ( INDEX_REPLACE_CARD==MAX_INDEX && i != m_cbMagicIndex ) ) )
				)
			{
				int nTempIndex = cbMagicCardIndex[i];
				do
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					BYTE cbIndex = i;
					BYTE cbCenterCard = SwitchToCardData(i);
					//如果是财神且财神有代替牌,则换成代替牌
					if( i == m_cbMagicIndex && INDEX_REPLACE_CARD != MAX_INDEX )
					{
						cbIndex = INDEX_REPLACE_CARD;
						cbCenterCard = SwitchToCardData(INDEX_REPLACE_CARD);
					}
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=cbCenterCard;
					KindItem[cbKindItemCount].cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
					cbKindItemCount++;

					//如果是财神,则退出
					if( i == INDEX_REPLACE_CARD || (i == m_cbMagicIndex && INDEX_REPLACE_CARD == MAX_INDEX) )
						break;

					nTempIndex -= 3;
					//如果刚好搭配全部，则退出
					if( nTempIndex == 0 ) break;

				}while( nTempIndex+cbMagicCardCount >= 3 );
			}

			//连牌判断
			if ((i<(MAX_INDEX-MAX_HUA_CARD-9))&&((i%9)<7))
			{
				//只要财神牌数加上3个顺序索引的牌数大于等于3,则进行组合
				if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
				{
					BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i+1],cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
					{
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = (i+j==m_cbMagicIndex&&INDEX_REPLACE_CARD!=MAX_INDEX)?INDEX_REPLACE_CARD:i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if( nMagicCountTemp >= 0 )
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
							cbKindItemCount++;
						}
						else break;
					}
				}
			}
		}
	}

	//组合分析
	if (cbKindItemCount>=cbLessKindItem)
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[MAX_WEAVE];
		for( BYTE i = 0; i < CountArray(cbIndex); i++ )
			cbIndex[i] = i;
		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem,sizeof(pKindItem));
		tagKindItem KindItemTemp[CountArray(KindItem)];
		if( m_cbMagicIndex != MAX_INDEX )
			CopyMemory( KindItemTemp,KindItem,sizeof(KindItem) );

		//开始组合
		do
		{
			//设置变量
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			cbMagicCardCount = 0;
			if( m_cbMagicIndex != MAX_INDEX )
			{
				CopyMemory( KindItem,KindItemTemp,sizeof(KindItem) );
			}

			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//数量判断
			bool bEnoughCard=true;
			
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//存在判断
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					if( m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex] > 0 )
					{
						cbCardIndexTemp[m_cbMagicIndex]--;
						pKindItem[i/3]->cbValidIndex[i%3] = m_cbMagicIndex;
					}
					else
					{
						bEnoughCard=false;
						break;
					}
				}
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//胡牌判断
			if (bEnoughCard==true)
			{
				//牌眼判断
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				if( GetCardCount(cbCardIndexTemp) == 2 )
				{
					for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
					{
						if (cbCardIndexTemp[i]==2)
						{
							cbCardEye=SwitchToCardData(i);
							if( m_cbMagicIndex != MAX_INDEX && i == m_cbMagicIndex ) bMagicEye = true;
							break;
						}
						else if( i!=m_cbMagicIndex && 
							m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
						{
							cbCardEye = SwitchToCardData(i);
							bMagicEye = true;
						}
					}
				}

				//组合类型
				if (cbCardEye!=0)
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置组合
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i] );
					}

					//设置牌型
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//设置牌眼
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//插入结果
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=0;
				for ( i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//钻牌
bool CGameLogic::IsMagicCard( BYTE cbCardData )
{
	if( m_cbMagicIndex != MAX_INDEX )
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//花牌判断
bool CGameLogic::IsHuaCard( BYTE cbCardData )
{
	ASSERT( IsValidCard(cbCardData) );

	return cbCardData>=0x38?true:false;
}

//排序,根据牌值排序
bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
{
	//数目过虑
	if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

	//排序操作
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if (cbCardData[i]>cbCardData[i+1])
			{
				//设置标志
				bSorted=false;

				//扑克数据
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

/*
// 胡法分析函数
*/

//大对子
bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			return false;
	}
	return true;
}

//清一色牌
bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard)
{
	//胡牌判断
	BYTE cbCardColor=0xFF;

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i==m_cbMagicIndex) continue;
		if (cbCardIndex[i]!=0)
		{
			//花色判断
			if (cbCardColor!=0xFF)
				return false;

			//设置花色
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//设置索引
			i=(i/9+1)*9-1;
		}
	}

	//如果手上只有王霸
	if( cbCardColor == 0xFF )
	{
		ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
		//检查组合
		ASSERT( cbItemCount > 0 );
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

	//组合判断
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
	}

	return true;
}

//七小对牌//*
BYTE CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	//组合判断
	if (cbWeaveCount!=0) return 0;

	//单牌数目
	BYTE cbReplaceCount = 0;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//计算单牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//王牌过滤
		if( i == m_cbMagicIndex ) continue;

		//单牌统计
		if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
	}

	//王牌不够
	if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
		m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
		return 0;

	BYTE cbSame=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		//王牌过滤
		if( i == m_cbMagicIndex || cbCardIndexTemp[i]==0) continue;

		//四张统计
		if(cbCardIndexTemp[i]==4)cbSame++;
		if(cbCardIndexTemp[i]==3 && cbCardIndexTemp[m_cbMagicIndex]>0)
		{
			cbSame++;
			cbCardIndexTemp[m_cbMagicIndex]--;
		}
		if(cbCardIndexTemp[i]==1 && cbCardIndexTemp[m_cbMagicIndex]>0)
			cbCardIndexTemp[m_cbMagicIndex]--;
	}
	cbSame += cbCardIndexTemp[m_cbMagicIndex]/2;

	return (cbSame+1);
}

//将将胡
bool CGameLogic::IsJiangJiangHu(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	for (int i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind == WIK_LEFT 
			|| WeaveItem[i].cbWeaveKind == WIK_CENTER 
			|| WeaveItem[i].cbWeaveKind == WIK_RIGHT )
		{
			return false;
		}
		BYTE cbValue =(WeaveItem[i].cbCenterCard & MASK_VALUE);
		//单牌统计
		if( cbValue != 2 && cbValue != 5 && cbValue != 8 )
		{
			return false;
		}
	}
	
	//单牌数目
	BYTE cbReplaceCount = 0;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//计算单牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndexTemp[i] == 0)
		{
			continue;
		}
		BYTE cbValue =(SwitchToCardData(i) & MASK_VALUE);

		//单牌统计
		if( cbValue != 2 && cbValue != 5 && cbValue != 8 )
		{
			return false;
		}
	}
	return true;
}

bool CGameLogic::IsDanDiao(const BYTE cbCardIndex[MAX_INDEX],const BYTE cbCurrentCard)
{
	//单牌数目
	BYTE cbReplaceCount = 0;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//计算单牌
	int nTaltal = 0;
	bool bDuizi = false;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//王牌过滤
		if( i == m_cbMagicIndex ) continue;

		//单牌统计
		if( cbCardCount == 2) 	
		{
			bDuizi = true;
		}
		nTaltal += cbCardCount;
	}

	if (bDuizi && nTaltal == 2)
	{
		return true;
	}
	return false;
}

//带幺
bool CGameLogic::IsDaiYao( const tagAnalyseItem *pAnalyseItem )
{
	//检查牌眼
	BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
	if( cbCardValue != 1 && cbCardValue != 9 ) return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			BYTE j = 0;
			for(; j < 3; j++ )
			{
				cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
				if( cbCardValue == 1 || cbCardValue == 9 ) break;
			}
			if( j == 3 ) return false;
		}
		else
		{
			cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
			if( cbCardValue != 1 && cbCardValue != 9 ) return false;
		}
	}
	return true;
}

//将对
bool CGameLogic::IsJiangDui( const tagAnalyseItem *pAnalyseItem )
{
	//是否大对子
	if( !IsPengPeng(pAnalyseItem) ) return false;

	//检查牌眼
	BYTE cbCardValue = pAnalyseItem->cbCardEye&MASK_VALUE;
	if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			BYTE j = 0;
			for(; j < 3; j++ )
			{
				cbCardValue = pAnalyseItem->cbCardData[i][j]&MASK_VALUE;
				if( cbCardValue == 2 || cbCardValue == 5 || cbCardValue == 8 ) break;
			}
			if( j == 3 ) return false;
		}
		else
		{
			cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
			if( cbCardValue != 2 && cbCardValue != 5 && cbCardValue != 8 ) return false;
		}
	}
	return true;
}
bool CGameLogic::has4MagicCard(const BYTE cbCardIndex[MAX_INDEX])
{
	return getMagicCardCount(cbCardIndex) == 4;
}

BYTE CGameLogic::getMagicCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	return cbCardIndex[m_cbMagicIndex];
}

BYTE CGameLogic::GetHongZhongNum(const BYTE cbCardIndex[MAX_INDEX])
{
	return cbCardIndex[31];
}
//////////////////////////////////////////////////////////////////////////

CString CGameLogic::GetPaiString(BYTE bCard)
{
	CString csInfo;
	switch(bCard)
	{
	case 0x01:csInfo=TEXT("一筒");break;
	case 0x02:csInfo=TEXT("二筒");break;
	case 0x03:csInfo=TEXT("三筒");break;
	case 0x04:csInfo=TEXT("四筒");break;
	case 0x05:csInfo=TEXT("五筒");break;
	case 0x06:csInfo=TEXT("六筒");break;
	case 0x07:csInfo=TEXT("七筒");break;
	case 0x08:csInfo=TEXT("八筒");break;
	case 0x09:csInfo=TEXT("九筒");break;

	case 0x11:csInfo=TEXT("一万");break;
	case 0x12:csInfo=TEXT("二万");break;
	case 0x13:csInfo=TEXT("三万");break;
	case 0x14:csInfo=TEXT("四万");break;
	case 0x15:csInfo=TEXT("五万");break;
	case 0x16:csInfo=TEXT("六万");break;
	case 0x17:csInfo=TEXT("七万");break;
	case 0x18:csInfo=TEXT("八万");break;
	case 0x19:csInfo=TEXT("九万");break;

	case 0x21:csInfo=TEXT("一条");break;
	case 0x22:csInfo=TEXT("二条");break;
	case 0x23:csInfo=TEXT("三条");break;
	case 0x24:csInfo=TEXT("四条");break;
	case 0x25:csInfo=TEXT("五条");break;
	case 0x26:csInfo=TEXT("六条");break;
	case 0x27:csInfo=TEXT("七条");break;
	case 0x28:csInfo=TEXT("八条");break;
	case 0x29:csInfo=TEXT("九条");break;

	case 0x31:csInfo=TEXT("东风");break;
	case 0x32:csInfo=TEXT("南风");break;
	case 0x33:csInfo=TEXT("西风");break;
	case 0x34:csInfo=TEXT("北风");break;
	case 0x35:csInfo=TEXT("红中");break;
	case 0x36:csInfo=TEXT("发财");break;
	case 0x37:csInfo=TEXT("白板");break;

	//case 0x38:csInfo=TEXT("春夏秋冬");break;
	//case 0x39:csInfo=TEXT("春夏秋冬");break;
	//case 0x3a:csInfo=TEXT("春夏秋冬");break;
	//case 0x3b:csInfo=TEXT("春夏秋冬");break;
	//case 0x3c:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3d:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3e:csInfo=TEXT("梅兰菊竹");break;
	//case 0x3f:csInfo=TEXT("梅兰菊竹");break;

	}
	return csInfo;
}