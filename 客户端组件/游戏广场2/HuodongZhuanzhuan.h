#pragma once
#include "ImageButton.h"
#include "PlatformPublicize.h"
// HuodongZhuanzhuan �Ի���
//����תת
class HuodongZhuanzhuan : public CDialog
{
	DECLARE_DYNAMIC(HuodongZhuanzhuan)

public:
	HuodongZhuanzhuan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~HuodongZhuanzhuan();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG_ZHUANZHUAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedBtChouJiang();
private:
	Bitmap* m_bmpBk;
	CFont m_font;
	bool			m_bPrized;
	CImageButton	m_btnChouJiang;
	CPlatformPublicize  m_PublicizeWeb;
	//���غ��� 
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetUserLuckyZhuan;						//��ȡ��־����
	bool							m_bGetLastYue;						//��ȡ��־����
	bool							m_bCanUserLuckyzhuanZhuan;						//��ȡ��־����ͨ��ID
public:
	VOID SendToServer(int nSendType);
};
