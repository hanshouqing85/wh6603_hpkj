#pragma once


enum {
	ZJD_JINDAN=1,
	ZJD_YINDAN,
	ZJD_TONGDAN
};

//�ҽ�
class CHuoDongZaJinDanDlg : public CDialog,public CMissionItem
{
	DECLARE_DYNAMIC(CHuoDongZaJinDanDlg)

	typedef enum  {
		YinDan_Image_1,
		YinDan_Image_2,
		YinDan_Image_3,
		YinDan_Image_4,
		YinDan_Image_5,
		YinDan_Image_6,

		YinDan_Image_Count,
	} YinDanState;

	typedef enum  {
		JinDan_Image_1,
		JinDan_Image_2,
		JinDan_Image_3,
		JinDan_Image_4,
		JinDan_Image_5,
		JinDan_Image_6,

		JinDan_Image_Count,
	} JinDanState;

	typedef enum  {
		TongDan_Image_1,
		TongDan_Image_2,
		TongDan_Image_3,
		TongDan_Image_4,
		TongDan_Image_5,
		TongDan_Image_6,

		TongDan_Image_Count,
	} TongDanState;

	typedef enum {
		ChuiZi_Image_Up,
		ChuiZi_Image_Mid,
		ChuiZi_Image_Down,

		ChuiZi_Count,
	} ChuiZiState;

public:
	CHuoDongZaJinDanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHuoDongZaJinDanDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HUODONG_ZAJINDAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();

private:
	void DrawJinDanImage(Graphics& graphics);
	void DrawYinDanImage(Graphics& graphics);
	void DrawTongDanImage(Graphics& graphics);

	void Getjiangjin(int dantype);

private:
	Bitmap* m_bmpBk;

	YinDanState m_yindanState;
	JinDanState	m_jindanState;
	TongDanState m_tongdanState;

	ChuiZiState m_yindanChuiziState;
	ChuiZiState m_jindanChuiziState;
	ChuiZiState m_tongdanChuiziState;

	bool	m_yindan_win;
	bool	m_jindan_win;
	bool	m_tongdan_win;

	Bitmap* m_bmpTongDan[YinDan_Image_Count];
	Bitmap* m_bmpYinDan[JinDan_Image_Count];
	Bitmap* m_bmpJinDan[TongDan_Image_Count];

	Bitmap* m_bmpChuiZi[ChuiZi_Count];

	double m_jinrixiaofei;
	bool m_got;
	bool m_bZhuiZi;
	int							m_cbJindanType;
	//CMissionManager					m_MissionManager;					//�������
	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
protected:
	bool							m_bGetZaJinDan;						//��ȡ��־����
	bool							m_bGetLastYue;						//��ȡ��־����
	bool							m_bCanZaJinDan;						//��ȡ��־����ͨ��ID
public:
	VOID SendToServer(int nSendType);
};
