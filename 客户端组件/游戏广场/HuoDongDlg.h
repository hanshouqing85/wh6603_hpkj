#pragma once

#include "afxwin.h"
#include "ImageButton.h"
#include "HuoDongHuanLeLingDlg.h"
#include "HuoDongZaJinDanDlg.h"
#include "HuoDongDaliBaoDlg.h"
#include "HuodongZhuanzhuan.h"
#include "HuoDongDaili.h"
#define  IDM_RETURN_GAME 11118


//»î¶¯×¨Çø
class CHuoDongDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongDlg)

public:
	CHuoDongDlg(CWnd* pParent = NULL);   // ±ê×¼¹¹Ốº¯Êư
	virtual ~CHuoDongDlg();

// ¶Ô»°¿̣Êư¾Ư
	enum { IDD = IDD_DLG_HUODONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnDalibao();
	afx_msg void OnBnClickedBtnZajindan();
	afx_msg void OnBnClickedBtnHuanlesong();
	afx_msg void OnBnClickedBtnZhuanzhuan();
	afx_msg void OnBnClickedBtnDaili();
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
	CPngImage				m_ImgTitle;

private:
	void					AdjustCtrls();

private:
	Bitmap*					m_bmpBk;
	//Bitmap*					m_bmpTip;
	CImageButton			m_btnDalibao;		//´óÀñ°ü°´Å¥
	CImageButton			m_btnZaJinDan;		//Ổ½đµ°°´Å¥
	CImageButton			m_btnHuanlesong;	//»¶ÀÖË̀
	CImageButton			m_btnZhuanZhuan;	//Đ̉ÔË×ª×ª
	CImageButton			m_btnDaili;			//´úÀí»ØÀ¡
public:
	CHuoDongDaliBaoDlg		m_dalibao;		//´óÀñ°ü
	CHuoDongZaJinDanDlg		m_zajindan;		//Ổ½đµ°
	CHuoDongHuanLeSongDlg	m_huanlesong;	//»¶ÀÖË̀
	HuodongZhuanzhuan		m_zhuanzhuan;	//Đ̉ÔË×ª×ª
	CHuoDongDaili			m_daili;		//´úÀí
	CImageButton			m_btnClose;
};
