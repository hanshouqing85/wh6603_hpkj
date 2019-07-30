#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//���ڲ����ת����������ƿ��Դ����������ʹ�ã�����Ϊ����������������ΪMAX_INDEX. ע:����滻����������,������.
#define	INDEX_REPLACE_CARD					MAX_INDEX

//////////////////////////////////////////////////////////////////////////
//�߼�����

#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//��������

//������־
#define WIK_NULL					0x00								//û������
#define WIK_LEFT					0x01								//�������
#define WIK_CENTER					0x02								//�г�����
#define WIK_RIGHT					0x04								//�ҳ�����
#define WIK_PENG					0x08								//��������
#define WIK_GANG					0x10								//��������
#define WIK_LISTEN					0x20								//��������
#define WIK_CHI_HU					0x40								//�Ժ�����
#define WIK_REPLACE					0x80								//�����滻

//////////////////////////////////////////////////////////////////////////
//���ƶ���

/*
//	1��
*/
#define CHR_ZI_MO					0x00000001									//����
#define CHR_DAN_DIAO_JIANG			0x00000002									//������
#define CHR_KAN_ZHANG				0x00000004									//����
#define CHR_BIAN_ZHANG				0x00000008									//����
#define CHR_WU_ZI					0x00000010									//����
#define CHR_QUE_YI_MEN				0x00000020									//ȱһ��
#define CHR_MING_GANG				0x00000040									//����
#define CHR_YAO_JIU					0x00000080									//�۾ſ�
#define CHR_LAO_SHAO_JIANG			0x00000100									//���ٸ�
#define CHR_LIAN_LIU				0x00000200									//����
#define CHR_XI_XIANG_FENG			0x00000400									//ϲ���
#define CHR_YI_BAN_GAO				0x00000800									//һ���

/*
//	2��
*/
#define CHR_DUAN_YAO				0x00001000									//����
#define CHR_AN_GANG					0x00002000									//����
#define CHR_SHUANG_AN_KE			0x00004000									//˫����
#define CHR_SHUANG_TONG_KE			0x00008000									//˫ͬ��
#define CHR_SHI_GUI_YI				0x00010000									//�Ĺ�һ
#define CHR_PING_HU					0x00020000									//ƽ��
#define CHR_MEN_QI_QING				0x00040000									//��ǰ��
#define CHR_MEN_FENG_KE				0x00080000									//�ŷ��
#define CHR_QUAN_FENG_KE			0x00100000									//Ȧ���
#define CHR_JIAN_KE					0x00200000									//����

/*
//	4��
*/
#define CHR_JUE_ZHANG				0x00400000									//����
#define CHR_SHUANG_MING_GANG		0x00800000									//˫����
#define CHR_BU_QIU_REN				0x01000000									//������
#define CHR_QUAN_DAI_YAO			0x02000000									//ȫ����

/*
//	6��
*/
#define CHR_SHUANG_JIAN_KE			0x04000000									//˫����
#define CHR_SHUANG_AN_GANG			0x08000000									//˫����
#define CHR_QUAN_QIU_REN			0x10000001									//ȫ����
#define CHR_WU_MEN_QI				0x10000002									//������
#define CHR_SAN_SE_BU				0x10000004									//��ɫ������
#define CHR_HUN_YI_SE				0x10000008									//��һɫ
#define CHR_PENG_PENG				0x10000010									//������

/*
//	8��
*/
#define CHR_QIANG_GANG				0x10000020									//���ܺ�
#define CHR_GANG_KAI				0x10000040									//���Ͽ���
#define CHR_HAI_DI					0x10000080									//��������
#define CHR_MIAO_SHOU				0x10000100									//���ֻش�
#define CHR_WU_FAN					0x10000200									//�޷���
#define CHR_SAN_SE_JIE				0x10000400									//��ɫ���ڸ�
#define CHR_SAN_SE_TONG				0x10000800									//��ɫ��ͬ˳ 
#define CHR_TUI_BU_DAO				0x10001000									//�Ʋ���
#define CHR_HUA_LONG				0x10002000									//����

/*
//	12��
*/
#define CHR_SAN_FENG_KE				0x10004000									//�����
#define CHR_XIAO_WU					0x10008000									//С����
#define CHR_DA_WU					0x10010000									//������
#define CHR_ZU_HE_LONG				0x10020000									//�����
#define CHR_QUAN_BU_KAO				0x10040000									//ȫ����

/*
//	16��
*/
#define CHR_SAN_AN_KE				0x10080000									//������
#define CHR_SAN_TONG_KE				0x10100000									//��ͬ��
#define CHR_QUAN_DAI_WU				0x10200000									//ȫ����
#define CHR_YI_SE_BU				0x10400000									//һɫ������
#define CHR_SAN_SE_SHUANG			0x10800000									//��ɫ˫����
#define CHR_QING_LONG				0x11000000									//����

/*
//	24��
*/
#define CHR_QUAN_XIAO				0x12000000									//ȫС
#define CHR_QUAN_ZHONG				0x14000000									//ȫ��
#define CHR_QUAN_DA					0x18000000									//ȫ��
#define CHR_YI_SE_JIE				0x20000001									//һɫ���ڸ�
#define CHR_YI_SE_TONG				0x20000002									//һɫ��ͬ˳
#define CHR_QING_YI_SE				0x20000004									//��һɫ
#define CHR_QUAN_SHUANG				0x20000008									//ȫ˫
#define CHR_QI_XING_BU_KAO			0x20000010									//���ǲ���
#define CHR_QI_DUI					0x20000020									//�߶�

/*
//	32��
*/
#define CHR_HUN_YAO					0x20000040									//���۾�
#define CHR_SAN_GANG				0x20000080									//����
#define CHR_YI_SHI_BU				0x20000100									//һɫ�Ĳ���

/*
//	48��
*/
#define CHR_YI_SHI_JIE				0x20000200									//һɫ�Ľڸ�
#define CHR_YI_SHI_TONG				0x20000400									//һɫ��ͬ˳

/*
//	64��
*/
#define CHR_YI_SE_SHUANG			0x20000800									//һɫ˫����
#define CHR_SI_AN_KE				0x20001000									//�İ���
#define CHR_ZI_YI_SE				0x20002000									//��һɫ
#define CHR_XIAO_SAN_YUAN			0x20004000									//С��Ԫ
#define CHR_XIAO_SHI_XI				0x20008000									//С��ϲ
#define CHR_QING_YAO_JIU			0x20010000									//���۾�

/*
//	88��
*/
#define CHR_SHI_SAN_YAO				0x20020000									//ʮ����
#define CHR_LIAN_QI_DUI				0x20040000									//���߶�
#define CHR_SI_GANG					0x20080000									//�ĸ�
#define CHR_JIU_LIAN_DENG			0x20100000									//��������
#define CHR_LU_YI_SE				0x20200000									//��һɫ
#define CHR_DA_SAN_YUAN				0x20400000									//����Ԫ
#define CHR_DA_SHI_XI				0x20800000									//����ϲ

//////////////////////////////////////////////////////////////////////////

//��������
struct tagKindItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbValidIndex[3];					//ʵ���˿�����
};

//�������
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbCardData[4];						//
};

//���ƽ��
struct tagGangCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_WEAVE];				//�˿�����
};

//��������
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//�����˿�
	bool                            bMagicEye;                          //�����Ƿ�������
	BYTE							cbWeaveKind[MAX_WEAVE];				//�������
	BYTE							cbCenterCard[MAX_WEAVE];			//�����˿�
	BYTE                            cbCardData[MAX_WEAVE][4];           //ʵ���˿�
};

//////////////////////////////////////////////////////////////////////////


#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	Ȩλ�ࡣ
//  ע�⣬�ڲ�����λʱ���ֻ��������Ȩλ.����
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)������������޶���ġ�
//  ֻ�ܵ�������:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//��̬����
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//Ȩλ����
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//���캯��
	CChiHuRight();

	//���������
