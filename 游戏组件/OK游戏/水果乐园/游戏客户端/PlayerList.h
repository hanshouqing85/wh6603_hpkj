#ifndef PLAYER_LIST_HEAD_FILE
#define PLAYER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MySkinScrollBar.h"

//�����Ϣ
struct tagPlayerRecord
{
	DWORD							dwUserID;							//���ID��
	__int64							lUserScore;							//��ҽ��
	TCHAR							szUserName[NAME_LEN];				//����ʺ�
};

//��Ϸ��¼
class CPlayerList : public CListCtrl
{
private:
	tagPlayerRecord					m_PlayerRecordArray[GAME_PLAYER];	//�б��м�¼������
	int								m_iRecordCount;						//��ǰ�б��е�����
	CMySkinScrollBar				m_ctrlScroll;
	HBITMAP							m_hBmpScrollBar;
	//��������
public:
	//���캯��
	CPlayerList();
	//��������
	virtual ~CPlayerList();

	//���ܺ���
public:
	//����
	void InserUser(tagPlayerRecord & Record);
	//ɾ��
	void DeleteUser(DWORD dwUserID);
	//����
	void UpdateUser(tagPlayerRecord & Record);
	//����
	int FindUser(tagPlayerRecord & Record);
	//���
	void ClearAll();
	//��ʽ��
	CString FormatNumber(__int64 lScore);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
