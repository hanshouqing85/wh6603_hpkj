#pragma once
#include "Stdafx.h"

class CAdminClientDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdminClientDlg)

protected:		
	LONGLONG				m_lAllUserBet[JETTON_AREA_COUNT];				//用户下注

public:
	CAdminClientDlg();   // 标准构造函数
	virtual ~CAdminClientDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	//更新控制
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns);
	//重置下注
	void ResetUserBet();
	//玩家下注
	void SetUserBetScore(BYTE cbArea,LONGLONG lScore);

protected:
	//信息解析
	void PrintingInfo(TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes);

public:
	//初始化
	virtual BOOL OnInitDialog();
	//设置颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//取消控制
	afx_msg void OnBnClickedButtonReset();
	//本局信息
	afx_msg void OnBnClickedButtonSyn();
	//开启控制
	afx_msg void OnBnClickedButtonOk();
	//取消关闭
	afx_msg void OnBnClickedButtonCancel();

	DECLARE_MESSAGE_MAP()
};
