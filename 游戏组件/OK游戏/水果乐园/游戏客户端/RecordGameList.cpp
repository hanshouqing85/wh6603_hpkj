#include "StdAfx.h"
#include "RecordGameList.h"

//构造函数
CGameRecord::CGameRecord()
{
	for( int i=0; i<MAX_SCORE_HISTORY; i++ )
	{
		m_RecordArray[i].cbGameRecord = 0xFF;
		m_RecordArray[i].dwGameTimes = -1;
	}
	m_nRecordCount = 0;
	m_nReadPos = 0;
	m_nWritePos = 0;
}

//析构函数
CGameRecord::~CGameRecord()
{
}

//插入列表
void CGameRecord::FillGameRecord(tagGameRecord * pGameRecord, int nRecordNum)
{
	for( int i=0; i< nRecordNum; i++ )
	{
		bool bRepeat = false;
		for(int j=0; j<m_nRecordCount; j++ )
		{
			//如果存在重复记录信息，则跳过，检查下一个新记录
			if( m_RecordArray[j].dwGameTimes == pGameRecord[i].dwGameTimes )
			{
				bRepeat = true;
				break;
			}
		}
		if( !bRepeat )	//如果不重复，则把当前新记录插入
		{
			m_nWritePos = (m_nWritePos)%MAX_SCORE_HISTORY;
			tagGameRecord & newRecord = m_RecordArray[m_nWritePos];
			newRecord.dwGameTimes = pGameRecord[i].dwGameTimes;
			newRecord.cbGameRecord = pGameRecord[i].cbGameRecord;
			newRecord.bGoodLuck=pGameRecord[i].bGoodLuck;
			if( m_nRecordCount < RECORD_COUNT_LIMIT)
				m_nRecordCount++;
			//if (m_nRecordCount<MAX_SCORE_HISTORY)//此处如果m_nRecordCount大于等于1000，则m_nReadPos与m_nWritePos将不同步
			{
				m_nReadPos = m_nWritePos;
			}
			m_nWritePos++;
		}
	}
	return;
}

void CGameRecord::ShowHistory(CDC * pDC, CBitImage &skinImage, CPngImage &Tip, CRect * pRectArray, int nRectCount)
{
	if( pDC == NULL || pRectArray == NULL || nRectCount<=0 )
		return;

	BYTE cbRecord = 0;
	int nRecordWidth = skinImage.GetWidth()/18;
	int nShowCount = (m_nRecordCount >= MAX_SHOW_HISTORY ? MAX_SHOW_HISTORY:m_nRecordCount);
	int k = nShowCount-1;//保证绘制的倒序
	int nCurIndex = 0;

	for( int i=0; i<nShowCount; i++)
	{
		//取得当前历史记录的图案
		nCurIndex = (m_nReadPos - i + MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;//倒序获取记录信息
		cbRecord = m_RecordArray[nCurIndex].cbGameRecord;
		switch( cbRecord )
		{
		case 0:
		case 12:
			{
				cbRecord = 14;
				break;
			}
		case 1:
		case 13:
			{
				cbRecord = 12;
				break;
			}
		case 2:
			{
				cbRecord = 0;
				break;
			}
		case 3:
			{
				cbRecord = 8;
				break;
			}
		case 4:
		case 10:
		case 16:
		case 22:
			{
				cbRecord = 15;
				break;
			}
		case 5:
			{
				cbRecord = 7;
				break;
			}
		case 6:
		case 18:
			{
				cbRecord = 13;
				break;
			}
		case 7:
			{
				cbRecord = 11;
				break;
			}
		case 8:
			{
				cbRecord = 3;
				break;
			}
		case 9:
			{
				cbRecord = 17;
				break;
			}
		case 11:
			{
				cbRecord = 6;
				break;
			}
		case 14:
			{
				cbRecord = 1;
				break;
			}
		case 15:
			{
				cbRecord = 9;
				break;
			}
		case 17:
			{
				cbRecord = 5;
				break;
			}
		case 19:
			{
				cbRecord = 10;
				break;
			}
		case 20:
			{
				cbRecord = 2;
				break;
			}
		case 21:
			{
				cbRecord = 16;
				break;
			}
		case 23:
			{
				cbRecord = 4;
				break;
			}
		}
		//cbRecord = cbRecord % MAX_SHOW_HISTORY;
		skinImage.BitBlt(pDC->GetSafeHdc(),
			pRectArray[k].left,pRectArray[k].top,//目的坐标
			nRecordWidth,skinImage.GetHeight(),//目的宽高
			cbRecord*nRecordWidth,0);//源坐标
		//////////
		//当前胜利区域
		if (nCurIndex==m_nWritePos-1)
		{
			Tip.DrawImage(pDC,
				pRectArray[k].left,pRectArray[k].top,
				39,39,
				0,0);
		}
		//LUCKY标识
		if (m_RecordArray[nCurIndex].bGoodLuck)
		{
			Tip.DrawImage(pDC,
				pRectArray[k].left,pRectArray[k].top,
				39,39,
				39,0);
		}
		//////////
		k--;
	}
}

void CGameRecord::ClearAllRecord()
{
	for( int i=0; i<MAX_SCORE_HISTORY; i++ )
	{
		m_RecordArray[i].cbGameRecord = 0xFF;
		m_RecordArray[i].dwGameTimes = -1;
	}
	m_nRecordCount = 0;
	m_nReadPos = 0;
}

int CGameRecord::ScrollLeft()
{
	int nTemp = 0;
	if(m_nRecordCount <= MAX_SCORE_HISTORY )
	{
		nTemp = m_nReadPos-MAX_SHOW_HISTORY;
		if( nTemp >=0 )
		{
			m_nReadPos -= 1;
			return m_nReadPos;
		}
		else
			return -1;
	}
	else
	{
		nTemp = (m_nReadPos-MAX_SHOW_HISTORY+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;
		if(nTemp != (m_nWritePos-1)%MAX_SCORE_HISTORY)
		{
			m_nReadPos = (m_nReadPos-1+MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;
			return m_nReadPos;
		}
		else
		{
			return -1;
		}
	}
}

int CGameRecord::ScrollRight()
{
	if( m_nReadPos != m_nWritePos-1 )
	{
		m_nReadPos = (m_nReadPos+1)%MAX_SCORE_HISTORY;
		return m_nReadPos;
	}
	else
	{
		//表示已经移动到最后面，即最新的一个记录
		return -1;
	}
}
