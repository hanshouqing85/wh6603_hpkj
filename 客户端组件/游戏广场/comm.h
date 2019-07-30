#pragma once


//������Ϣ
#define				ID_GFX_SMALLICON                50000
#define				ID_GFX_LARGEICON                50001
#define				ID_GFX_REMOVEITEM               50002
#define				ID_GFX_RENAMEITEM               50003
#define				ID_GFX_GROUPICON                50004
#define				ID_GFX_FONTCICON                50005
#define				ID_GFX_BACKCICON                50006



//�˻�����
#define		ACCOUNT_LENGTH	21
//���볤��
#define		PASSWORD_LENGTH	21
//IP��ַ����
#define		IP_LENGTH	11
//ʱ�䳤��
#define		TIME_LENGTH	51
//QQ���볤��
#define		QQ_LENGTH	12
//������󳤶�
#define		HAOMAO_LENGTH 60

//������Ϣ
#define	KJ_QIHAO_LENGTH	30
#define	KJ_HAOMA_LENGTH	45
#define	KJ_SHIJIAN_LENGTH	30

#define	TOP_KAIJIANG_NUMBERS	5


//��Ʊ����
typedef enum {
	CZChongQingSSC=1,		//����ʱʱ��
	CZJiangXiSSC=2,			//����ʱʱ��
	CZ3D=3,					//����3D
	CZXinJiangSSC = 4,		//�½�ʱʱ��
	CZPaiLie3,			//����3
	CZGD11Xuan5,		//�㶫11ѡ5
	CZCQ11Xuan5,		//����11ѡ5
	CZSD11Xuan5,		//ɽ��11ѡ5
	CZJX11Xuan5,		//����11ѡ5
	CZKUAILE8,				//��������8
	CZ_FENFEN_CAI,			//�ֲַʣ�5���ӿ�����24Сʱ��ͣ��ÿ��288��
	CZ_WUFEN_CAI,			//5�ֲ�
	CZ_BJKUAICHE,			//�����쳵
	CZ_PK10=13,		//PK10
	CZ_LIUHECAI = 14,
	CZ_QiXingCai = 15,			//���ǲ�--lly
	CZHLJ11Xuan5=16,		//������11ѡ5
	CZXingYun28,
	CZ_HGYDWFC=20,
	CZ_TianJinSSC = 23,	//���ʱʱ��
	CZCount
}CaiZhong;

