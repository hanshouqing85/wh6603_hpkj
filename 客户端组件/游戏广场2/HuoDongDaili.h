#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CHuoDongDaliBao �Ի���
//�����
class CHuoDongDaili : public CDialog
{
	DECLARE_DYNAMIC(CHuoDongDaili)

public:
	CHuoDongDaili(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuoDongDaili();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG_DAILI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedLingJiang();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	void AdjustLingjiang();
	CImageButton m_btnQianDao[9];
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	int   m_nToday;
	//���غ���
public:
	//��ȡ�¼�
	 bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetDaLiBao;						//��ȡ��־����
	bool							m_bGetLastYue;						//��ȡ��־����
	bool							m_bCanDaLiBao;						//��ȡ��־����ͨ��ID
	bool							m_bGetXjYkTjByAct;						//��ȡ��־����ͨ���˻�
	BYTE							m_cbNewRegUser;						//��ע���û�
	DOUBLE							m_fTouZhuJine;						//Ͷע���
	DOUBLE							m_fChongZhiJine;					//��ֵ���
public:
	VOID SendToServer(int nSendType);
};
