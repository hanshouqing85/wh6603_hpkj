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
#include "OpenRecord.h"
typedef CWHArray<CString>	CStringArr;					//��������
#include "MyStatic.h"
#define IDM_LUCKY_NUM_SSC			10001
#define IDM_SHOW_MENU_SSC			11110								//��ʾ�˵�
#define IDM_UPDATE_ACCOUNT			11113								//�����������
#define IDM_SHOW_XGMM				11117								//�޸�����

#define IDM_TANCHUANG				900									//����
// ����ʱʱ�� �Ի���
class CChongQingSSC : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CChongQingSSC)

public:
	CDlgStatus						m_DlgStatus;						//״̬����
	COpenRecord						m_DlgOpenRecord;					//���࿪����¼
	//ȡ������
	virtual VOID OnStatusCancel();
	CString ChangeStringToT(CStringA strInput);
// 	//ʱʱ���淨����
	enum {
		Dxdsh_Shi_Da,
		Dxdsh_Shi_Xiao,
		Dxdsh_Shi_Dan,
		Dxdsh_Shi_Shuang,
		Dxdsh_Ge_Da,
		Dxdsh_Ge_Xiao,
		Dxdsh_Ge_Dan,
		Dxdsh_Ge_Shuang,
	};

	enum {
		Kj_XinXi_Count = 5
	};

public:
	CChongQingSSC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChongQingSSC();

	HBRUSH hBrush;
	BYTE m_cbIfTs;
// �Ի�������
	enum { IDD = IDD_DLG_CHQSSC };
	int							m_SocketID;
	int							m_nTzhSign;
	int							m_nTzhSign1;
