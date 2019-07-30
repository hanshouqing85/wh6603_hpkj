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
#include "QrTzhDlg.h"
#include "FanDian.h"
#include	"GridCtrl/GridCtrl.h"
#include "DlgStatus.h"
class CPlazaViewItem;
#include "..\..\�������\�������\ServiceCoreHead.h"

typedef CWHArray<CString>	CStringArr;					//��������



#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC			11110								//��ʾ�˵�
#define IDM_UPDATE_ACCOUNT			11113								//�����������
#define IDM_SHOW_XGMM				11117								//�޸�����

#define IDM_TANCHUANG				900									//����



// CXingYun28 �Ի���

class CXingYun28 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CXingYun28)

public:
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();


	enum {
		Kj_XinXi_Count = 5
	};


public:
	CXingYun28(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CXingYun28();

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int GetXy28Zhushu(int nTypeID,int nKindID);
	int				m_singlezhushu;
	DOUBLE			m_zongJine;


	HBRUSH hBrush;



// �Ի�������
	enum { IDD = IDD_XINGYUN_28 };

	int							m_SocketID;
	int							m_nTzhSign;
	int							m_nTzhSign1;
public:
	
	void FlushZongjine();
	bool	CheckInput();//У������
	CString		ChangeStringToT(CStringA strInput);



	//void CheckNumBtn()

	BYTE m_cbIfTs;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
// 	CTextButton m_btnYuan;
// 	CTextButton m_btnJiao;
// 	CTextButton m_btnFen;
// 	CTextButton m_btnLi;
// 	afx_msg void OnBnClickedBtnYuan();
// 	afx_msg void OnBnClickedBtnJiao();
// 	afx_msg void OnBnClickedBtnFen();
// 	afx_msg void OnBnClickedBtnLi();

	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

	//�淨��ť
	afx_msg void OnBnClinckedBtnRenXuan();	//��ѡ

	afx_msg void OnBnClinckedBtnTeMa();	//����
	afx_msg void OnBnClinckedBtnDaXiaoDanShuang();	//��С��˫ 
	afx_msg void OnBnClinckedBtnFunny();//Ȥζ�淨
	afx_msg void OnBnClinckedBtnJiZhi();	//��ֵ

	afx_msg void OnBnClinckedBtnDx();//��С
	afx_msg void OnBnClinckedBtnDsh();	//��˫

// 	afx_msg void OnRioDuiZi();	//����
// 	afx_msg void OnRioShunZi();	//˳��
// 	afx_msg void OnRioBaoZi();	//����

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	afx_msg void OnBnClickedBtnChqAdd();//��Ӻ���
	afx_msg void OnBnClickedBtnZhuihao();//׷��
	afx_msg void OnBnClickedBtnTouzhu();//Ͷע
	//afx_msg void OnBnClickedBtnLock();//�����޸�
	afx_msg void OnBnClickedBtnDelSel();//ɾ��
	afx_msg void OnBnClickedBtnSave();//ɾ��
	afx_msg void OnBnClickedBtnJetton1();//����
	afx_msg void OnBnClickedBtnJetton2();//����
	afx_msg void OnBnClickedBtnJetton3();//����
	afx_msg void OnBnClickedBtnJetton4();//����
	afx_msg void OnBnClickedBtnJetton5();//����
public:
	afx_msg void OnBnClickedBtnClsList();//���

	afx_msg void OnEnChangeEditDanshiInput();//ע���޸ġ�
	afx_msg void OnEnChangeEditBeishu();//���� �޸�


	DECLARE_MESSAGE_MAP()

	TCHAR				m_szMD5Haoma[33];

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	CString m_strTouzhuHaoma;
protected:
	bool							m_bZhuiZjtz;						//�Ƿ��н�ͣ׷
	int								m_nTouzhuCount;						//��ע��
	CPngImage						m_ImageItemBackMid;
	WORD							m_wGameHoverItem;
	CRect							m_GameListRect;						//�б�����
	bool							m_bShowMenu;						//�Ƿ���ʾ�˵�
	bool							m_bShowFail;						//��ʾʧ�ܶ���
	BYTE							m_cbMenuType;						//�˵����
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	CFont							m_Font;
	WORD							m_wViewItemDown;					//�������
	bool							m_bCanSend	;						//�Ƿ���
	CFanDian						m_FandianView;						//����
	bool							m_bDx;								//�Ƿ��С

	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	CString GetDxdshStringDesc(int nNum);
	CString GetFunnyDesc(int nNum);



	//����ؼ�
private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_numImageList1;
	Bitmap* m_numImageList2;
	Bitmap* m_numImageList3;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumBig2;
	Bitmap* m_kjAdd;
	Bitmap* m_kjAdd1;

	Bitmap* m_kjNumSmall;
	Bitmap* m_kjNumSmall1;
	Bitmap* m_bmpDxdsh;
	Bitmap* m_bmpDxdsh1;
	Bitmap* m_bmpDxdsh2;
	Bitmap* m_bmpNumTip;
	Bitmap* m_bmpNumTip1;
	Bitmap*	m_imgKaijiang;			//��������
	Bitmap* m_imgDxDsh;
	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;
	bool	m_bKaiJiangzhong;		//���ڿ���
	Bitmap* m_imgHistory;
	int m_gameKind;




	//����28����
	CNumerButton m_btnHeZhi_0;
	CNumerButton m_btnHeZhi_1;
	CNumerButton m_btnHeZhi_2;
	CNumerButton m_btnHeZhi_3;
	CNumerButton m_btnHeZhi_4;
	CNumerButton m_btnHeZhi_5;
	CNumerButton m_btnHeZhi_6;
	CNumerButton m_btnHeZhi_7;
	CNumerButton m_btnHeZhi_8;
	CNumerButton m_btnHeZhi_9;
	CNumerButton m_btnHeZhi_10;
	CNumerButton m_btnHeZhi_11;
	CNumerButton m_btnHeZhi_12;
	CNumerButton m_btnHeZhi_13;
	CNumerButton m_btnHeZhi_14;
	CNumerButton m_btnHeZhi_15;
	CNumerButton m_btnHeZhi_16;
	CNumerButton m_btnHeZhi_17;
	CNumerButton m_btnHeZhi_18;
	CNumerButton m_btnHeZhi_19;
	CNumerButton m_btnHeZhi_20;
	CNumerButton m_btnHeZhi_21;
	CNumerButton m_btnHeZhi_22;
	CNumerButton m_btnHeZhi_23;
	CNumerButton m_btnHeZhi_24;
	CNumerButton m_btnHeZhi_25;
	CNumerButton m_btnHeZhi_26;
	CNumerButton m_btnHeZhi_27;


	CTextButton m_btnRenXuan;//��ѡ1

	CTextButton m_btnTeMa;					//����
	CTextButton m_btnDaXiaoDanShuang;		//��С��˫
	CTextButton m_btnJiZhi;					//��ֵ
	CTextButton m_btnFunny;					//Ȥζ

	CTextButton m_btnTextDx;					//��С  
	CTextButton m_btnTextDsh;					//��˫  
// 	CImgRioButton m_rioDuiZi;					//����
// 	CImgRioButton m_rioShunZi;					//˳��
// 	CImgRioButton m_rioBaoZi;					//����

	CNumerButton m_btnMax;					//����
	CNumerButton m_btnMin;					//��С
	



	//��С��˫
	CNumerButton m_btnDa;
	CNumerButton m_btnXiao;
	CNumerButton m_btnDan;
	CNumerButton m_btnShuang;
	CNumerButton m_btnXiaoDan;				//С��
	CNumerButton m_btnXiaoShuang;			//С˫
	CNumerButton m_btnDaDan;					//��
	CNumerButton m_btnDaShuang;				//��˫



	CImageButton	m_btnSave;
	CImageButton m_btnDelSel;	//�Զ��� ��ť
	CImageButton m_btnClsList;	//��Ϸ˵��
	CImageButton m_btnTouZhu;	//Ͷע
	CImageButton m_btnZhuiHao;	//׷��
	CImageButton m_btnChqAdd;		//��Ӱ�ť
	//CTextButton m_btnLock;			//���㰴ť


	CSkinButton m_btnJetton1;
	CSkinButton m_btnJetton2;
	CSkinButton m_btnJetton3;
	CSkinButton m_btnJetton4;
	CSkinButton m_btnJetton5;
	bool		m_bShowEdit;
	CSkinEditEx m_editJetton1;		//��������
	CSkinEditEx m_editJetton2;		//��������
	CSkinEditEx m_editJetton3;		//��������
	CSkinEditEx m_editJetton4;		//��������
	CSkinEditEx m_editJetton5;		//��������

	CSkinEditEx m_editBeiTou;		//��������
	CEdit m_editDanShiHaoMa;

	CRichEditCtrl m_richDanshiHaoma;			//��ʽ����

	bool			m_RenxuanKeyboardInput;		//��ѡN�ļ�������



	CListCtrlCl m_listChqSSC;		//��ע�б�
	CGridCtrl	m_GridList;			//

	int				m_nZongZhuShu;				//������ע��
	UINT			m_beishu;					//����
	int				m_nFrameIndex;				//GIF�����ĵ�ǰ֡��
	int				m_nFrameCount;				//GIF�����ܹ�֡��


	CString m_StrFunny;
	static CString m_strHaoma;					//����

	//��󼸴ο�����Ϣ
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[50];

	//����С�Ŀ�����ʱʹ�õ�����
	CFont m_smallNumfont;
	//�ܽ����ע��ʹ�õ�����
	CFont m_zongFont;
	//������Ϣʹ������
	CFont m_KjInfoFont;
	CFont m_KjInfoFont1;
	CKuaiLe8RUle m_chqRule;				//ʱ��ؼ�

	//�������
public:
	void AdjustRadio();
	void AdjustWanFa();
	void AdjustNumView();
	void AdjustNumBtn();
	void AdjustDanShi();
	void AdjustAdd();

	void CheckWanFaBtn(CTextButton& btn);


	void ShowCtrl(int ctrl_id);
	void HideCtrl(int ctrl_id);

	//��ʾ����1-80
	void ShowBJNums();
	void HideBJNums();

	//��ʾ����1-27
	void ShowHezhiNums();
	void HideHeZhiNums();

	//��������״̬
	void ResetAllNums(void);

	//ѡ������
	void CheckNumBtn(CNumerButton& btn);
	//ȡ��ѡ������
	void UnCheckNumBtn(CNumerButton& btn);
	//�������Ƿ�ѡ��
	bool IsNumBtnCheck(CNumerButton& btn);

	//��ʾ��������
	void ShowDanShiEdit();
	//���ؼ�������
	void HideDanShiEdit();

	//��ʾ�����ش�С��˫
	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();

	//�����淨
	void HideRio();
	//ѡ���淨
	void SelectRio(CImgRioButton  &rio);
	//��ʾ���ؼ�ֵ
	void HideJiZhi();
	void ShowJiZji();







public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult);
	//���غ���
