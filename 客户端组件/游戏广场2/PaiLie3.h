#pragma once
#include "afxwin.h"
#include "NumerButton.h"
#include "ImageButton.h"
#include "TextButton.h"
#include "ImgRioButton.h"
#include "afxcmn.h"
#include "ImgStatic.h"
#include "DlgStatus.h"
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "resource.h"
#include "comm.h"
#include "OpenRecord.h"

// CPaiLie3 �Ի���

class CPlazaViewItem;


class CPaiLie3 : public CDialog, public IStatusViewSink
{
	DECLARE_DYNAMIC(CPaiLie3)
// 	typedef enum 
// 	{
// 		InValid,
// 		//SanXingZhiXuan,		//����ֱѡ
// 		enWF_ZhixuanFushi,	
// 		enWF_ZhixuanDanshi,
// 
// 		//SanXingZuXuan,		//������ѡ
// 		enWF_QiansanzusanFushi,
// 		enWF_QiansanzusanDanshi,
// 		enWF_QiansanzuliuFushi,
// 		enWF_QiansanzuliuDanshi,
// 
// 		enWF_QianyizhixuanFushi,		//ǰһֱѡ
// 
// 		//QianErZhiXuan,		//ǰ��ֱѡ
// 		enWF_QianerzhixuanFushi,
// 		enWF_QianerzhixuanDanshi,
// 
// 		enWF_Budingwei,	//���ǲ���λ
// 
// 		enWF_HouyizhixuanFushi,		//��һֱѡ
// 
// 		//HouErZhiXuan,		//���ֱѡ
// 		enWF_HouerzhixuanFushi,
// 		enWF_HouerzhixuanDanshi,
// 
// 		//SanXingZhiXuanHeZhi,//����ֱѡ��ֵ
// 
// 		WanFaCount
// 	}tagEnumTreeNumbers;

	enum {
		Kj_XinXi_Count = 5
	};

public:
	CPaiLie3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPaiLie3();
	virtual BOOL PreTranslateMessage(MSG * pMsg);

// �Ի�������
	enum { IDD = IDD_DLG_PAILIE3 };
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CImageButton					m_btWxzxTip;

public:
	
	void FlushZongjine();
	CString GetScoreMoshi();
	int GetMoshiRet(CString moshi);
	afx_msg void OnRichEditCopy();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int Get3dZhushu(int nTypeID,int nKindID);
	afx_msg void OnBnClickedBtnMoreRecord();

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	DOUBLE	m_zongJine;
	int				m_singlezhushu;
	CTextButton m_btnYuan;
	CTextButton m_btnJiao;
	CTextButton m_btnFen;
	CTextButton m_btnLi;
	afx_msg void OnBnClickedBtnYuan();
	afx_msg void OnBnClickedBtnJiao();
	afx_msg void OnBnClickedBtnFen();
	afx_msg void OnBnClickedBtnLi();

	afx_msg void OnBnClickedBtn3dSxzhx();
	afx_msg void OnBnClickedBtn3dSxzx();
	afx_msg void OnBnClickedBtn3dQyzhx();
	afx_msg void OnBnClickedBtn3dQezhx();
	afx_msg void OnBnClickedBtn3dSxbdw();
	afx_msg void OnBnClickedBtn3dHyzhx();
	afx_msg void OnBnClickedBtn3dHezhx();
	afx_msg void OnBnClickedBtn3dDwd();
	afx_msg void OnBnClickedBtn3dDxds();
	afx_msg void OnBnClickedBtnBai3dQuan();
	afx_msg void OnBnClickedBtnBai3dDa();
	afx_msg void OnBnClickedBtnBai3dXiao();
	afx_msg void OnBnClickedBtnBai3dDan();
	afx_msg void OnBnClickedBtnBai3dShuang();
	afx_msg void OnBnClickedBtnBai3dQing();
	afx_msg void OnBnClickedBtnShi3dQuan();
	afx_msg void OnBnClickedBtnShi3dDa();
	afx_msg void OnBnClickedBtnShi3dXiao();
	afx_msg void OnBnClickedBtnShi3dDan();
	afx_msg void OnBnClickedBtnShi3dShuang();
	afx_msg void OnBnClickedBtnShi3dQing();
	afx_msg void OnBnClickedBtnGe3dQuan();
	afx_msg void OnBnClickedBtnGe3dDa();
	afx_msg void OnBnClickedBtnGe3dXiao();
	afx_msg void OnBnClickedBtnGe3dDan();
	afx_msg void OnBnClickedBtnGe3dShuang();
	afx_msg void OnBnClickedBtnGe3dQing();