public:
	
	void FlushZongjine();
	bool	CheckInput();
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);
	int	m_nCanadaStartQihao;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedBtnHszhx();
	afx_msg void OnBnClickedBtnNiuNiu();
	afx_msg void OnBnClickedBtnLhd();
	afx_msg void OnBnClickedBtnHszx();
	afx_msg void OnBnClickedBtnWxhe();
	afx_msg void OnBnClickedBtnHezx();
	afx_msg void OnBnClickedBtnDxdsh();
	afx_msg void OnBnClickedBtnBdw();
	afx_msg void OnBnClickedBtnQszhx();
	afx_msg void OnBnClickedBtnQszx();
	afx_msg void OnBnClickedBtnWxqe();
	afx_msg void OnBnClickedBtnQezx();
	afx_msg void OnBnClickedBtnErXing();
	afx_msg void OnBnClickedBtnHszhxhzh();
	afx_msg void OnBnClickedBtnDwd();
	afx_msg void OnBnClickedBtnQw();

	afx_msg void OnBnClickedBtnNiuQuan();
	afx_msg void OnBnClickedBtnNiuDa();
	afx_msg void OnBnClickedBtnNiuXiao();
	afx_msg void OnBnClickedBtnNiuDan();
	afx_msg void OnBnClickedBtnNiuShuang();
	afx_msg void OnBnClickedBtnNiuQing();

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

	afx_msg void OnBnClickedRioZuXuan120();
	afx_msg void OnBnClickedRioZuXuan60();
	afx_msg void OnBnClickedRioZuXuan30();
	afx_msg void OnBnClickedRioZuXuan20();
	afx_msg void OnBnClickedRioZuXuan10();
	afx_msg void OnBnClickedRioZuXuan5();

	afx_msg void OnBnClickedRioQsZuXuan24();
	afx_msg void OnBnClickedRioQsZuXuan12();
	afx_msg void OnBnClickedRioQsZuXuan6();
	afx_msg void OnBnClickedRioQsZuXuan4();

	afx_msg void OnBnClickedRioHsZuXuan24();
	afx_msg void OnBnClickedRioHsZuXuan12();
	afx_msg void OnBnClickedRioHsZuXuan6();
	afx_msg void OnBnClickedRioHsZuXuan4();
	afx_msg void OnBnClickedRioYffs();
	afx_msg void OnBnClickedRioHscs();
	afx_msg void OnBnClickedRioSxbx();
	afx_msg void OnBnClickedRioSjfc();

	afx_msg void OnBnClickedRioLhdWq();
	afx_msg void OnBnClickedRioLhdWb();
	afx_msg void OnBnClickedRioLhdWs();
	afx_msg void OnBnClickedRioLhdWg();
	afx_msg void OnBnClickedRioLhdQb();
	afx_msg void OnBnClickedRioLhdQs();
	afx_msg void OnBnClickedRioLhdQg();
	afx_msg void OnBnClickedRioLhdBs();
	afx_msg void OnBnClickedRioLhdBg();
	afx_msg void OnBnClickedRioLhdSg();

	afx_msg void OnBnClickedRioQsHezhi();
	afx_msg void OnBnClickedRioZsHezhi();
	afx_msg void OnBnClickedRioHsHezhi();
	afx_msg void OnBnClickedRioQeHezhi();
	afx_msg void OnBnClickedRioHeHezhi();

	afx_msg void OnBnClickedRioWuXingZhx();
	afx_msg void OnBnClickedRioWuXingZhxDan();
	afx_msg void OnBnClickedRioWuXingQs();		//����ǰ��
	afx_msg void OnBnClickedRioWuXingQsDan();	//����ǰ�ĵ�ʽ
 	afx_msg void OnBnClickedRioWuXingHS();		//���Ǻ���
	afx_msg void OnBnClickedRioWuXingHSDan();	//���Ǻ��ĵ�ʽ
 	afx_msg void OnBnClickedRioWuXingZS();		//��������
	afx_msg void OnBnClickedRioWuXingZSDan();	//����������ʽ

	afx_msg void OnBnClickedRioNiuShu();
	afx_msg void OnBnClickedRioNiuNiu();
	afx_msg void OnBnClickedRioWuNiu();
	afx_msg void OnBnClickedRioRenXuan2();
	afx_msg void OnBnClickedRioRenXuan2Dan();
	afx_msg void OnBnClickedRioRenXuan2Zx();
	afx_msg void OnBnClickedRioRenXuan2ZxDan();
	afx_msg void OnBnClickedRioRenXuan3();
	afx_msg void OnBnClickedRioRenXuan3Dan();
	afx_msg void OnBnClickedRioRenXuan3Zx3();
	afx_msg void OnBnClickedRioRenXuan3Zx3Dan();
	afx_msg void OnBnClickedRioRenXuan3Zx6();
	afx_msg void OnBnClickedRioRenXuan3Zx6Dan();
	afx_msg void OnBnClickedRioRenXuan3Hunhe();
	afx_msg void OnBnClickedRioRenXuan4();
	afx_msg void OnBnClickedRioRenXuan4Dan();
	afx_msg void OnBnClickedRioHszhxFushi();
	afx_msg void OnBnClickedRioHszhxDanshi();
	afx_msg void OnBnClickedRioQszhxFushi();
	afx_msg void OnBnClickedRioQszhxDanshi();
	afx_msg void OnBnClickedRioHszsFushi();
	afx_msg void OnBnClickedRioHszsDanshi();
	afx_msg void OnBnClickedRioHszlFushi();
	afx_msg void OnBnClickedRioZszsFushi();	//��������
	afx_msg void OnBnClickedRioZszsDanshi();	//��������
	afx_msg void OnBnClickedRioZszlFushi();	//��������
	afx_msg void OnBnClickedRioZszlDanshi();	//��������
	afx_msg void OnBnClickedRioHszlDanshi();
	afx_msg void OnBnClickedRioQszsFushi();
	afx_msg void OnBnClickedRioQszsDanshi();
	afx_msg void OnBnClickedRioQszlFushi();
	afx_msg void OnBnClickedRioQszlDanshi();
	afx_msg void OnBnClickedRioHezhxFushi();
	afx_msg void OnBnClickedRioHezhxDanshi();
	afx_msg void OnBnClickedRioHezxFushi();
	afx_msg void OnBnClickedRioHezxDanshi();
	afx_msg void OnBnClickedRioQezhxFushi();
	afx_msg void OnBnClickedRioQezhxDanshi();
	afx_msg void OnBnClickedRioQezxFushi();
	afx_msg void OnBnClickedRioQezxDanshi();
	afx_msg void OnBnClickedRioDxdsh();
	afx_msg void OnBnClickedRioDxdshDw();
	afx_msg void OnBnClickedRioDxdshHzh();
	afx_msg void OnBnClickedRioQsbdw();
	afx_msg void OnBnClickedRioZsbdw();
	afx_msg void OnBnClickedRioHsbdw();
	afx_msg void OnBnClickedRioQsHunhe();
	afx_msg void OnBnClickedRioZsHunhe();
	afx_msg void OnBnClickedRioHsHunhe();
	afx_msg void OnBnClickedRioDwd();
	afx_msg void OnBnClickedRioQebdw();
	afx_msg void OnBnClickedRioHebdw();

	afx_msg void OnBnClickedBtnZhuihao();
	afx_msg void OnBnClickedBtnMoreRecord();
	afx_msg void OnBnClickedBtnQuShi();
	afx_msg void OnBnClickedBtnTouzhu();
	afx_msg void OnBnClickedBtnTouzhuOneKey();
	void OnBnClickedBtnTouzhuQ();
	
	afx_msg void OnBnClickedBtnDelSel();
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnBnClickedBtnClsList();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeRichEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
	DECLARE_MESSAGE_MAP()

	TCHAR				m_szMD5Haoma[33];
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();

	//�������
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
	CFont							m_TimeFont;
	WORD							m_wViewItemDown;					//�������
	bool							m_bCanSend	;						//�Ƿ���
	CFanDian						m_FandianView;						//����

	int GetDanShiZhusu(int nKind);
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
	CString GetHezhiString();
	CString GetNiuString();
	CString GetNiuNumString();
	CString GetNiuNum(CString strNiu);
	CString GetNiuString(int nNiu);
	CString GetWanString();
	CString GetQianString();
	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	//��ȡ��С��˫
	CString GetWanDxdshString(); 
	CString GetQianDxdshString();
	CString GetBaiDxdshString(); 
	CString GetShiDxdshString(); 
	CString GetGeDxdshString();

	CString GetWanDesc();
	SSCWanFaKind GetGameKindByDesc(const CString& desc);

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void HideHeZhiNums();
	void ShowHeZhiNums();
	void HideErXingHeZhiNums();
	void ShowErXingHeZhiNums();
	void HideLonghudou();
	void ShowLonghudou();

	void HideWanNums();
	void HideQianNums();
	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void HideNiuNums();
	void ShowWanNums();
	void ShowQianNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();
	void ShowNiuNums();

	//��ʾ������ѡ�Ͳ���λ
	void ShowZuXuan();
	void ShowDanHao();
	void ShowBuDingWei();
	void ShowXuanHao();
	void ShowZu120();
	void ShowErChong();
	void ShowErChong1();
	void ShowSanChong();
	void ShowSiChong();
	void ShowSxErChong();
	void ShowSxSanChong();

	//��ʾ�����ش�С��˫
	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();
	//��ʾ�����ش�С��˫ ��λ
	void ShowDaXiaoDanShuangDw();
	void HideDaXiaoDanShuangDw();
	//��ʾ�����ش�С��˫ ��ֵ
	void ShowDaXiaoDanShuangHzh();
	void HideDaXiaoDanShuangHzh();
	
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

	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawZongZhushuJinE(CDC* pDC);
	void SetLockText(float fBonus=0.0f, float fBonusPercent=0.0f);
	void ResetAllNums();
	VOID SendQueryLuckyNumCQSSC();
	CString GetDxdshStringDesc(int nNum);
	VOID SendToServer(int nSendType);

	//void SetLockText();

	//��ȡ������ʽ�ĺ���
	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);
	//��ȡ��ʽ����
	bool GetDanShiHaoma(CStringArray& strArr, int size);
	bool GetDanShiHaoma(CString& haoma, int size, int& zhushu, int nMax=0);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size);

	bool GetKeyBoardHaoma(CString& haoma, int size, int nLeast);
	bool GetKeyHaomaStrs(CStringArray& strArr, CString line, int size, int nLeast);

	//��ʼ���淨��Ϣ
	void InitWanFaMoShi();