public:
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	void	GetSubStringA(CStringA& strInput, int nPos, int nLen, CStringA& strOutput);

	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	bool							m_bLogonSuccess;

	VOID SetLogonSuccess(bool bSuccess)
	{
		m_bLogonSuccess = bSuccess;
	}

	//������Ϣ��ȡ
private:
	long GetKjShjDiff();//��ȡ����ʱ��
	void InitNumberBtns();//��ʼ�����ְ�ť
	void InitListCtrl();//��ʼ���б�ؼ�

	//��ͼ����
	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);//��󿪽�����
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);//��󼸴εĿ�������
	void DrawZongZhushuJinE(CDC* pDC);					//��ע��
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num,int nWidth,int nHeight);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawDxdshNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);


	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	VOID SendToServer(int nSendType);

	CString GetBjNum();//��ȡ������8�İ�ť��ֵ
	CString GetHezhiString();//��ȡ����28��ֵ
	CString GetWanDesc();//��ȡ�淨����
	CString GetDxdshString();//��ȡ��С��˫
	CString GetJiZhiString();//��ȡ��ֵ



	//��ȡ��ʽ����
	//bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int nLeast);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);


	//��ʼ���淨��Ϣ
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//�������˷���ǰN����������
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);


public:
	bool						m_bQueryTime;					//�Ƿ��ѯʱ��
	bool						m_bQueryLuckyNum;					//�Ƿ��ѯʱ��
	bool						m_bTouzhu;						//�Ƿ���ע
	bool						m_bHovering;
	bool						m_bGetTime;
	bool						m_bGetCPUserInfo;				//��ȡ��Ʊ�û���Ϣ
	bool						m_bZhuihao;	
	bool						m_bGetUserFandian;				//��ȡ����
	bool						m_bGetMapBonus;
	bool						m_bGetLastYue;
	bool						m_bSetBonus;					//
	float						m_fBonus;						//���õ�
	float						m_fPercent;						//���õ�
	int							m_nZhuihaoCount;				//׷��ע��
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;
};
