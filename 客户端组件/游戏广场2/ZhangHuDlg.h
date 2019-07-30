#pragma once

#include "afxwin.h"
#include "TextButton.h"
#include "ZhangHuBaseInfoDlg.h"
#include "ZhangHuXgmmDlg.h"
#include "ZhangHuZhhbhDlg.h"
#include "ZhangHuShZhTkZhhDlg.h"
#include "ZhangHuTzhLogDlg.h"
#include "ZhangHuTxLogDlg.h"
#include "ZhangHuChzhLogDlg.h"
#include "ZhangHuYkLogDlg.h"
#include "ZhangHuYkMXDlg.h"
#include "ZhangHuQPYkDlg.h"
#define  IDM_RETURN_GAME 11118
#define IDM_RELEASE_FACE	11119
#include "MessageDlg.h"
//�ҵ��˻�
class CZhangHuDlg : public CDialog
{
	DECLARE_DYNAMIC(CZhangHuDlg)

	typedef enum {
		BASEINFO,
		MODIFYPWD,
		ZHANGHAOBAOHU,
		TIXIANZHANGHU,
		TOUZHUJILU,
		TIXIANJILU,
		CHONGZHIJILU,
		YINGKUIJILU,
		YINGKUIMINGXI,
		QIPAIYINGKUI
	}ShowType;

public:
	CZhangHuDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZhangHuDlg();

// �Ի�������
	enum { IDD = IDD_DLG_ZHANGHU };
	CImageButton m_btnClose;
	CPngImage m_ImgTitle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnClose();

	afx_msg void OnBnClickedBtnBaseinfo();
	afx_msg void OnBnClickedBtnAccountbaohu();
	afx_msg void OnBnClickedBtnTikuanact();
	afx_msg void OnBnClickedBtnTouzhujilu();
	afx_msg void OnBnClickedBtnTixianjilu();
	afx_msg void OnBnClickedBtnChongzhijilu();
	afx_msg void OnBnClickedBtnYingkuijilu();
	afx_msg void OnBnClickedBtnYingkuimingxi();
	afx_msg void OnBnClickedBtnQipaiYingkui();

	LRESULT OnReleaseFace(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnModifypwd();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
public:
	CMissionManager* m_MissionManager;

	void SetMissionManager(CMissionManager* pMissionManager)
	{
		m_MissionManager = pMissionManager;
	}

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

private:
	void InitBtns();
	void ResetDlg();

private:
	DWORD m_dwTickCount;
	Bitmap* m_bmpBk;
//	Bitmap* m_bmpTip;
	CFont m_font;
	CTextButton m_btnBaseInfo;		
	CTextButton m_btnModifyPwd;
	CTextButton m_btnZhangHaoBaoHu;
	CTextButton m_btnTiKuanZhangHao;
	CTextButton m_btnTouZhuJiLu;
	CTextButton m_btnTiXianJiLu;
	CTextButton m_btnChongZhiJiLu;
	CTextButton m_btnYingKuiJiLu;
	CTextButton m_btnYingKuiMingXi;		
	CTextButton m_btnQipaiYingkui;		

	ShowType m_showType;
public:
	CZhangHuBaseInfoDlg	m_baseInfoDlg;	//������Ϣ�Ӵ���
	CZhangHuXgmmDlg	m_xgmmDlg;	//�޸������Ӵ���
	CZhangHuZhhbhDlg m_zhhbhDlg;	//�˻������Ӵ���
	CZhangHuShZhTkZhhDlg m_shzhTkzhh;	//��������˻�
	CZhangHuTzhLogDlg m_tzhlogDlg;	//Ͷע��¼�Ӵ���
	CZhangHuTxLogDlg m_txlogDlg;	//���ּ�¼�Ӵ���
	CZhangHuChzhLogDlg m_chzhlogdlg;	//��ֵ��¼�Ӵ���
	CZhangHuYkLogDlg m_yklogdlg;	//��ֵ��¼�Ӵ���
	CZhangHuYkMXDlg m_ykmxdlg;	//ӯ���Ӵ���
	CZhangHuQPYkDlg m_qpykdlg;	//ӯ���Ӵ���
};
