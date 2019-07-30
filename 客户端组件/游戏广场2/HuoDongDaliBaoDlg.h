#pragma once
#include "afxwin.h"
#include "ImageButton.h"

// CHuoDongDaliBao �Ի���
//�����
class CHuoDongDaliBaoDlg : public CDialog,public CMissionItem
{
	DECLARE_DYNAMIC(CHuoDongDaliBaoDlg)

public:
	CHuoDongDaliBaoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuoDongDaliBaoDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG_DALIBAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedLingJiang();
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();


private:
	Bitmap* m_bmpBk;
	CFont m_font;
	CImageButton m_btnLingJiang;

	double m_jinrixiaofei;
	int m_jine;

	//CMissionManager					m_MissionManager;					//�������
	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetDaLiBao;						//��ȡ��־����
	bool							m_bGetLastYue;						//��ȡ��־����
	bool							m_bCanDaLiBao;						//��ȡ��־����ͨ��ID
	bool							m_bGetXjYkTjByAct;						//��ȡ��־����ͨ���˻�
public:
	VOID SendToServer(int nSendType);
};
