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
#include "DlgStatus.h"
 #include "QrTzhDlg.h"
#include "FanDian.h"
#include "OpenRecord.h"

class CPlazaViewItem;


#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC				11110								//��ʾ�˵�
#define IDM_SHOW_MENU				11112								//��ʾ�˵�
#define IDM_UPDATE_ACCOUNT			11113								//�����������
#define IDM_SHOW_XGMM				11117								//�޸�����

// �㶫11ѡ5 �Ի���
class CGuangDong11X5 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CGuangDong11X5)
public:
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();
	COpenRecord						m_DlgOpenRecord;					//���࿪����¼
	CImageButton m_btnMoreRecord;
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CImageButton					m_btWxzxTip;
	bool							m_bZhuiZjtz;
	BYTE m_cbIfTs;

public:
	typedef enum 
	{
		inValid,

		IIRenXuan2=1,						//��ѡ��

		IIRenXuan3,					//��ѡ��
		IIRenXuan4,							//��ѡ��
		IIRenXuan5,							//��ѡ��

		IIRenXuan6,							//��ѡ��

		IIRenXuan7,							//��ѡ��

		IIRenXuan8,									//��ѡ��
		QianYi,					//ǰһ


		QianEr_ZhiXuan,							//ǰ�� ֱѡѡ��
		QianEr_ZuXuan,						//ǰ����ѡѡ��


		QianSan_ZhiXuan,							//ǰ��ֱѡѡ��
		QianSan_ZuXuan,						//ǰ����ѡѡ��
		Ren2_Dantuo,
		Ren3_Dantuo,
		Ren4_Dantuo,
		Ren5_Dantuo,
		Ren6_Dantuo,
		Ren7_Dantuo,
		Ren8_Dantuo,
		IIRenXuan1,
		WanFaCount
	}SSCGameKind;



	enum {
		Kj_XinXi_Count = 5
	};

public:
	CGuangDong11X5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGuangDong11X5();
	DOUBLE					m_zongJine;
// �Ի�������
	enum { IDD = IDD_DLG_GD_11X5 };
	int							m_nTzhSign;
	int							m_nTzhSign1;
	static CString m_strHaoma;
	bool							m_bCanSend	;						//�Ƿ���
	TCHAR				m_szMD5Haoma[33];
