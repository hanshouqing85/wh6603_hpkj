#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "comm.h"
#include "NumerButton.h"
#include "TextButton.h"
#include "ImageButton.h"
#include "ImgRioButton.h"
#include "ImgStatic.h"
#include "GameRule.h"
#include "resource.h"
// #include "GamePlaza.h"
 #include "QrTzhDlg.h"
#include "FanDian.h"
#include	"GridCtrl/GridCtrl.h"
#include "DlgStatus.h"
class CPlazaViewItem;
#include "..\..\�������\�������\ServiceCoreHead.h"

#define			AREA_TMDM_MAX				 50
#define			AREA_TMDS_MAX				 3
#define			AREA_TMDX_MAX				 3
#define			AREA_TMSX_MAX				 13
#define			AREA_TMBS_MAX				 4

//ѡȡ������Ϣ
struct tagFlashInfo
{
	BYTE							cbFlashArea;					//��ʾ����
	int								nFlashAreaAlpha;				//��ʾ͸����
	bool							bFlashAreaAlpha;				//�Ƿ���ʾ
};

// ����ʱʱ�� �Ի���
class CLiuHeCai : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CLiuHeCai)

public:
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();

// 	//ʱʱ���淨����
	typedef enum {
		LiuHeCai_Tmdm=1,
		LiuHeCai_Tmds,
		LiuHeCai_Tmdx,
		LiuHeCai_Tmsx,
		LiuHeCai_Tmbs
	}LiuHeCaiKind;

	enum {
		Kj_XinXi_Count = 5
	};

public:
	CLiuHeCai(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLiuHeCai();

	HBRUSH hBrush;

// �Ի�������
	enum { IDD = IDD_DLG_LIUHECAI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	LiuHeCaiKind m_gameKind;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedBtnTmdm();
	afx_msg void OnBnClickedBtnTmds();
	afx_msg void OnBnClickedBtnTmdx();
	afx_msg void OnBnClickedBtnTmsx();
	afx_msg void OnBnClickedBtnTmbs();

	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn5();
	afx_msg void OnBnClickedBtn10();
	afx_msg void OnBnClickedBtn50();
	afx_msg void OnBnClickedBtn100();
	afx_msg void OnBnClickedBtn1000();

	afx_msg void OnBnClickedJettonOK();
	afx_msg void OnBnClickedCancelJetton();
	afx_msg void OnBnClickedFandian();
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()

	CString GetWebData();
	void DrawNum2(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawTmdm(CDC* pDC, Graphics& graphics);
	void DrawTmds(CDC* pDC, Graphics& graphics);
	void DrawTmdx(CDC* pDC, Graphics& graphics);
	void DrawTmsx(CDC* pDC, Graphics& graphics);
	void DrawTmbs(CDC* pDC, Graphics& graphics);
	void EnableJetton(DOUBLE fyue);
	BYTE GetBetArea(CPoint MousePoint);
	void DrawFlashFrame(CDC *pDC, int nWidth, int nHeight);

	VOID OnMouseMove(UINT nFlags, CPoint Point);
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	void InitWanFaMoShi();
	void SetLockText(float fBonus, float fBonusPercent);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, int lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, CString szOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat = DT_LEFT);

	VOID DrawJetton(CDC *pDC, int nWidth, int nHeight);
	VOID DrawLuckNum(CDC *pDC, int nWidth, int nHeight);
	void ResetJetton();
	void SendToServer(int nSendType);
public:
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);
	int GetBoSe(int nNum);
	bool ChangeStringToA(CString strInput, string&  strOutput);

	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//λ����Ϣ
protected:
	CWHArray<CRect>					m_ArrayTmdmBetArea[AREA_TMDM_MAX];			//������������
	CWHArray<CRect>					m_ArrayTmdsBetArea[AREA_TMDS_MAX];			//���뵥˫����
	CWHArray<CRect>					m_ArrayTmdxBetArea[AREA_TMDX_MAX];			//�����С����
	CWHArray<CRect>					m_ArrayTmsxBetArea[AREA_TMSX_MAX];			//������Ф����
	CWHArray<CRect>					m_ArrayTmbsBetArea[AREA_TMBS_MAX];			//������Ф����

	int		m_nMoshi;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];
	bool	m_bKaiJiangzhong;
	int	m_kjXinxiCont;
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
public:
	void FlushZongjine();
	Bitmap* m_bmpBk;
	CPngImage m_Num2;
	Bitmap* m_bmpTmdm;
	Bitmap* m_bmpTmds;
	Bitmap* m_bmpTmdx;
	Bitmap* m_bmpTmsx;
	Bitmap* m_bmpTmbs;
	float						m_fBonus;						//���õ�
	float						m_fPercent;						//���õ�

	int m_Num2Width;
	int m_Num2Height;
	int							m_nTzhSign;
	int							m_nTzhSign1;
	bool						m_bShowFail;
	CPngImage						m_ImgNum;		//����
	CTextButton						 m_btnTmdm;		//���뵥��
	CTextButton						 m_btnTmds;		//���뵥˫
	CTextButton						 m_btnTmdx;		//�����С
	CTextButton						 m_btnTmsx;		//������Ф
	CTextButton						 m_btnTmbs;		//���벨ɫ

	CSkinButton						m_btJetton1;					//���밴ť
	CSkinButton						m_btJetton5;					//���밴ť
	CSkinButton						m_btJetton10;					//���밴ť
	CSkinButton						m_btJetton50;					//���밴ť
	CSkinButton						m_btJetton100;					//���밴ť
	CSkinButton						m_btJetton1000;					//���밴ť

	CSkinButton						m_btnJetton;					//ȷ����ע
	CSkinButton						m_btnCancelJetton;					//ȡ����ע
//	CSkinButton						m_btnFandian;					//����
	CTextButton						m_btnFandian;					//����

	SCORE							m_fCurrentBet;		//��ǰ����

	CPngImage						m_ImageTmdmFrame;
	CPngImage						m_ImageTmdsFrame;
	CPngImage						m_ImageTmsxFrame;
	CPngImage						m_ImageTmbsFrame;
	CPngImage						m_ImageLuckNum;
	CPngImage						m_ImageLuckBall;

	int								m_nFrameX;
	int								m_nFrameY;
	BYTE							m_cbCurrentArea;
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;

	BYTE							m_cbOldArea;
private:
	void InitBtns();

};