public:
	//��ֵ��
	CChiHuRight & operator = ( DWORD dwRight );

	//�����
	CChiHuRight & operator &= ( DWORD dwRight );
	//�����
	CChiHuRight & operator |= ( DWORD dwRight );

	//��
	CChiHuRight operator & ( DWORD dwRight );
	CChiHuRight operator & ( DWORD dwRight ) const;

	//��
	CChiHuRight operator | ( DWORD dwRight );
	CChiHuRight operator | ( DWORD dwRight ) const;

	//���
	bool operator == ( DWORD dwRight ) const;
	bool operator == ( const CChiHuRight chr ) const;

	//�����
	bool operator != ( DWORD dwRight ) const;
	bool operator != ( const CChiHuRight chr ) const;

	//���ܺ���
public:
	//�Ƿ�ȨλΪ��
	bool IsEmpty();

	//����ȨλΪ��
	void SetEmpty();

	//��ȡȨλ��ֵ
	BYTE GetRightData( DWORD dwRight[], BYTE cbMaxCount );

	//����Ȩλ��ֵ
	bool SetRightData( const DWORD dwRight[], BYTE cbRightCount );

private:
	//���Ȩλ�Ƿ���ȷ
	bool IsValidRight( DWORD dwRight );
};

//////////////////////////////////////////////////////////////////////////

//����˵��
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//�˿�����
	BYTE							m_cbMagicIndex;						//��������

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ƺ���
public:
	//�����˿�
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//�����˿�
	void RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[]);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//ɾ���˿�
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//���ò���
	void SetMagicIndex( BYTE cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
	//�����ж�
	bool IsMagicCard( BYTE cbCardData );
	//�����ж�
	bool IsHuaCard( BYTE cbCardData );

	//��������
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�˿���Ŀ
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
	//����˿�
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//�ȼ�����
public:
	//�����ȼ�
	BYTE GetUserActionRank(BYTE cbUserAction);
	//���Ƶȼ�
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);

	//�����ж�
public:
	//�����ж�
	BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//�����ж�
	BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//�����ж�
public:
	//���Ʒ���
	BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//�Ժ�����
	BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	//���Ʒ���
	BYTE AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );

	//ת������
public:
	//�˿�ת��
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//�˿�ת��
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//�˿�ת��
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//�˿�ת��
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//��������
protected:
	//������
	bool IsDanDiaoJiang( const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard );
	//����
	bool IsKanZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard );
	//����
	bool IsBianZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard );
	//����
	bool IsWuZi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//ȱһ��
	bool IsQueYiMen( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//����
	bool IsMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//�۾ſ�
	bool IsYaoJiuKe( const tagAnalyseItem *pAnalyseItem );
	//���ٸ�
	bool IsLaoShaoFu( const tagAnalyseItem *pAnalyseItem );
	//����
	bool IsLianLu( const tagAnalyseItem *pAnalyseItem );
	//ϲ���
	bool IsXiXiangFeng( const tagAnalyseItem *pAnalyseItem );
	//һ���
	bool IsYiBanGao( const tagAnalyseItem *pAnalyseItem );
	//����
	bool IsDuanYao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//����
	bool IsAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//˫����
	bool IsShuangAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//˫ͬ��
	bool IsShuangTongKe( const tagAnalyseItem *pAnalyseItem );
	//ƽ��
	bool IsPingHu( const tagAnalyseItem *pAnalyseItem );
	//��ǰ��
	bool IsMenQianQing( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//ȫ����
	bool IsQuanQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//����
	bool IsJianKe( const tagAnalyseItem *pAnalyseItem );
	//˫����
	bool IsShuangMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//������
	bool IsBuQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//ȫ����
	bool IsQuanDaiYao( const tagAnalyseItem *pAnalyseItem );
	//˫����
	bool IsShuangJianKe( const tagAnalyseItem *pAnalyseItem );
	//˫����
	bool IsShuangAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//������
	bool IsWuMenQi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//��ɫ������
	bool IsSanSeBu( const tagAnalyseItem *pAnalyseItem );
	//��һɫ
	bool IsHunYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//������
	bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
	//��ɫ���ڸ�
	bool IsSanSeJie( const tagAnalyseItem *pAnalyseItem );
	//��ɫ��ͬ˳
	bool IsSanSeTong( const tagAnalyseItem *pAnalyseItem );
	//����
	bool IsHuaLong( const tagAnalyseItem *pAnalyseItem );
	//�����
	bool IsSanFengKe( const tagAnalyseItem *pAnalyseItem );
	//С����
	bool IsXiaoWu( const tagAnalyseItem *pAnalyseItem );
	//������
	bool IsDaWu( const tagAnalyseItem *pAnalyseItem );
	//�İ���
	bool IsSiAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//һɫ������
	bool IsYiSeBu(const tagAnalyseItem & AnalyseItem);
	//ȫ��5
	bool IsQuanDaiWu(const tagAnalyseItem & AnalyseItem);
	//��ͬ��
	bool IsSanTongKe(const tagAnalyseItem & AnalyseItem);
	//������
	bool IsSanAnKe(const tagAnalyseItem & AnalyseItem,const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//����ϲ
	bool IsDaSiXi(const tagAnalyseItem & AnalyseItem);
	//����Ԫ
	bool IsDaSanYuan(const tagAnalyseItem & AnalyseItem);
	//�ĸ�
	bool IsSiGang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);
	//���۾�
	bool IsQingYaoJiu(const tagAnalyseItem &AnalyseItem);
	//С��ϲ
	bool IsXiaoSiXi(const tagAnalyseItem & AnalyseItem);
	// С��Ԫ
	bool IsXiaoShanYuan(const tagAnalyseItem & AnalyseItem);
	//��һɫ��
	bool IsZiYiSe(const tagAnalyseItem & AnalyseItem);
	// һɫ˫����
	bool IsYiSeHuangHuiLong(const tagAnalyseItem & AnalyseItem);
	//һɫ��ͬ˳
	bool IsYiSeSiTongShun(const tagAnalyseItem &AnalyseItem);
	//һɫ�Ľڸ�
	bool IsYiSeSiJieGao(const tagAnalyseItem &AnalyseItem);
	//һɫ�Ĳ���
	bool IsYiSeSiBuGao(const tagAnalyseItem &AnalyseItem);
	//����
	bool IsSanGang(const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount);
	//���۾�
	bool IsHunYaoJiu(const tagAnalyseItem &AnalyseItem);
	//��һɫ��
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard);
	//��С����
	bool IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	//ȫ˫
	bool IsQuanShuang(const tagAnalyseItem &AnalyseItem);
	//һɫ��ͬ˳
	bool IsYiSeSanTongShun(const tagAnalyseItem &AnalyseItem);
	//һɫ���ڸ�
	bool IsYiSeSanJieGao(const tagAnalyseItem &AnalyseItem);
	//ȫ��
	bool IsQuanDa(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//ȫ��
	bool IsQuanZhong(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//ȫС
	bool IsQuanXiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//�Ʋ���
	bool IsTuiBuDao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//�����
	bool IsZuHeLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//ȫ����
	bool IsQuanBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//��ɫ˫����
	bool IsSanSeShuangLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//����
	bool IsQingLong( const tagAnalyseItem & AnalyseItem );
	//���ǲ���
	bool IsQiXingBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//ʮ����
	bool IsShiSanYao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//��������
	bool IsJiuLianBaoDeng( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//��һɫ
	bool IsLuYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//�Ĺ�һ
	bool IsSiGuiYi( const tagAnalyseItem *pAnalyseItem, const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//���߶�
	bool IsLianQiDui( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );

	//�ڲ�����
private:
	//�����˿�
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//����,������ֵ����
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
};

//////////////////////////////////////////////////////////////////////////

#endif
