#ifdef WINCONTROL_EXPORTS
#define WINCONTROL_API __declspec(dllexport)
#else
#define WINCONTROL_API __declspec(dllimport)
#endif

#ifndef		_WIN_CONTRL_H__
#define		_WIN_CONTRL_H__

#include <list>



/**
 * ˵����
 * ͳ����ʷ������������ҵ�����Ӯ��
 * �����һ����ʷ����֮ǰ���ǿ�����Ϊ��Ӱ�����ڵ��������׸���
 */

class	WINCONTROL_API	CHistoryPlayerWin            
{
private:
    int							m_iMaxCount;                    // ���ͳ�Ƶľ���
	int							m_iHighLimit;					// �������
	int							m_iLowLimit;					// �������
    std::list< __int64 >		m_arrWinMoney;					// �����Ӯ����ʷ��¼
    std::list< __int64 >		m_arrPay;						// �����Ӯ����ʷ��¼
    __int64						m_nWinMoney;					// ���m_nTurnCount������ܵ���Ӯ
    __int64						m_nPay;						    // ���m_nTurnCount������ܵ�Ͷע
public:
	//���캯��
    CHistoryPlayerWin();

	//��ʼ��
    void Init(int iMaxCount/*���ͳ�Ƶľ���*/, int nHighLimit=9950, int nLowLimit=9500);

	//�ж������Ӯ�Ƿ����Ҫ��
	//@nWin:��ʾ����������ҵ���Ӯ�ܺͣ�Ӯ=�� ��=����ӵ���������banker��
	//@nPay:��ʾ����������ҵ�Ͷע��ӣ�����������
	//@nCurMinute:��ǰʱ��ķ�����
	//@return:����ֵ��true��ʾ�ں���Χ֮�ڡ�false��ʾ��Ӯ�����������Ҫ�����������·��ƣ����¿�ʼ
	bool HpGetResult(__int64 nWin/*���ÿ����Ӯ�ۼ�*/, __int64 nPay/*ÿ�����Ͷע�ۼ�*/, int nCurMinute, int nParam);
    
	//����ͬ���ϣ��������������������д�룬ֻ�������ж�
	//@nParam:�����ȣ�1-100�� 1=�ǳ�������100=�ǳ��ٶۣ�����ֵ��10�����ٶ۴���ϵͳ�п���������
	bool HpTest(__int64 nWin/*���ÿ����Ӯ�ۼ�*/, __int64 nPay/*ÿ�����Ͷע�ۼ�*/, int nCurMinute, int nParam=10);

    // ������m_nTurnCount������ҵ���Ӯ���ʣ�������ֱ�
    int GetWinRate();
	// �������ݣ�����µķ��ر�
	int GetTestWinRate(__int64 nWin,  __int64  nPay,	int nRatio=1);
	//��ȡӮ��
	__int64	GetWinSum();

	//��ȡ����
	__int64	GetTotalSum();

};



#endif