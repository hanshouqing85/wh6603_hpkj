#pragma once

#include "ImageButton.h"
#include "SelectRevAccount.h"
#include <map>
// CPlatformSendMessage 对话框
#define IDM_GO_OUTBOX 310
//欢乐颂
class CPlatformSendMessage : public CDialog
{
	DECLARE_DYNAMIC(CPlatformSendMessage)
public:
	CPlatformSendMessage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlatformSendMessage();

// 对话框数据
	enum { IDD = IDD_DLG_SEND_MESSAGE };

protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnEnChangeEditTitle();
	DECLARE_MESSAGE_MAP()

protected:
	double		m_dJinriXiafei;			//今日消费
	double		m_dWinPrize;			//中奖金额		
	int			m_nSignedCount;			//签到次数
	int			m_XXCount;				//剩余签到次数
	int				m_cbSendType;
	CString  m_strRevName;
	int				m_nRevUserID;
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CSkinEditEx	m_edRevAct;
	CSkinEditEx	m_edRevTitle;
	CRichEditCtrl				m_richContent;
	CImageButton		m_btnSendMessage;
	CSelectRevAccount m_dlgSelectRevAccount;

	CImageButton			m_btnShangji;			//上级
	CImageButton			m_btnXiaji;				//下级

	CImageButton			m_btnKefu;				//下级
	afx_msg void OnBnClickedBtnSj();
	afx_msg void OnBnClickedBtnXj();
	afx_msg void OnBnClickedBtnKeFu();
	afx_msg void OnBnClickedBtnSendMessage();

	map<int ,CString>mapRevUserID;
	map<int ,int >mapIndex;
	int	m_nPage;
	int	m_nMaxPage;
	int   m_nSearchType;
	//重载函数
public:
	void SearchUser();
	LRESULT OnTurnPage(WPARAM wParam, LPARAM lParam);
	void SetSendUser(CString strName,int nUserID,CString strTitle);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	void SelectUser(int nUser,int nSel);
protected:
	bool							m_bUserQianDao;						//获取日志数量
	bool							m_bGetUserHuanlesongInfo;						//获取日志数量
	bool							m_bUserHuanlesongLingjiang;						//获取日志数量通过ID
	int								m_nSign;
public:
	VOID SendToServer(int nSendType);
};