//ʱʱ���淨����
typedef enum 
{
	inValid,
	//QianSanZhiXuan,			//ǰ��ֱѡ
	QianSanZhiXuan_FuShi=1,		//ǰ��ֱѡ��ʽ��Ҳ������ǰ��
	QianSanZhiXuan_DanShi,		//

	//HouSanZhiXuan,			//����ֱѡ
	HouSanZhiXuan_FuShi,		//����ֱѡ��ʽ��Ҳ�����Ǻ���
	HouSanZhiXuan_DanShi,

	//QianSanZuXuan,			//ǰ����ѡ
	QianSanZuXuan_ZuSanFuShi,
	QianSanZuXuan_ZuSanDanShi,
	QianSanZuXuan_ZuLiuFuShi,
	QianSanZuXuan_ZuLiuDanShi,

	//HouSanZuXuan,				//������ѡ
	HouSanZuXuan_ZuSanFuShi,
	HouSanZuXuan_ZuSanDanShi,
	HouSanZuXuan_ZuLiuFuShi,
	HouSanZuXuan_ZuLiuDanShi,

	//WuXingQianEr,				//����ǰ��
	WuXingQianEr_FuShi,
	WuXingQianEr_DanShi,

	//WuXingHouEr,				//���Ǻ��
	WuXingHouEr_FuShi,
	WuXingHouEr_DanShi,

	//QianErZuXuan,				//ǰ����ѡ
	QianErZuXuan_FuShi,
	QianErZuXuan_DanShi,

	//HouErZuXuan,				//�����ѡ
	HouErZuXuan_FuShi,
	HouErZuXuan_DanShi,

	DaXiaoDanShuang,			//��С��˫

	//BuDingWei,				//����λ
	BuDingWei_QianSan,
	BuDingWei_HouSan,

	DingWeiDan,					//��λ��
		
	RenXuan2,					//��ѡ2,3,4
	RenXuan3,
	RenXuan4,
	
	QianSanHunhe,				//ǰ�����
	ZhongSanHunhe,				//�������
	HouSanHunhe,				//�������
	
	//����ֱѡ��ʽ�淨
	WuxingZhixuan,				//����ֱѡ��0209������
	WuxingQianSi,				//����ǰ��
	WuxingHousi,				//���Ǻ���
	WuxingZhongsan,				//��������������ǰ�������Ǻ�������һ�ţ�

	//������ѡ�淨
	ZhongsanZusan,				//����������0209������
	ZhongsanZuliu,				//��������
	
	//������ѡ�淨
	Zuxuan120,					//��ѡ120
	Zuxuan60,					//��ѡ60
	Zuxuan30,					//��ѡ30
	Zuxuan20,					//��ѡ20
	Zuxuan10,					//��ѡ10
	Zuxuan5,					//��ѡ5
								
	//������ֵ�淨
	QianSanZhiXuanHeZhi,		//ǰ��ֱѡ��ֵ
	HouSanZhiXuanHeZhi,			//����ֱѡ��ֵ
	ZhongSanZhiXuanHeZhi,		//����ֱѡ��ֵ

	//��������λ
	BuDingWei_QianEr,			//����λ��ǰ2
	BuDingWei_HouEr,			//����λ����2

	//����Ȥζ�淨
	QW_YiFanFengShun,			//Ȥζ��һ����˳��һ��һ��һ�����־��н�
	QW_HaoShiChengShuang,		//Ȥζ�����³�˫��2��һ�������ֲ��н�
	QW_SanXingBaoXi,			//Ȥζ�����Ǳ�ϲ��3��һ�������ֲ��н�
	QW_SiJiFaCai,				//Ȥζ���ļ����ƣ�4��һ�������ֲ��н�
	NiuNiu_Num,					//ţţ��5������֮����10�ı�����ţ1--ţ10��ţţ�� 1:10����ţͨ�ԣ�
	NiuNiu_None,				//ţţ����ţ 1:2
	NiuNiu_DxDs,				//ţţ����С��˫ 1:2.5
	DaXiaoDanShuang_dw,			//��С��˫��λ
	Zonghe_DXDSLh=56,			//�ܺʹ�С��˫���ܺ�ֵ��춵��23���С춵��22��С��
	SSC_LongHu,					//������ͷ����βΪ����β����ͷΪ������ ��
	QianSan_Quwei,				//ǰ������˳�Ӷ��Ӱ�˳����
	ZhongSan_Quwei,				//��������˳�Ӷ��Ӱ�˳����
	HouSan_Quwei,				//��������˳�Ӷ��Ӱ�˳����
	HouErZhixuan_Hezhi=61,		//���ֱѡ��ֵ
	QianErZhixuan_Hezhi,		//ǰ��ֱѡ��ֵ
	RenSanZhixuan_Hezhi,		//����ֱѡ��ֵ	0
	RenErZhixuan_Hezhi,			//�ζ�ֱѡ��ֵ	0
	SSC_LongHuYaJun=65,			//�������Ǿ�
	Ren3Zu3_Fushi,				//��3��3��ʽ	0
	Ren3Zu6_Fushi,				//��3��6��ʽ	0
	Ren3Hunhe_Zuxuan,			//��3��� ��ѡ	0
	Ren2Zuxuan_Fushi,			//��2��ѡ ��ʽ	0
	Ren2Zuxuan_Danshi,			//��2��ѡ ��ʽ	0
	BuDingWei_ZhongSan,
	WanFaCount
}SSCWanFaKind;

typedef enum {
	
	QiXingCai_LiangDing=1,
	QiXingCai_SanDing,
	QiXingCai_SiDing,
	QiXingCai_YiDing,
	QiXingCai_LiangZiXian,
	QiXingCai_SanZiXian,
}QiXingCaiKind;

