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

typedef CWHArray<CString>	CStringArr;					//��������



#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC			11110								//��ʾ�˵�
#define IDM_UPDATE_ACCOUNT			11113								//�����������
#define IDM_SHOW_XGMM				11117								//�޸�����

#define IDM_TANCHUANG				900									//����



// CBeiJingKuai8 �Ի���

class CBeiJingKuai8 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CBeiJingKuai8)

public:
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();


	enum {
		Kj_XinXi_Count = 5
	};
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CImageButton					m_btWxzxTip;
	virtual BOOL PreTranslateMessage(MSG * pMsg);


public:
	CBeiJingKuai8(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBeiJingKuai8();



	HBRUSH hBrush;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int GetKl8Zhushu(int nTypeID,int nKindID);
	int				m_singlezhushu;
	DOUBLE			m_zongJine;

	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);

// �Ի�������
	enum { IDD = IDD_BEIJINGKUAI8 };

	int							m_SocketID;
	int							m_nTzhSign;
	int							m_nTzhSign1;
public:
	
	void FlushZongjine();
	bool	CheckInput();//У������
	CString		ChangeStringToT(CStringA strInput);


	BeiJingKuai8Kind GetGameKindByDesc(const CString& desc);




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;
	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();

	//�淨��ť
	afx_msg void OnBnClinckedBtnRenXuan();	//��ѡ

	afx_msg void OnBnClinckedBtnTeMa();	//����
	afx_msg void OnBnClinckedBtnDaXiaoDanShuang();	//��С��˫ 
	afx_msg void OnBnClinckedBtnFunny();//Ȥζ�淨
	afx_msg void OnBnClinckedBtnJiZhi();	//��ֵ

	afx_msg void OnBnClinckedBtnRenXuan6();	//��ѡ
	afx_msg void OnBnClinckedBtnRenXuan7();	//����
	afx_msg void OnBnClinckedBtnRenXuan8();	//��С��˫ 
	afx_msg void OnBnClinckedBtnRenXuan9();//Ȥζ�淨
	afx_msg void OnBnClinckedBtnRenXuan10();	//��ֵ
	afx_msg void OnBnClinckedBtnQuWei();	//Ȥζ

	afx_msg void OnRioDuiZi();	//����
	afx_msg void OnRioShunZi();	//˳��
	afx_msg void OnRioBaoZi();	//����

	//afx_msg void OnRioMax();		//����
	//afx_msg void OnRioMin();		//��С
	

	//�淨����

	////��С��˫
	//afx_msg void OnBnClinckedBtnDa();
	//afx_msg void OnBnClinckedBtnXiao();
	//afx_msg void OnBnClinckedBtnDan();
	//afx_msg void OnBnClinckedBtnShuang();
	//afx_msg void OnBnClinckedBtnXiaoDan();	//С��
	//afx_msg void OnBnClinckedBtnXiaoShuang();	//С˫
	//afx_msg void OnBnClinckedBtnDaDan();	//��
	//afx_msg void OnBnClinckedBtnDaShuang();	//��˫

	//afx_msg void OnBnClinckedBtnMax();		//����
	//afx_msg void OnBnClinckedBtnMin();		//��С





	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	CImgRioButton m_rioMouseInput;		//�������
	CImgRioButton m_rioKeyboardInput;	//��������

	afx_msg void OnBnClickedBtnChqAdd();//��Ӻ���
	afx_msg void OnBnClickedBtnZhuihao();//׷��
	afx_msg void OnBnClickedBtnTouzhu();//Ͷע
	afx_msg void OnBnClickedBtnTouzhuOneKey();//Ͷע
	afx_msg void OnBnClickedBtnLock();//�����޸�
	afx_msg void OnBnClickedBtnDelSel();//ɾ��
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;
	void OnBnClickedBtnTouzhuQ();//Ͷע

	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

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
	CString GetSZXDesc(int num);
protected:
	bool							m_bZhuiZjtz;						//�Ƿ��н�ͣ׷
	int								m_nTouzhuCount;						//��ע��
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



	//CString GetDxdshStringDesc();
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	CString GetDxdshStringDesc(int nNum);
	CString GetJHODesc(int nNum);
	CString GetHZDXDSDesc(int nNum);




	//����ؼ�
