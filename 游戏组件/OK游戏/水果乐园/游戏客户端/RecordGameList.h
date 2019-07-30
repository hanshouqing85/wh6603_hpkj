#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"

//��Ϸ��¼
struct tagGameRecord
{
	DWORD dwGameTimes;//�ڼ���
	BYTE cbGameRecord;//���ֽ��
	bool bGoodLuck;
};

//��Ϸ��¼
class CGameRecord
{
public:
	CGameRecord();
	virtual ~CGameRecord();
private:
	int		m_nRecordCount;		//��¼����
	int		m_nReadPos;			//��ǰ����λ��
	int		m_nWritePos;		//��ǰд��λ��
	tagGameRecord  m_RecordArray[MAX_SCORE_HISTORY];
	enum{RECORD_COUNT_LIMIT = 2100000000};

public:
	void FillGameRecord(tagGameRecord * pGameRecord, int nRecordNum);
	int GetRecordCount(){return m_nRecordCount;}
	int GetCurReadPos(){return m_nReadPos;}
	int GetCurWritePos(){return m_nWritePos;}
	void ClearAllRecord();
	int	ScrollLeft();
	int ScrollRight();
	void ShowHistory(CDC * pDC, CBitImage &skinImage, CPngImage &Tip1, CRect * pRectArray, int nRectCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
