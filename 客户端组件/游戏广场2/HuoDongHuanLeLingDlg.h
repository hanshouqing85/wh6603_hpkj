#pragma once

#include "ImageButton.h"

// CHuoDongHuanLeSongDlg �Ի���
//������
class CHuoDongHuanLeSongDlg : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongHuanLeSongDlg)
public:
	void	GetUserHuanlesongInfo();
public:
	CHuoDongHuanLeSongDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuoDongHuanLeSongDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG_HUANLELING };

	void AdjustLingjiang();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	double		m_dJinriXiafei;			//��������
	double		m_dWinPrize;			//�н����		
	int			m_nSignedCount;			//ǩ������
	int			m_XXCount;				//ʣ��ǩ������

private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CImageButton m_btnQiandao;
	CImageButton m_btnLingJiang;

	CImageButton m_btnQiandaoDay[7];

	afx_msg void OnBnClickedBtnQiandao();
	afx_msg void OnBnClickedBtnLingjiang();

	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bUserQianDao;						//��ȡ��־����
	bool							m_bGetUserHuanlesongInfo;						//��ȡ��־����
	bool							m_bUserHuanlesongLingjiang;						//��ȡ��־����ͨ��ID
public:
	VOID SendToServer(int nSendType);
};