	afx_msg void OnBnClickedRioSxzhxFushi();
	afx_msg void OnBnClickedRioSxzhxDanshi();
	afx_msg void OnBnClickedRioHezhxFushi();
	afx_msg void OnBnClickedRioHezhxDanshi();
	afx_msg void OnBnClickedRioSxzsFushi();
	afx_msg void OnBnClickedRioSxzsDanshi();
	afx_msg void OnBnClickedRioSxzlFushi();
	afx_msg void OnBnClickedRioSxzlDanshi();
	afx_msg void OnBnClickedRioQyzhx();
	afx_msg void OnBnClickedRioQezhxFushi();
	afx_msg void OnBnClickedRioQezhxDanshi();
	afx_msg void OnBnClickedRioSxbdw();
	afx_msg void OnBnClickedRioHyzhx();
	afx_msg void OnBnClickedRioSxzhxHezhi();
	afx_msg void OnBnClickedRioDwd();
	afx_msg void OnBnClickedRioDwDxds();

	afx_msg void OnBnClickedBtn3dAdd();
	afx_msg void OnBnClickedBtnDelSel();
	afx_msg void OnBnClickedBtnTouzhu();
	afx_msg void OnBnClickedBtnZhuihao();
	afx_msg void OnEnChangeRichEditDanshiInput();
	int GetDanShiZhusu(int nKind);
	afx_msg void OnBnClickedBtnTouzhuOneKey();
	void OnBnClickedBtnTouzhuQ();

	afx_msg void OnEnChangeEditDanshiInput();
	afx_msg void OnEnChangeEditBeishu();
public:
	afx_msg void OnBnClickedBtnLock();
	afx_msg void OnBnClickedBeiTouJia();
	afx_msg void OnBnClickedBeiTouJian();
	CImageButton m_btnBeiTouJia;
	CImageButton m_btnBeiTouJian;
	CImageButton m_btnMoreRecord;

	afx_msg void OnBnClickedBtnClsList();
	DECLARE_MESSAGE_MAP()
public:
	//�������˷���ǰN����������
	bool GetTopLuckyNumber(CMD_GP_QueryLotResult* pResult, int	nDataSize);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	COpenRecord						m_DlgOpenRecord;					//���࿪����¼

private:
	void CheckNumBtn(CNumerButton& btn);
	void UnCheckNumBtn(CNumerButton& btn);
	bool IsNumBtnCheck(CNumerButton& btn);

	void HideCtrl(int ctrl_id);
	void ShowCtrl(int ctrl_id);

	void HideAllRio();

	void AdjustWanFa();
	void AdjustRadio();
	void AdjustNumBtn();
	void AdjustNumView();
	void AdjustDanShi();
	void AdjustAdd();

	void CheckWanFaBtn(CTextButton& btn);

	void InitWanFaBtns();
	void InitNumberBtns();
	void InitListCtrl();

	void HideBaiNums();
	void HideShiNums();
	void HideGeNums();
	void ShowBaiNums();
	void ShowShiNums();
	void ShowGeNums();

	//��ʾ������ѡ�Ͳ���λ
	void ShowZuXuan();
	void ShowBuDingWei();

	//��ʾ���ص�ʽ
	void HideDanShiEdit();
	void ShowDanShiEdit();

