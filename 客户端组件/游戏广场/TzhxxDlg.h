#pragma once
#include "afxwin.h"
#include "ImageButton.h"
#include "GameRule.h"
#include "PaiLie3.h"
#include "3D.h"
// CTzhxxDlg �Ի���

class CTzhxxDlg : public CDialog,public CMissionItem
{
	DECLARE_DYNAMIC(CTzhxxDlg)

public:
	CTzhxxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTzhxxDlg();
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CStatic						m_StaticKaijiang;
	CSkinButton					m_btnKaiJiang;
	// �Ի�������
	enum { IDD = IDD_DLG_TZHXX };

protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnBnClickedBtnClose();
	DECLARE_MESSAGE_MAP()
	CString GetDxdshStringDesc(int nNum);
	CString GetSZXDesc(int nNum);
	CString GetFunnyDesc(int nNum);
	CString GetJHODesc(int nNum);
	CString GetHZDXDSDesc(int nNum);

	CString		ChangeStringToT(CStringA strInput);
	CChqSSCRule m_chqRule;
	CJxSSCRule m_jxRule;
	CXJSSCRule m_xjRule;
	CFenFenCaiRule m_ffcRule;
	CErFenCaiRule m_efcRule;
	CWuFenCaiRule m_wfcRule;
	CHgydwfcRule	m_hgRule;
	CCQ11X5Rule m_chq11x5Rule;
	CGD11X5Rule m_gd11x5Rule;
	CJX11X5Rule m_jx11x5Rule;
	CSD11X5Rule m_sd11x5Rule;
	CHLJ11X5Rule m_hlj11x5Rule;
	CQiXingCaiRule m_qxcRule;
	CBJPK10Rule m_pk10Rule;
	CCanadaRule	m_jndRule;
	CKuaiLe8RUle	m_bj5fcRule;
	CTXFenFenCaiRule m_txffcRule;
	CQQFenFenCaiRule m_qqffcRule;

	CPaiLie3 m_dlgPaiLie3;
	C3D	m_dlg3D;
	int	GetZuxuanType(string strNumber);
	//��Ф��˳��ת��
	int	GetShengXiaoIndex(int nIndex, int nYearXiuZheng);

	bool	ChangeStringToA(CString strInput, string&  strOutput);
	CString GetNiuString(CString strNiu);
protected:
	virtual BOOL OnInitDialog();

public:
	int m_id;
	bool b_hideName;

	CString m_wanjiazhanghao;
	int m_dingdanhao;
	CString m_qihao;
	int m_gametype;
	int m_gamekind;
	CString m_touzhuhaoma;
	CString m_kaijianghaoma;
	CString m_xiadanshijian;
	int m_state;

	int m_touzhushuliang;
	double m_danzhujine;
	int m_beishu;
	double m_touzhuzonge;
	int m_zhongjiangzhushu;
	double m_danzhujiangjin;
	double m_zhongjiangjine;
	double m_fandian;
	double m_fandianjine;
	double m_yinhkui;

private:
	Bitmap* m_bmpBk;
	CFont m_font;

	CEdit m_editHaoma;
	CRichEditCtrl m_RichEditHaoma;
	CImageButton m_btnClose;

	CMissionManager					m_MissionManager;					//�������
	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bUserQianDao;						//��ȡ��־����
	bool							m_bGetUserHuanlesongInfo;						//��ȡ��־����
	bool							m_bUserHuanlesongLingjiang;						//��ȡ��־����ͨ��ID
public:
	VOID SendToServer(int nSendType);
};
