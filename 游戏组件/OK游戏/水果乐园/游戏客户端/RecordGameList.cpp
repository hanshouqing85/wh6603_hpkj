#include "StdAfx.h"
#include "RecordGameList.h"

//���캯��
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

//��������
CGameRecord::~CGameRecord()
{
}

//�����б�
void CGameRecord::FillGameRecord(tagGameRecord * pGameRecord, int nRecordNum)
{
	for( int i=0; i< nRecordNum; i++ )
	{
		bool bRepeat = false;
		for(int j=0; j<m_nRecordCount; j++ )
		{
			//��������ظ���¼��Ϣ���������������һ���¼�¼
			if( m_RecordArray[j].dwGameTimes == pGameRecord[i].dwGameTimes )
			{
				bRepeat = true;
				break;
			}
		}
		if( !bRepeat )	//������ظ�����ѵ�ǰ�¼�¼����
		{
			m_nWritePos = (m_nWritePos)%MAX_SCORE_HISTORY;
			tagGameRecord & newRecord = m_RecordArray[m_nWritePos];
			newRecord.dwGameTimes = pGameRecord[i].dwGameTimes;
			newRecord.cbGameRecord = pGameRecord[i].cbGameRecord;
			newRecord.bGoodLuck=pGameRecord[i].bGoodLuck;
			if( m_nRecordCount < RECORD_COUNT_LIMIT)
				m_nRecordCount++;
			//if (m_nRecordCount<MAX_SCORE_HISTORY)//�˴����m_nRecordCount���ڵ���1000����m_nReadPos��m_nWritePos����ͬ��
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
	int k = nShowCount-1;//��֤���Ƶĵ���
	int nCurIndex = 0;

	for( int i=0; i<nShowCount; i++)
	{
		//ȡ�õ�ǰ��ʷ��¼��ͼ��
		nCurIndex = (m_nReadPos - i + MAX_SCORE_HISTORY)%MAX_SCORE_HISTORY;//�����ȡ��¼��Ϣ
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
			pRectArray[k].left,pRectArray[k].top,//Ŀ������
			nRecordWidth,skinImage.GetHeight(),//Ŀ�Ŀ��
			cbRecord*nRecordWidth,0);//Դ����
		//////////
		//��ǰʤ������
		if (nCurIndex==m_nWritePos-1)
		{
			Tip.DrawImage(pDC,
				pRectArray[k].left,pRectArray[k].top,
				39,39,
				0,0);
		}
		//LUCKY��ʶ
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
		//��ʾ�Ѿ��ƶ�������棬�����µ�һ����¼
		return -1;
	}
}