//ͨ��Ͷע����ת��������£�
#define		NSmallNumber		1				//С
#define		NSingleNumber		3				//��
#define		NDoubleNumber		2				//˫
#define		NTigerNumber		4				//��
#define		NPingNumber			5				//��=ƽ
#define		NDragonNumber		6				//��
#define		NHalfSerialNumber	7				//��˳ ��540 790 706
#define		NSimpleNumber		8				//�������ų����ӣ�˳�ӣ����ӣ���˳ ��Ϊ�н�
#define		NBigNumber			9				//��
#define		NSmallSingleNumber	10				//С��
#define		NSmallDoubleNumber	11				//С˫
#define		NBigSingleNumber	12				//��
#define		NBigDoubleNumber	13				//��˫
#define		NCoupleNumber		14				//����
#define		NSerialNumber		15				//˳�ӣ�һ������
#define		NAllSame			16				//���ӣ�ȫ��ͬ��
#define		NLimitSmall			17				//��ֵС,0-5
#define		NLimitBig			18				//��ֵ��22-27
//����10��
#define		NSumDigitBig		19				//��β���ܺ͸�λ����
#define		NSumDigitSmall		20				//��βС���ܺ͸�λ��С��
#define		NHeshuDa			21				//���������еĸ�λ��ʮλ�������֮�ͣ�
#define		NHeshuXiao			22				//����С�����еĸ�λ��ʮλ�������֮�ͣ�
#define		NZhongNumber		23				//�У��_��̖֮�a��01��02��03��04��05��06��07
#define		NFacaiNumber		24				//�l���_��̖֮�a��08��09��10��11��12��13��14
#define		NBaiNumber			25				//�ף��_��̖֮�a��15��16��17��18��19��20
#define		NEastNumber			26				//�|���_����̖�a��01��05��09��13��17
#define		NSouthNumber		27				//�ϣ��_����̖�a��02��06��10��14��18
#define		NWestNumber			28				//�����_����̖�a��03��07��11��15��19
#define		NNorthNumber		29				//�����_����̖�a��04��08��12��16��20
//����8
#define		NFrontMore			30				//ǰ�ࣺǰ�P̖�a��01-40������P̖�a�ǣ�41~80���������r
#define		NBackMore			31				//��ࣺ��P̖�a��41-80����ǰ�P̖�a�ǣ�01~40��������
#define		NEqualNumber		32				//ǰ��ͣ�ǰ�̺�������̺��������ͬ
#define		NSingleMore			33				//���ࣺ�_����20��̖�a�У�����̖�a��˫��̖�a������
#define		NDoubleMore			34				//˫�ࣺ�_����20��̖�a�У�˫��̖�a�ȵ���̖�a������
#define		NEqualDigits		35				//��˫����ͬ
//�_����20��̖�a��ӵ��ܺ�ֵ����5���Σ��Խ�ľ��ˮ������������
//��210��695����ľ��696��763����ˮ��764��855������856��923��������924��1410��
#define		NSumGold			36				//��
#define		NSumWood			37				//ľ
#define		NSumWater			38				//ˮ
#define		NSumFire			39				//��
#define		NSumEarth			40				//��


//������8
typedef enum {
	enBJK8_RX_1=1,
	enBJK8_RX_2,
	enBJK8_RX_3,
	enBJK8_RX_4,
	enBJK8_RX_5,
	enBJK8_RX_6,
	enBJK8_RX_7,
	enBJK8_RX_8,
	enBJK8_RX_9,
	enBJK8_RX_10,

// 	enXY28_DXDS,	//��ֵ��С��˫
// 	enXY28_Funny,		//Ȥζ
// 	enXY28_JiZhi,		//��ֵ
// 	enXY28_TeMa=14,	//����

	enBJK8_ShangXia=15,
	enBJK8_JiOu=16,
	enBJK8_HeZhiDXDS=17,
}BeiJingKuai8Kind;

//����28
typedef enum {
	enBJK8_1=1,
	enBJK8_2,
	enBJK8_3,
	enBJK8_4,
	enBJK8_5,
	enBJK8_6,
	enBJK8_7,
	enBJK8_8,
	enBJK8_9,
	enBJK8_10,

	enXY28_DXDS,	//��ֵ��С��˫
	enXY28_Funny,		//Ȥζ
	enXY28_JiZhi,		//��ֵ
	enXY28_TeMa=14,	//����

}XingYun28Kind;

//11ѡ5���淨
typedef enum tagEnumIIXuan5
{
		IIinValid,

		IIRenXuan2,						//��ѡ��

		IIRenXuan3,						//��ѡ��
		IIRenXuan4,						//��ѡ��
		IIRenXuan5,						//��ѡ��

		IIRenXuan6,						//��ѡ��

		IIRenXuan7,						//��ѡ��

		IIRenXuan8,						//��ѡ��
		QianYi,							//ǰһ


		QianEr_ZhiXuan,					//ǰ�� ֱѡѡ��
		QianEr_ZuXuan,					//ǰ����ѡѡ��


		QianSan_ZhiXuan,				//ǰ��ֱѡѡ��
		QianSan_ZuXuan,					//ǰ����ѡѡ��


		IIWanFaCount
}IIXuan5GameKind;

typedef enum 
{
	PK10inValid,

	WF_GuanJun,
	WF_GuanYaJun,
	WF_YaJun,
	WF_QianSan,
	WF_DiSan,
	WF_QianSi,
	WF_DiSi,
	WF_QianWu,
	WF_DiWu,
	WF_QianLiu,
	WF_DiLiu,
	WF_QianQi,
	WF_DiQi,
	WF_QianBa,
	WF_DiBa,
	WF_QianJiu,
	WF_DiJiu,
	WF_QianShi,
	WF_DiShi,
	WF_DXDS,	//��С��˫
	WF_DWD,
	WF_GYHZH,	//���Ǻ�ֵ
	PK10WanFaCount
}PK10GameKind;


