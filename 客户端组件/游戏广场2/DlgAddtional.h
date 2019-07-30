#pragma once
#include	"math.h"
#include	"GridCtrl/GridCtrl.h"
#include	"GridCtrl/GridCellCheck.h"
#include "ImageButton.h"
#include "MyCheckBox.h"
#include	<vector>
#include "MessageDlg.h"
using namespace std;

#define			FONT_BG_COLOR		RGB(255,255,255)



// CDlgAddtional �Ի���

class CDlgAddtional : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddtional)

protected:
	CGridCtrl	m_Grid;
	Bitmap*		m_bmpBk;
	CImageButton m_btnOK;
	CImageButton m_btnCancel;
// 	CMyCheckBox	m_btCheckBeiTou;
// 	CMyCheckBox m_btZhongj;
	int         m_nFirst;
	int         m_nLast;
	CString		m_strQiHao;
	CString		m_strTzhJine;
	CString		m_strTime;
	bool		m_bBeiTou;					//��Ͷ
	bool		m_bAutoStop;				//�н����Զ�ֹͣ׷��
	DOUBLE		m_fJine;					//��ע���
	DOUBLE      m_ZongJine;
	int			m_nMostQishu;
	int			m_nGameType;

	CFont		m_font;
public:
	void GridCtrlInit();
public:
	CDlgAddtional(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddtional();
	virtual BOOL OnInitDialog();
	CString GetNextQiHao();
	CString GetNextTime(int nPlus);
	CString GetTzhJine(int nRow);

	void	SetCurrentQihaoTime(CString strQihao, CString strTime,int nFirst=8,int nLast=3,DOUBLE fJine = 2.0,int nMostQishu=120,int nTypeID=CZChongQingSSC);
	//׷������
	int		GetZhuiHaoQiShu();
	//׷�ڱ���
	int		GetZhuiQiBeiShu();
	//�Ƿ�Ͷ
	bool	GetBeiTou();
	//�н����Զ�ֹͣ׷��
	bool	GetPrizeStop();
	//�ںŵ�����
	int		GetZhuiHaoData(vector<wstring>&	vecData1, vector<wstring>& vecData2, vector<wstring>& vecData3);
// �Ի�������
	enum { IDD = IDD_DLG_ADDTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridEndSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
public:
	afx_msg void OnBnClickedCheck1();
public:
	afx_msg void OnBnClickedCheck2();
};
