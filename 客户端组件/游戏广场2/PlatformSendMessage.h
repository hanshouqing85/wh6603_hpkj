#pragma once

#include "ImageButton.h"
#include "SelectRevAccount.h"
#include <map>
// CPlatformSendMessage �Ի���
#define IDM_GO_OUTBOX 310
//������
class CPlatformSendMessage : public CDialog
{
	DECLARE_DYNAMIC(CPlatformSendMessage)
public:
	CPlatformSendMessage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlatformSendMessage();

// �Ի�������
	enum { IDD = IDD_DLG_SEND_MESSAGE };

protected:
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnEnChangeEditTitle();
	DECLARE_MESSAGE_MAP()

protected:
	double		m_dJinriXiafei;			//��������
	double		m_dWinPrize;			//�н����		
	int			m_nSignedCount;			//ǩ������
	int			m_XXCount;				//ʣ��ǩ������
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

	CImageButton			m_btnShangji;			//�ϼ�
	CImageButton			m_btnXiaji;				//�¼�

	CImageButton			m_btnKefu;				//�¼�
	afx_msg void OnBnClickedBtnSj();
	afx_msg void OnBnClickedBtnXj();
	afx_msg void OnBnClickedBtnKeFu();
	afx_msg void OnBnClickedBtnSendMessage();

	map<int ,CString>mapRevUserID;
	map<int ,int >mapIndex;
	int	m_nPage;
	int	m_nMaxPage;
	int   m_nSearchType;
	//���غ���
public:
	void SearchUser();
	LRESULT OnTurnPage(WPARAM wParam, LPARAM lParam);
	void SetSendUser(CString strName,int nUserID,CString strTitle);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	void SelectUser(int nUser,int nSel);
protected:
	bool							m_bUserQianDao;						//��ȡ��־����
	bool							m_bGetUserHuanlesongInfo;						//��ȡ��־����
	bool							m_bUserHuanlesongLingjiang;						//��ȡ��־����ͨ��ID
	int								m_nSign;
public:
	VOID SendToServer(int nSendType);
};