	void ResetAllNums();

	CString GetBaiString();
	CString GetShiString();
	CString GetGeString();

	CString GetWanDesc();
	tagEnumTreeNumbers GetWanFaByDesc(const CString& desc);

	void DrawLastKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawTopKjHaoma(CDC* pDC, Graphics& graphics);
	void DrawBigNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawSmallNum(Graphics& graphics, Bitmap* img, Rect& rc_dest, int num);
	void DrawZongZhushuJinE(CDC* pDC);

	bool LuckyNumCQSSC();
public:
	CString GetNextExpect();
	//���ڿ���ʱ��
	CTime GetNextKjShj();
	//���´ο���ʱ�仹ʣ�µ�ʱ��
	long GetKjShjDiff();
	CString GetKjShjDiffDesc();

	void SetLockText();

	bool GetZuSanHaoma(CStringArray& strArr);
	bool GetZuSanHaoma(CString& haoma, int& zhushu);
	bool IsZuSanHaoma(const CString& str);

	bool GetDanShiHaoma(CString& haoma, int size, int& zhushu);
	bool GetHaomaStrs(CStringArray& strArr, CString line, int size);

	void InitWanFaMoShi();

private:
	Bitmap* m_bmpBk;
	Bitmap* m_numImageList;
	Bitmap* m_kjNumBig;
	Bitmap* m_kjNumSmall;
	Bitmap* m_bmpNumTip;

	tagEnumTreeNumbers	m_gameKind;
	static CString m_strHaoma;

	CNumerButton m_btn3DBai0;
	CNumerButton m_btn3DBai1;
	CNumerButton m_btn3DBai2;
	CNumerButton m_btn3DBai3;
	CNumerButton m_btn3DBai4;
	CNumerButton m_btn3DBai5;
	CNumerButton m_btn3DBai6;
	CNumerButton m_btn3DBai7;
	CNumerButton m_btn3DBai8;
	CNumerButton m_btn3DBai9;

	CNumerButton m_btn3DShi0;
	CNumerButton m_btn3DShi1;
	CNumerButton m_btn3DShi2;
	CNumerButton m_btn3DShi3;
	CNumerButton m_btn3DShi4;
	CNumerButton m_btn3DShi5;
	CNumerButton m_btn3DShi6;
	CNumerButton m_btn3DShi7;
	CNumerButton m_btn3DShi8;
	CNumerButton m_btn3DShi9;

	CNumerButton m_btn3DGe0;
	CNumerButton m_btn3DGe1;
	CNumerButton m_btn3DGe2;
	CNumerButton m_btn3DGe3;
	CNumerButton m_btn3DGe4;
	CNumerButton m_btn3DGe5;
	CNumerButton m_btn3DGe6;
	CNumerButton m_btn3DGe7;
	CNumerButton m_btn3DGe8;
	CNumerButton m_btn3DGe9;

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


	CTextButton m_btnSanXingZhiXuan;
	CTextButton m_btnSanXingZuXuan;
	CTextButton m_btnQianYiZhiXuan;
	CTextButton m_btnQianErZhiXuan;
	CTextButton m_btnSanXingBuDingWei;
	CTextButton m_btnHouYiZhiXuan;
	CTextButton m_btnHouErZhiXuan;
	CTextButton m_btnZhiXuanHeZhi;
	CTextButton m_btnDingWeiDan;
	CTextButton m_btnDaXiaoDanShuang;

	CTextButton m_btn3DBaiQuan;
	CTextButton m_btn3DBaiDa;
	CTextButton m_btn3DBaiXiao;
	CTextButton m_btn3DBaiDan;
	CTextButton m_btn3DBaiShuang;
	CTextButton m_btn3DBaiQing;
	CTextButton m_btn3DShiQuan;
	CTextButton m_btn3DShiDa;
	CTextButton m_btn3DShiXiao;
	CTextButton m_btn3DShiDan;
	CTextButton m_btn3DShiShuang;
	CTextButton m_btn3DShiQing;
	CTextButton m_btn3DGeQuan;
	CTextButton m_btn3DGeDa;
	CTextButton m_btn3DGeXiao;
	CTextButton m_btn3DGeDan;
	CTextButton m_btn3DGeShuang;
	CTextButton m_btn3DGeQing;

