#ifndef PLAYER_LIST_HEAD_FILE
#define PLAYER_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "MySkinScrollBar.h"

//玩家信息
struct tagPlayerRecord
{
	DWORD							dwUserID;							//玩家ID号
	__int64							lUserScore;							//玩家金币
	TCHAR							szUserName[NAME_LEN];				//玩家帐号
};

//游戏记录
class CPlayerList : public CListCtrl
{
private:
	tagPlayerRecord					m_PlayerRecordArray[GAME_PLAYER];	//列表中记录的数据
	int								m_iRecordCount;						//当前列表中的人数
	CMySkinScrollBar				m_ctrlScroll;
	HBITMAP							m_hBmpScrollBar;
	//函数定义
public:
	//构造函数
	CPlayerList();
	//析构函数
	virtual ~CPlayerList();

	//功能函数
public:
	//插入
	void InserUser(tagPlayerRecord & Record);
	//删除
	void DeleteUser(DWORD dwUserID);
	//更新
	void UpdateUser(tagPlayerRecord & Record);
	//查找
	int FindUser(tagPlayerRecord & Record);
	//清空
	void ClearAll();
	//格式化
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