public:
	VOID SendQuerySystemTime();
	//�������˷���ǰN����������
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize,int nTypeID);
	void SetTypeID(CaiZhong TypeID);
private:

	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CToolTipCtrl					m_ToolTipHelp;						//��ʾ�ؼ�

	CImageButton					m_btWxzxTip;
	CImageButton					m_btWxzxHelp;
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_numImageList1;
	Bitmap* m_numImageList2;
	Bitmap* m_numImageList3;
	Bitmap* m_kjNumBig;
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

	SSCWanFaKind m_gameKind;
	CaiZhong	m_TypeID;
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
	
	CNumerButton m_btnNiu_0;
	CNumerButton m_btnNiu_1;
	CNumerButton m_btnNiu_2;
	CNumerButton m_btnNiu_3;
	CNumerButton m_btnNiu_4;
	CNumerButton m_btnNiu_5;
	CNumerButton m_btnNiu_6;
	CNumerButton m_btnNiu_7;
	CNumerButton m_btnNiu_8;
	CNumerButton m_btnNiu_9;
	CNumerButton m_btnNiu_10;

	CNumerButton m_btnLong;
	CNumerButton m_btnHu;
	CNumerButton m_btnHe;

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

	CTextButton	m_staticRio1;
	CTextButton	m_staticRio2;
	CTextButton	m_staticRio3;
	CListCtrlCl m_listChqSSC;
	CGridCtrl	m_GridList;
	int				m_zongZhuShu;				//������ע��
	DOUBLE			m_zongJine;					//�ܽ��
	int				m_singlezhushu;
	bool			m_RenxuanKeyboardInput;		//��ѡN�ļ�������
	int				m_nTouzhu;
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
	CImgStatic m_staticNiu;
	CImgStatic m_staticLhd;
	CImgStatic m_statichzh;

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

	CTextButton m_btnNiuQuan;
	CTextButton m_btnNiuDa;
	CTextButton m_btnNiuXiao;
	CTextButton m_btnNiuDan;
	CTextButton m_btnNiuShuang;
	CTextButton m_btnNiuQing;

	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;
	CImageButton m_btnTouZhu;
	CImageButton m_btnZhuiHao;
	CImageButton m_btnTouZhuOnekey;

	CImageButton m_btnMoreRecord;
	CImageButton m_btnQushi;
	CSkinEditEx m_editBeiTou;

	CImageButton m_btnChqAdd;
	CTextButton m_btnLock;

	CTextButton m_btnHouSanZhiXuan;
	CTextButton m_btnNiuNiu;
	CTextButton m_btnLhd;
	CTextButton m_btnQianSanZhiXuan;
	CTextButton m_btnHouSanZuXuan;
	CTextButton m_btnQianSanZuXuan;
	CTextButton m_btnWuXingHouEr;
	CTextButton m_btnWuXingQianEr;
	CTextButton m_btnHouErZuXuan;
	CTextButton m_btnQianErZuXuan;
	CTextButton m_btnQianErXing;
	CTextButton m_btnDaXiaoDanShuang;
	CTextButton m_btnHouSanHeZhi;
	CTextButton m_btnBuDingWei;
	CTextButton m_btnDingWeiDan;
	CTextButton m_btnQuwei;


	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;

	CMarkup m_xml;
	int m_nLhdID;
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[50][Kj_XinXi_Count];

	CImgRioButton m_rioRenXuan2;
	CImgRioButton m_rioRenXuan2Dan;
	CImgRioButton m_rioRenXuan2Zx;
	CImgRioButton m_rioRenXuan2ZxDan;
	CImgRioButton m_rioRenXuan3;
	CImgRioButton m_rioRenXuan3Dan;
	CImgRioButton m_rioRenXuan3Zx3;
	CImgRioButton m_rioRenXuan3Zx3Dan;
	CImgRioButton m_rioRenXuan3Zx6;
	CImgRioButton m_rioRenXuan3Zx6Dan;
	CImgRioButton m_rioRenXuan3Hunhe;
	CImgRioButton m_rioRenXuan4;
	CImgRioButton m_rioRenXuan4Dan;
	CImgRioButton m_rioMouseInput;
	CImgRioButton m_rioKeyboardInput;

	//
	CButton		m_rioWan;
	CButton		m_rioQian;
	CButton		m_rioBai;
	CButton		m_rioShi;
	CButton		m_rioGe;

 	CImgRioButton m_rioWuXingZX;
	CImgRioButton m_rioWuXingZXDan;
 	CImgRioButton m_rioWuXingQS;		//����ǰ��
	CImgRioButton m_rioWuXingQSDan;		//����ǰ�ĵ�ʽ
	CImgRioButton m_rioWuXingHS;		//���Ǻ���
	CImgRioButton m_rioWuXingHSDan;		//���Ǻ��ĵ�ʽ
 	CImgRioButton m_rioWuXingZS;		//����ֱѡ
	CImgRioButton m_rioWuXingZSDan;		//����ֱѡ��ʽ
	CImgRioButton m_rioNiuShu;
	CImgRioButton m_rioNiuNiu;
	CImgRioButton m_rioWuNiu;

	CImgRioButton m_rioLhdWq;
	CImgRioButton m_rioLhdWb;
	CImgRioButton m_rioLhdWs;
	CImgRioButton m_rioLhdWg;
	CImgRioButton m_rioLhdQb;
	CImgRioButton m_rioLhdQs;
	CImgRioButton m_rioLhdQg;
	CImgRioButton m_rioLhdBs;
	CImgRioButton m_rioLhdBg;
	CImgRioButton m_rioLhdSg;

	CImgRioButton m_rioHsZhxFuShi;
	CImgRioButton m_rioHsZhxDanShi;
	CImgRioButton m_rioQsZhxFuShi;
	CImgRioButton m_rioQsZhxDanShi;
	CImgRioButton m_rioHsZsFuShi;
	CImgRioButton m_rioHsZsDanShi;
	CImgRioButton m_rioHsZlFuShi;
	CImgRioButton m_rioHsZlDanShi;
	CImgRioButton m_rioQsZsFuShi;
	CImgRioButton m_rioQsZsDanShi;
	CImgRioButton m_rioQsZlFuShi;
	CImgRioButton m_rioQsZlDanShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioHeZxFuShi;
	CImgRioButton m_rioHeZxDanShi;
	CImgRioButton m_rioQeZxFuShi;
	CImgRioButton m_rioQeZxDanShi;
	CImgRioButton m_rioQeDxdsh;
	CImgRioButton m_rioDwDxdsh;
	CImgRioButton m_rioHzhDxdsh;
	CImgRioButton m_rioQsbdw;
	CImgRioButton m_rioZsbdw;
	CImgRioButton m_rioHsbdw;
	CImgRioButton m_rioDwd;
	CImgRioButton m_rioHebdw;
	CImgRioButton m_rioQebdw;

	CImgRioButton m_rioZsZsFuShi;			//��������
	CImgRioButton m_rioZsZsDanShi;			//��������
	CImgRioButton m_rioZsZlFuShi;			//��������
	CImgRioButton m_rioZsZlDanShi;			//��������
	CImgRioButton m_rioZuXuan120;
	CImgRioButton m_rioZuXuan60;
	CImgRioButton m_rioZuXuan30;
	CImgRioButton m_rioZuXuan20;
	CImgRioButton m_rioZuXuan10;
	CImgRioButton m_rioZuXuan5;			

 	CImgRioButton m_rioQsZuXuan24;
 	CImgRioButton m_rioQsZuXuan12;
 	CImgRioButton m_rioQsZuXuan6;
 	CImgRioButton m_rioQsZuXuan4;			
	CImgRioButton m_rioHsZuXuan24;
	CImgRioButton m_rioHsZuXuan12;
	CImgRioButton m_rioHsZuXuan6;
	CImgRioButton m_rioHsZuXuan4;			

	CImgRioButton m_rioYffs;
	CImgRioButton m_rioHscs;
	CImgRioButton m_rioSxbx;
	CImgRioButton m_rioSjfc;	

	CImgRioButton m_rioQsHezhi;
	CImgRioButton m_rioZsHezhi;
	CImgRioButton m_rioHsHezhi;
	CImgRioButton m_rioQeHezhi;				//ǰ����ֵ
	CImgRioButton m_rioHeHezhi;				//�����ֵ

	CImgRioButton m_rioQsHunhe;				//ǰ�����
	CImgRioButton m_rioZsHunhe;				//�������
	CImgRioButton m_rioHsHunhe;				//�������

	CImgStatic m_staticZuXuan;
	CImgStatic m_staticBuDingWei;
	CStaticTrans m_staticHunheTip;
	CStaticTrans m_staticNiuNiuTip;
	CImgStatic m_staticDanHao;
	CImgStatic m_staticXuanHao;
	CImgStatic m_staticZu120;
	CImgStatic m_staticErChong;
	CImgStatic m_staticErChong1;
	CImgStatic m_staticSanChong;
	CImgStatic m_staticSiChong;

	//��С��˫
	CNumerButton m_btnDaWan;
	CNumerButton m_btnXiaoWan;
	CNumerButton m_btnDanWan;
	CNumerButton m_btnShuangWan;
	CNumerButton m_btnDaQian;
	CNumerButton m_btnXiaoQian;
	CNumerButton m_btnDanQian;
	CNumerButton m_btnShuangQian;
	CNumerButton m_btnDaBai;
	CNumerButton m_btnXiaoBai;
	CNumerButton m_btnDanBai;
	CNumerButton m_btnShuangBai;
	CNumerButton m_btnDaShi;
	CNumerButton m_btnXiaoShi;
	CNumerButton m_btnDanShi;
	CNumerButton m_btnShuangShi;
	CNumerButton m_btnDaGe;
	CNumerButton m_btnXiaoGe;
	CNumerButton m_btnDanGe;
	CNumerButton m_btnShuangGe;

	static CString m_strHaoma;

	//����С�Ŀ�����ʱʹ�õ�����
	CFont m_staticFont;
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
	afx_msg void OnRichEditCopy();
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	CChqSSCRule m_chqRule;
	CJxSSCRule m_jxRule;
	CXJSSCRule m_xjRule;
	CFenFenCaiRule m_ffcRule;
	CTXFenFenCaiRule m_txffcRule;
	CQQFenFenCaiRule m_qqffcRule;
	CErFenCaiRule m_efcRule;
	CWuFenCaiRule m_wfcRule;
	CHgydwfcRule	m_hgRule;
	CCanadaRule	m_jndRule;
	CKuaiLe8RUle m_bj5fcRule;				//ʱ��ؼ�
	int GetTXQihaoCha();
public:
	VOID AddOpenResult(CMD_GP_QueryLotResult* pQueryLotResult);
	//���غ���
public:
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	void	GetSubStringA(CStringA& strInput, int nPos, int nLen, CStringA& strOutput);
	CString GetLongHu();
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	bool							m_bLogonSuccess;
	int GetSSCZhushu(int nTypeID,int nKindID);
	VOID SetLogonSuccess(bool bSuccess)
	{
		m_bLogonSuccess = bSuccess;
	}
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
	bool						m_bGetMoreRecord;				//�����¼
	float						m_fBonus;						//���õ�
	float						m_fPercent;						//���õ�
	int							m_nZhuihaoCount;				//׷��ע��
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;

	int							m_nTestCount;
	afx_msg void OnBnClickedBtnQian5();
};