	CImgStatic m_staticBai;
	CImgStatic m_staticShi;
	CImgStatic m_staticGe;
	CImgStatic m_staticZuXuan;
	CImgStatic m_staticBuDingWei;

	CEdit					m_editDanShiHaoMa;
	CRichEditCtrl m_richDanshiHaoma;

	CImageButton m_btn3DAdd;
	CTextButton m_btnLock;
	CImageButton m_btnDelSel;
	CImageButton m_btnClsList;

	CListCtrlCl m_list3D;
	int m_zongZhuShu;	//������ע��

	CSkinEditEx m_editBeiShu;

	CImageButton m_btnTouZhu;
	CImageButton m_btnZhuiHao;
	CImageButton m_btnTouZhuOnekey;

	CImgRioButton m_rioSxzhxFuShi;
	CImgRioButton m_rioSxzhxDanShi;
	CImgRioButton m_rioHeZhxFuShi;
	CImgRioButton m_rioHeZhxDanShi;
	CImgRioButton m_rioSxzsFuShi;
	CImgRioButton m_rioSxzsDanShi;
	CImgRioButton m_rioSxzlFuShi;
	CImgRioButton m_rioSxzlDanShi;
	CImgRioButton m_rioSxzhxHeZhi;
	CImgRioButton m_rioQyZhx;
	CImgRioButton m_rioQeZhxFuShi;
	CImgRioButton m_rioQeZhxDanShi;
	CImgRioButton m_rioSxbdw;
	CImgRioButton m_rioHyzhx;
	CImgRioButton m_rioDwd;
	CImgRioButton m_rioDwDxds;

	UINT m_beishu;

	int m_bigNumWidth;
	int m_bigNumHeight;
	int m_smallNumWidth;
	int m_smallNumHeight;

	CMarkup m_xml;
	TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	int	m_kjXinxiCont;
	KjXinXi m_kjXinxi[Kj_XinXi_Count];

	//����С�Ŀ�����ʱʹ�õ�����
	CFont m_smallNumfont;
	//�ܽ����ע��ʹ�õ�����
	CFont m_zongFont;
	//������Ϣʹ������
	CFont m_KjInfoFont;
	bool	m_bKaiJiangzhong;		//���ڿ���
	Bitmap*	m_imgKaijiang;			//��������
	int				m_nFrameIndex;				//GIF�����ĵ�ǰ֡��
	int				m_nFrameCount;				//GIF�����ܹ�֡��

	CFont m_KjInfoFont1;
	int							m_nTzhSign;
	int							m_nTzhSign1;
	bool							m_bShowFail;						//��ʾʧ�ܶ���
	float	nPercent;
	float	fDanweiFandian;
	float	nBonus ;

	float						m_fBonus;						//���õ�
	float						m_fPercent;						//���õ�
	TCHAR				m_szMD5Haoma[33];
	int							m_nZhuihaoCount;				//׷��ע��
	bool							m_bZhuiZjtz;						//�Ƿ��н�ͣ׷
	CDlgStatus						m_DlgStatus;						//״̬����
	//ȡ������
	virtual VOID OnStatusCancel();

	VOID SendToServer(int nSendType);
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	CString GetBaiDxdshString(); 
	CString GetShiDxdshString(); 
	CString GetGeDxdshString();

	void ShowDaXiaoDanShuang();
	void HideDaXiaoDanShuang();
	Bitmap* m_bmpDxdsh;
	CString GetDxdshStringDesc(int nNum);

	BYTE m_cbIfTs;
	int								m_nStartQihao;
	CTime							m_tStartTime;

};