//3D�����������淨��������3��
typedef	enum	
{
	Invalid,
	enWF_ZhixuanFushi,					//����ֱѡ��ʽ,
	enWF_ZhixuanDanshi,					//����ֱѡ��ʽ,
	enWF_QiansanzusanFushi,				//ǰ��������ʽ,
	enWF_QiansanzusanDanshi,			//ǰ��������ʽ,
	enWF_QiansanzuliuFushi,				//ǰ��������ʽ,
	enWF_QiansanzuliuDanshi,			//ǰ��������ʽ,
	enWF_QianyizhixuanFushi,			//ǰһֱѡ��ʽ,
	enWF_QianerzhixuanFushi,			//ǰ��ֱѡ��ʽ,
	enWF_QianerzhixuanDanshi,			//ǰ��ֱѡ��ʽ,
	enWF_Budingwei,						//���ǲ���λ,
	enWF_HouyizhixuanFushi,				//��һֱѡ��ʽ,
	enWF_HouerzhixuanFushi,				//���ֱѡ��ʽ,
	enWF_HouerzhixuanDanshi,			//���ֱѡ��ʽ,
	enWF_ZhixuanHezhi,					//����ֱѡ��ֵ,
	enWF_DaxiaoDanshuang,				//��С��˫
	enWF_DingWeiDan,					//��λ��
	enWF_Count							//�淨����
}tagEnumTreeNumbers;

// 	//ʱʱ���淨����
typedef enum {
	LiuHeCai_Tmdm=1,
	LiuHeCai_Tmds,
	LiuHeCai_Tmdx,
	LiuHeCai_Tmsx,
	LiuHeCai_Tmbs,
	LiuHeCai_Pmdm,
	LiuHeCai_Zm1t,
	LiuHeCai_Zm2t,
	LiuHeCai_Zm3t,
	LiuHeCai_Zm4t,
	LiuHeCai_Zm5t,
	LiuHeCai_Zm6t
}LiuHeCaiKind;

//�����û���Ϣ-��¼������������ʾ
typedef struct _BaseUser{
	int user_id;
	BYTE cbgender;
	TCHAR account[ACCOUNT_LENGTH];
	int type;
	int Scoretype;	//1Ԫ   2��  3��
	BYTE moormachine;
	double	fandian;
	double yue;
	double dongjie;
}BaseUser;

//�ҵ��˻�-�û���Ϣҳ����ʾ
typedef struct _WDZHUserInfo{
	int user_id;
	SYSTEMTIME	t_reg_time;
	TCHAR account[ACCOUNT_LENGTH];
	double fandian;
	TCHAR	last_ip[IP_LENGTH];
	TCHAR	last_time[TIME_LENGTH];
	TCHAR current_ip[IP_LENGTH];
	TCHAR	qq[QQ_LENGTH];
	TCHAR	shangji_qq[QQ_LENGTH];
	double	yongjin;
	double yongjin_ze;
	double	kuisunyongjin;
	double kuisunyongjin_ze;
	double fenhong;
	double	fenhong_ze;
	double qipai_ye;
	//double	yingkui_ze;
	SYSTEMTIME	t_fh_time;
	SYSTEMTIME	t_ksyj_time;
	SYSTEMTIME t_yj_time;
}WDZHUserInfo;

//DingDan
typedef struct _TouZhuOrder{
	TCHAR haomao[60];
	int zhushu;
	int beishu;
	int jine;
}TouZhuOrder;

typedef struct _KjXinXi {
	TCHAR qihao[KJ_QIHAO_LENGTH];
	TCHAR haoma[KJ_HAOMA_LENGTH];
	TCHAR shijian[KJ_SHIJIAN_LENGTH];
}KjXinXi;

typedef struct _topuser {
	int t_id;
	int user_id;
	int gametype;
	int gamekind;
	double jiangjin;
}TopUser;

typedef struct _zhongjiang {
	int t_type;
	CString t_qishu;
	int zhushu;
}Zhongjiang;



#ifdef	_DEBUG

	#define		OUT_TEXT	OutputDebugString

#else
	#define	OUT_TEXT	
#endif

// class LogFile
// {
// public:
// 	LogFile();
// 	~LogFile();
// 
// 	int		LogText(LPCTSTR lpszFormat,...);
// 	int		LogText(CString& strLog);
// 
// 	
// 	TCHAR*			GetWorkFolder();
// 	static	LogFile &instance();
// private:
// 
// 	TCHAR	m_szPath[255];
// 
// };

//MD5 ������ 
class  CMD5Encrypt
{
	//��������
private:
	//���캯��
	CMD5Encrypt() {}

	//���ܺ���
public:
	//��������
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
};