public:
	
	void FlushZongjine();
	bool	CheckInput();
	void SetTypeID(CaiZhong TypeID);
	CaiZhong	m_TypeID;
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);

	HBRUSH hBrush;
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int Get11x5Zhushu(int nTypeID,int nKindID);
	int m_singlezhushu;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedBtnMoreRecord();
	afx_msg void OnEnChangeRichEditDanshiInput();
	int GetDanShiZhusu(int nKind);

	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;
	CImageButton m_btnQushi;

	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();
	afx_msg void OnBnClickedBtnQuShi();

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );

	afx_msg void OnBnClickedBtnRx1();
	afx_msg void OnBnClickedBtnHszhx();
	afx_msg void OnBnClickedBtnDanTuo();
	afx_msg void OnBnClickedBtnHszx();
	afx_msg void OnBnClickedBtnWxhe();
	afx_msg void OnBnClickedBtnHezx();
	afx_msg void OnBnClickedBtnDxdsh();
	afx_msg void OnBnClickedBtnBdw();
	afx_msg void OnBnClickedBtnQszhx();
	afx_msg void OnBnClickedBtnQszx();
	afx_msg void OnBnClickedBtnWxqe();
	afx_msg void OnBnClickedBtnQezx();
	afx_msg void OnBnClickedBtnHszhxhzh();
	afx_msg void OnBnClickedBtnDwd();

	afx_msg void OnBnClickedBtnWanQuan();
	afx_msg void OnBnClickedBtnWanDa();
	afx_msg void OnBnClickedBtnWanXiao();
	afx_msg void OnBnClickedBtnWanDan();
	afx_msg void OnBnClickedBtnWanShuang();
	afx_msg void OnBnClickedBtnWanQing();
	afx_msg void OnBnClickedBtnQianQuan();
	afx_msg void OnBnClickedBtnQianDa();
	afx_msg void OnBnClickedBtnQianXiao();
	afx_msg void OnBnClickedBtnQianDan();
	afx_msg void OnBnClickedBtnQianShuang();
	afx_msg void OnBnClickedBtnQianQing();
	afx_msg void OnBnClickedBtnBaiQuan();
	afx_msg void OnBnClickedBtnBaiDa();
	afx_msg void OnBnClickedBtnBaiXiao();
	afx_msg void OnBnClickedBtnBaiDan();
	afx_msg void OnBnClickedBtnBaiShuang();
	afx_msg void OnBnClickedBtnBaiQing();
	afx_msg void OnBnClickedBtnShiQuan();
	afx_msg void OnBnClickedBtnShiDa();
	afx_msg void OnBnClickedBtnShiXiao();
	afx_msg void OnBnClickedBtnShiDan();
	afx_msg void OnBnClickedBtnShiShuang();
	afx_msg void OnBnClickedBtnShiQing();
	afx_msg void OnBnClickedBtnGeQuan();
	afx_msg void OnBnClickedBtnGeDa();
	afx_msg void OnBnClickedBtnGeXiao();
	afx_msg void OnBnClickedBtnGeDan();
	afx_msg void OnBnClickedBtnGeShuang();
	afx_msg void OnBnClickedBtnGeQing();
	afx_msg void OnBnClickedBtnChqAdd();

	afx_msg void OnBnClickedRioRenXuan2();
	afx_msg void OnBnClickedRioRenXuan1();
	afx_msg void OnBnClickedRioHszhxFushi();
	afx_msg void OnBnClickedRioHszsFushi();
	afx_msg void OnBnClickedRioQszsFushi();
	afx_msg void OnBnClickedRioHezhxFushi();
	afx_msg void OnBnClickedRioHezxFushi();
	afx_msg void OnBnClickedRioQezhxFushi();
	afx_msg void OnBnClickedRioQezhxDanshi();
	afx_msg void OnBnClickedRioQezxFushi();
	afx_msg void OnBnClickedRioQezxDanshi();
	afx_msg void OnBnClickedRioDxdsh();
	afx_msg void OnBnClickedRioHsbdw();
	afx_msg void OnBnClickedRioRen2DanTuo();
	afx_msg void OnBnClickedRioRen3DanTuo();
	afx_msg void OnBnClickedRioRen4DanTuo();
	afx_msg void OnBnClickedRioRen5DanTuo();
	afx_msg void OnBnClickedRioRen6DanTuo();
	afx_msg void OnBnClickedRioRen7DanTuo();
	afx_msg void OnBnClickedRioRen8DanTuo();

	afx_msg void OnBnClickedBtnZhuihao();
	afx_msg void OnBnClickedBtnTouzhu();
	afx_msg void OnBnClickedBtnTouzhuOneKey();
	void OnBnClickedBtnTouzhuQ();

	afx_msg void OnBnClickedBtnDelSel();
	afx_msg void OnBnClickedBtnClsList();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
	afx_msg void OnRichEditCopy();
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//�������
protected:
	WORD							m_wGameHoverItem;
	CRect							m_GameListRect;						//�б�����
	bool							m_bShowMenu;						//�Ƿ���ʾ�˵�
	BYTE							m_cbMenuType;						//�˵����
	int								m_nMenuXPos;
	int								m_nMenuYPos;
	CFont							m_Font;
	WORD							m_wViewItemDown;					//�������
	bool							m_bShowFail;

	CFanDian						m_FandianView;						//����

	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
// 	//����뿪
// 	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
// 

