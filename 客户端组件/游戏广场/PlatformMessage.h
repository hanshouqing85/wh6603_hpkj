#pragma once

#include "afxwin.h"
#include "ImageButton.h"

#include "PlatformInbox.h"
#include "PlatformOutbox.h"
#include "PlatformSendMessage.h"
#define  IDM_RETURN_GAME 11118
//»î¶¯×¨Çø
class CPlatformMessage : public CDialog
{
	DECLARE_DYNAMIC(CPlatformMessage)

public:
	CPlatformMessage(CWnd* pParent = NULL);   // ±ê×¼¹¹Ốº¯Êư
	virtual ~CPlatformMessage();

// ¶Ô»°¿̣Êư¾Ư
	enum { IDD = IDD_DLG_PLATFORM_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnInbox();
	afx_msg void OnBnClickedBtnOutbox();
	afx_msg void OnBnClickedBtnSendMessage();

	LRESULT OnMessageReply(WPARAM wParam, LPARAM lParam);
	LRESULT OnGoOutbox(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

private:
	void AdjustCtrls();

private:
	Bitmap*					m_bmpBk;
	//Bitmap*					m_bmpTip;
//	CImageButton			m_btnDalibao;		//´óÀñ°ü°´Å¥
//	CImageButton			m_btnZaJinDan;		//Ổ½đµ°°´Å¥
// 	CImageButton			m_btnZhuanZhuan;	//Đ̉ÔË×ª×ª


	CImageButton			m_btnInbox;				//�ռ���
	CImageButton			m_btnOutbox;			//������
	CImageButton			m_btnSendMessage;	//����Ϣ
public:
// 	CHuoDongDaliBaoDlg		m_dalibao;		//´óÀñ°ü
// 	CHuoDongZaJinDanDlg		m_zajindan;		//Ổ½đµ°
// 	CHuoDongHuanLeSongDlg	m_huanlesong;	//»¶ÀÖË̀
// 	HuodongZhuanzhuan		m_zhuanzhuan;	//Đ̉ÔË×ª×ª
// 	CHuoDongDaili			m_daili;		//´úÀí
	CImageButton m_btnClose;

	CPlatformInbox			m_dlgInbox;
	CPlatformOutbox			m_dlgOutbox;
	CPlatformSendMessage	m_dlgSendMessage;
};