private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_numImageList1;
	Bitmap* m_numImageList2;
	Bitmap* m_numImageList3;
	Bitmap* m_numImageList4;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumBig2;
	Bitmap* m_kjAdd;

	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpDxdsh;
	Bitmap* m_bmpNumTip;
	Bitmap* m_bmpNumTip1;
	Bitmap*	m_imgKaijiang;			//��������
	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;
	bool	m_bKaiJiangzhong;		//���ڿ���

	BeiJingKuai8Kind m_gameKind;




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

	CNumerButton m_btnShang;
	CNumerButton m_btnZhong;
	CNumerButton m_btnXia;

	CNumerButton m_btnJi;
	CNumerButton m_btnHe;
	CNumerButton m_btnOu;

	CNumerButton m_btnDaDan1;
	CNumerButton m_btnDaShuang1;
	CNumerButton m_btnXiaoDan1;
	CNumerButton m_btnXiaoShuang1;
	//��������8
	CNumerButton	m_btnHuanLe_01;
	CNumerButton	m_btnHuanLe_02;
	CNumerButton	m_btnHuanLe_03;
	CNumerButton	m_btnHuanLe_04;
	CNumerButton	m_btnHuanLe_05;
	CNumerButton	m_btnHuanLe_06;
	CNumerButton	m_btnHuanLe_07;
	CNumerButton	m_btnHuanLe_08;
	CNumerButton	m_btnHuanLe_09;
	CNumerButton	m_btnHuanLe_10;
	CNumerButton	m_btnHuanLe_11;
	CNumerButton	m_btnHuanLe_12;
	CNumerButton	m_btnHuanLe_13;
	CNumerButton	m_btnHuanLe_14;
	CNumerButton	m_btnHuanLe_15;
	CNumerButton	m_btnHuanLe_16;
	CNumerButton	m_btnHuanLe_17;
	CNumerButton	m_btnHuanLe_18;
	CNumerButton	m_btnHuanLe_19;
	CNumerButton	m_btnHuanLe_20;
	CNumerButton	m_btnHuanLe_21;
	CNumerButton	m_btnHuanLe_22;
	CNumerButton	m_btnHuanLe_23;
	CNumerButton	m_btnHuanLe_24;
	CNumerButton	m_btnHuanLe_25;
	CNumerButton	m_btnHuanLe_26;
	CNumerButton	m_btnHuanLe_27;
	CNumerButton	m_btnHuanLe_28;
	CNumerButton	m_btnHuanLe_29;
	CNumerButton	m_btnHuanLe_30;
	CNumerButton	m_btnHuanLe_31;
	CNumerButton	m_btnHuanLe_32;
	CNumerButton	m_btnHuanLe_33;
	CNumerButton	m_btnHuanLe_34;
	CNumerButton	m_btnHuanLe_35;
	CNumerButton	m_btnHuanLe_36;
	CNumerButton	m_btnHuanLe_37;
	CNumerButton	m_btnHuanLe_38;
	CNumerButton	m_btnHuanLe_39;
	CNumerButton	m_btnHuanLe_40;
	CNumerButton	m_btnHuanLe_41;
	CNumerButton	m_btnHuanLe_42;
	CNumerButton	m_btnHuanLe_43;
	CNumerButton	m_btnHuanLe_44;
	CNumerButton	m_btnHuanLe_45;
	CNumerButton	m_btnHuanLe_46;
	CNumerButton	m_btnHuanLe_47;
	CNumerButton	m_btnHuanLe_48;
	CNumerButton	m_btnHuanLe_49;
	CNumerButton	m_btnHuanLe_50;
	CNumerButton	m_btnHuanLe_51;
	CNumerButton	m_btnHuanLe_52;
	CNumerButton	m_btnHuanLe_53;
	CNumerButton	m_btnHuanLe_54;
	CNumerButton	m_btnHuanLe_55;
	CNumerButton	m_btnHuanLe_56;
	CNumerButton	m_btnHuanLe_57;
	CNumerButton	m_btnHuanLe_58;
	CNumerButton	m_btnHuanLe_59;
	CNumerButton	m_btnHuanLe_60;
	CNumerButton	m_btnHuanLe_61;
	CNumerButton	m_btnHuanLe_62;
	CNumerButton	m_btnHuanLe_63;
	CNumerButton	m_btnHuanLe_64;
	CNumerButton	m_btnHuanLe_65;
	CNumerButton	m_btnHuanLe_66;
	CNumerButton	m_btnHuanLe_67;
	CNumerButton	m_btnHuanLe_68;
	CNumerButton	m_btnHuanLe_69;
	CNumerButton	m_btnHuanLe_70;
	CNumerButton	m_btnHuanLe_71;
	CNumerButton	m_btnHuanLe_72;
	CNumerButton	m_btnHuanLe_73;
	CNumerButton	m_btnHuanLe_74;
	CNumerButton	m_btnHuanLe_75;
	CNumerButton	m_btnHuanLe_76;
	CNumerButton	m_btnHuanLe_77;
	CNumerButton	m_btnHuanLe_78;
	CNumerButton	m_btnHuanLe_79;
	CNumerButton	m_btnHuanLe_80;


	CTextButton m_btnRenXuan;//��ѡ1

	CTextButton m_btnTeMa;					//����
	CTextButton m_btnDaXiaoDanShuang;		//��С��˫
	CTextButton m_btnJiZhi;					//��ֵ
	CTextButton m_btnFunny;					//Ȥζ
	CTextButton m_btnRenXuan6;//��ѡ6
	CTextButton m_btnRenXuan7;//��ѡ7
	CTextButton m_btnRenXuan8;//��ѡ8
	CTextButton m_btnRenXuan9;//��ѡ9
	CTextButton m_btnRenXuan10;//��ѡ10
	CTextButton m_btnQuWei;//��ѡ10

	CImgRioButton m_rioDuiZi;					//����
	CImgRioButton m_rioShunZi;					//˳��
	CImgRioButton m_rioBaoZi;					//����

	CNumerButton m_btnMax;					//����
	CNumerButton m_btnMin;					//��С
	
	CImgRioButton m_rioSZX;					//������
	CImgRioButton m_rioJHO;					//��ż��
	CImgRioButton m_rioHZDXDS;					//��ֵ��С��˫



	//��С��˫
	CNumerButton m_btnDa;
	CNumerButton m_btnXiao;
	CNumerButton m_btnDan;
	CNumerButton m_btnShuang;
	CNumerButton m_btnXiaoDan;				//С��
	CNumerButton m_btnXiaoShuang;			//С˫
	CNumerButton m_btnDaDan;					//��
	CNumerButton m_btnDaShuang;				//��˫




	CImageButton m_btnDelSel;	//ɾ��
	CImageButton m_btnClsList;	//���
	CImageButton m_btnTouZhu;	//Ͷע
	CImageButton m_btnZhuiHao;	//׷��
	CImageButton m_btnChqAdd;		//��Ӱ�ť
	CTextButton m_btnLock;			//���㰴ť
	CImageButton m_btnTouZhuOnekey;



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
	KjXinXi m_kjXinxi[Kj_XinXi_Count];

	//����С�Ŀ�����ʱʹ�õ�����
	CFont m_smallNumfont;
	//�ܽ����ע��ʹ�õ�����
	CFont m_zongFont;
	//������Ϣʹ������
	CFont m_KjInfoFont;
	CFont m_KjInfoFont1;
	afx_msg void OnBnClickedRioMouseInput();
	afx_msg void OnBnClickedRioKeyboardInput();

	
	CKuaiLe8RUle m_chqRule;				//ʱ��ؼ�
	BYTE m_cbIfTs;

	void ShowShangZhongXia();
	void HideShangZhongXia();
	void ShowJiHeOu();
	void HideJiHeOu();
	void ShowDaXiaoDanShuang1();
	void HideDaXiaoDanShuang1();

	//�������
public:
	void AdjustRadio();
	void AdjustWanFa();
	void AdjustNumView();
	void AdjustNumBtn();
	void AdjustDanShi();
	void AdjustAdd();

	void CheckWanFaBtn(CTextButton& btn);

	void HideAllRio();
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

	void OnBnClickedBtnSZX();
	void OnBnClickedBtnJHO();
	void OnBnClickedBtnHZDXDS();

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


	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	VOID SendToServer(int nSendType);

	CString GetBjNum();//��ȡ������8�İ�ť��ֵ
	CString GetHezhiString();//��ȡ����28��ֵ
	CString GetWanDesc();//��ȡ�淨����
	CString GetDxdshString();//��ȡ��С��˫
	CString GetJiZhiString();//��ȡ��ֵ

	CString GetShangZhongXiaString();
	CString GetJiHeOuString();
	CString GetDaXiaoDanShuang1String();


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