private:
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	CString ChangeStringToT(CStringA strInput);
	long GetKjShjDiff();
	//void InitWanFaBtns();
	void InitNumberBtns();
	void InitListCtrl();

	void CheckNumBtn(CNumerButton& btn);
	void UnCheckNumBtn(CNumerButton& btn);
	bool IsNumBtnCheck(CNumerButton& btn);

	void HideCtrl(int ctrl_id);
	void ShowCtrl(int ctrl_id);

	void HideAllRio();

	CString GetWanString();
	CString GetQianString();
	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	//��ȡ��С��˫
	CString GetShiDxdshString(); 
	CString GetGeDxdshString();

	CString GetWanDesc();
	SSCGameKind GetGameKindByDesc(const CString& desc);

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void HideWanNums();
	void HideQianNums();
	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void HideDanTuo();
	void ShowWanNums();
	void ShowQianNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();
	void ShowDanTuo();

	//��ʾ������ѡ�Ͳ���λ
	void ShowZuXuan();
	void ShowBuDingWei();

	//��ʾ�����ش�С��˫
	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();
	
	//��ʾ�����ص�ʽѡ��
	void HideDanShiEdit();
	void ShowDanShiEdit();
	//��ѡ���е���ǧ���٣�ʮ����λ��
	void ShowRenxuanWeiShu();
	void HideRenxuanWeiShu();
	void SetCheckRenXuan(bool bChecked);
	void CheckWanFaBtn(CTextButton& btn);
	void ValidateTips();
	//����ѡ�пؼ��ĸ���
	int GetRenXuanCheckBox(BOOL& bWan, BOOL& bQian, BOOL& bBai, BOOL& bShi, BOOL& bGet);		//��ѡ����ǧ���٣�ʮ����
	int factorial(int number);
	int combo(int base, int up);
	//��XML���л�ȡ��������
	bool LuckyNumCQSSC();

	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawZongZhushuJinE(CDC* pDC);
	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	void ResetAllNums();
	VOID SendQueryLuckyNumCQSSC();

	VOID SendToServer(int nSendType);

	//void SetLockText();

	//��ȡ������ʽ�ĺ���
	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);
	//��ȡ��ʽ����
	bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int nLeast);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);

	//��ʼ���淨��Ϣ
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//�������˷���ǰN����������
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize,int nTypeID);
private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpDxdsh;
	Bitmap* m_bmpNumTip;
	Bitmap*	m_imgKaijiang;			//��������
	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;
	bool	m_bKaiJiangzhong;		//���ڿ���

	SSCGameKind m_gameKind;

	CNumerButton m_btnWan_0;
	CNumerButton m_btnWan_1;
	CNumerButton m_btnWan_2;
	CNumerButton m_btnWan_3;
	CNumerButton m_btnWan_4;
	CNumerButton m_btnWan_5;
	CNumerButton m_btnWan_6;
	CNumerButton m_btnWan_7;
	CNumerButton m_btnWan_8;
	CNumerButton m_btnWan_9;
	CNumerButton m_btnWan_10;

	CNumerButton m_btnQian_0;
	CNumerButton m_btnQian_1;
	CNumerButton m_btnQian_2;
	CNumerButton m_btnQian_3;
	CNumerButton m_btnQian_4;
	CNumerButton m_btnQian_5;
	CNumerButton m_btnQian_6;
	CNumerButton m_btnQian_7;
	CNumerButton m_btnQian_8;
	CNumerButton m_btnQian_9;
	CNumerButton m_btnQian_10;

	CNumerButton m_btnBai_0;
	CNumerButton m_btnBai_1;
	CNumerButton m_btnBai_2;
	CNumerButton m_btnBai_3;
	CNumerButton m_btnBai_4;
	CNumerButton m_btnBai_5;
	CNumerButton m_btnBai_6;
	CNumerButton m_btnBai_7;
	CNumerButton m_btnBai_8;
	CNumerButton m_btnBai_9;
	CNumerButton m_btnBai_10;

	CNumerButton m_btnShi_0;
	CNumerButton m_btnShi_1;
	CNumerButton m_btnShi_2;
	CNumerButton m_btnShi_3;
	CNumerButton m_btnShi_4;
	CNumerButton m_btnShi_5;
	CNumerButton m_btnShi_6;
	CNumerButton m_btnShi_7;
	CNumerButton m_btnShi_8;
	CNumerButton m_btnShi_9;
	CNumerButton m_btnShi_10;

	CNumerButton m_btnGe_0;
	CNumerButton m_btnGe_1;
	CNumerButton m_btnGe_2;
	CNumerButton m_btnGe_3;
	CNumerButton m_btnGe_4;
	CNumerButton m_btnGe_5;
	CNumerButton m_btnGe_6;
	CNumerButton m_btnGe_7;
	CNumerButton m_btnGe_8;
	CNumerButton m_btnGe_9;
	CNumerButton m_btnGe_10;

	CListCtrlCl m_listChqSSC;
	int				m_zongZhuShu;				//������ע��
	bool			m_RenxuanKeyboardInput;		//��ѡN�ļ�������

	//CArray<TouZhuOrder, TouZhuOrder&> m_arrOrder;
	UINT			m_beishu;					//����
	int				m_nFrameIndex;				//GIF�����ĵ�ǰ֡��
	int				m_nFrameCount;				//GIF�����ܹ�֡��
	//CStatic m_staticDanShi;
	CEdit m_editDanShiHaoMa;
	CRichEditCtrl m_richDanshiHaoma;

	CImgStatic m_staticWan;
	CImgStatic m_staticQian;
	CImgStatic m_staticBai;
	CImgStatic m_staticShi;
	CImgStatic m_staticGe;
// 	CImgStatic m_staticDan;
// 	CImgStatic m_staticTuo;

	CTextButton m_btnWanQuan;
	CTextButton m_btnWanDa;
	CTextButton m_btnWanXiao;
	CTextButton m_btnWanDan;
	CTextButton m_btnWanShuang;
	CTextButton m_btnWanQing;

	CTextButton m_btnQianQuan;
	CTextButton m_btnQianDa;
	CTextButton m_btnQianXiao;
	CTextButton m_btnQianDan;
	CTextButton m_btnQianShuang;
	CTextButton m_btnQianQing;

	CTextButton m_btnBaiQuan;
	CTextButton m_btnBaiDa;
	CTextButton m_btnBaiXiao;
	CTextButton m_btnBaiDan;
	CTextButton m_btnBaiShuang;
	CTextButton m_btnBaiQing;

	CTextButton m_btnShiQuan;
	CTextButton m_btnShiDa;
	CTextButton m_btnShiXiao;
	CTextButton m_btnShiDan;
	CTextButton m_btnShiShuang;
	CTextButton m_btnShiQing;

	CTextButton m_btnGeQuan;
	CTextButton m_btnGeDa;
	CTextButton m_btnGeXiao;
	CTextButton m_btnGeDan;
	CTextButton m_btnGeShuang;
	CTextButton m_btnGeQing;

	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;
	CImageButton m_btnTouZhu;
	CImageButton m_btnTouZhuOnekey;
	CImageButton m_btnZhuiHao;

	CSkinEditEx m_editBeiTou;

	CImageButton m_btnChqAdd;
	CTextButton m_btnLock;

	CTextButton m_btnRenXuan1;
	CTextButton m_btnHouSanZhiXuan;
	CTextButton m_btnQianSanZhiXuan;
	CTextButton m_btnHouSanZuXuan;
	CTextButton m_btnQianSanZuXuan;
	CTextButton m_btnWuXingHouEr;
	CTextButton m_btnWuXingQianEr;
	CTextButton m_btnHouErZuXuan;
	CTextButton m_btnQianErZuXuan;
	CTextButton m_btnDaXiaoDanShuang;
	CTextButton m_btnHouSanHeZhi;
	CTextButton m_btnBuDingWei;
	CTextButton m_btnDanTuo;
	//CTextButton m_btnDingWeiDan;

	CMarkup m_xml;

	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[50][Kj_XinXi_Count];

	CImgRioButton m_rioRenXuan2;
	CImgRioButton m_rioRenXuan3;
	CImgRioButton m_rioRenXuan4;
	CImgRioButton m_rioMouseInput;
	CImgRioButton m_rioKeyboardInput;
	//
	CImgRioButton m_rioRen2DanTuo;
	CImgRioButton m_rioRen3DanTuo;
	CImgRioButton m_rioRen4DanTuo;
	CImgRioButton m_rioRen5DanTuo;
	CImgRioButton m_rioRen6DanTuo;
	CImgRioButton m_rioRen7DanTuo;
	CImgRioButton m_rioRen8DanTuo;

	CButton		m_rioWan;
	CButton		m_rioQian;
	CButton		m_rioBai;
	CButton		m_rioShi;
	CButton		m_rioGe;

	CImgRioButton m_rioRenXuan1;
	CImgRioButton m_rioHsZhxFuShi;
	CImgRioButton m_rioHsZhxDanShi;
	CImgRioButton m_rioQsZhxFuShi;
	CImgRioButton m_rioQsZhxDanShi;
	CImgRioButton m_rioHsZsFuShi;
	//CImgRioButton m_rioHsZsDanShi;
	CImgRioButton m_rioHsZlFuShi;
	//CImgRioButton m_rioHsZlDanShi;
	CImgRioButton m_rioQsZsFuShi;
	//CImgRioButton m_rioQsZsDanShi;
	CImgRioButton m_rioQsZlFuShi;
	//CImgRioButton m_rioQsZlDanShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioHeZxFuShi;
	CImgRioButton m_rioHeZxDanShi;
	CImgRioButton m_rioQeZxFuShi;
	CImgRioButton m_rioQeZxDanShi;
	CImgRioButton m_rioQeDxdsh;
	CImgRioButton m_rioQsbdw;
	CImgRioButton m_rioHsbdw;
	CImgRioButton m_rioDwd;

	CImgRioButton m_rioQsHunhe;				//ǰ�����
	CImgRioButton m_rioZsHunhe;				//�������
	CImgRioButton m_rioHsHunhe;				//�������

	CImgStatic m_staticZuXuan;
	CImgStatic m_staticBuDingWei;
	CStatic		m_staticHunheTip;
	CImgStatic m_staticDanMa;
	CImgStatic m_staticTuoMa;

	//��С��˫
	CNumerButton m_btnDaShi;
	CNumerButton m_btnXiaoShi;
	CNumerButton m_btnDanShi;
	CNumerButton m_btnShuangShi;
	CNumerButton m_btnDaGe;
	CNumerButton m_btnXiaoGe;
	CNumerButton m_btnDanGe;
	CNumerButton m_btnShuangGe;

	//����С�Ŀ�����ʱʹ�õ�����
	CFont m_smallNumfont;
	//�ܽ����ע��ʹ�õ�����
	CFont m_zongFont;
	//������Ϣʹ������
	CFont m_KjInfoFont;
	CFont m_KjInfoFont1;

	afx_msg void OnBnClickedRioMouseInput();
	afx_msg void OnBnClickedRioKeyboardInput();
	afx_msg void OnBnClickedChkWan();
	afx_msg void OnBnClickedChkQian();
	afx_msg void OnBnClickedChkBai();
	afx_msg void OnBnClickedChkShi();
	afx_msg void OnBnClickedChkge();
	afx_msg void OnBnClickedBtnLock();
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;


	CGD11X5Rule m_chqRule;
	CJX11X5Rule m_jxRule;
	CSD11X5Rule m_sdRule;
	CHLJ11X5Rule m_hljRule;
public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult);
	//���غ���
public:
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
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

