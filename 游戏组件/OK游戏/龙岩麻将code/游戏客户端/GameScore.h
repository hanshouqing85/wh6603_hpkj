#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfo
{
	//�����˿�
	BYTE							cbCardCount;							//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];					//�˿�����

	//�û���Ϣ
	TCHAR							szUserName[GAME_PLAYER][LEN_ACCOUNTS];	//�û�����

	//������Ϣ
	WORD							wChiHuUser;								//�������
	WORD							wProvideUser;							//��Ӧ�û�
	BYTE							cbProvideCard;							//��Ӧ�˿�
	LONGLONG						lGameScore[GAME_PLAYER];				//��Ϸ����
	BYTE							cbHuaCardCount;							//���Ƹ���
	BYTE							cbFanCount;								//�ܷ���

	//���ƽ��
	DWORD							dwChiHuKind[GAME_PLAYER];				//��������
};

//�����Ϣ
struct tagWeaveInfo
{
	BYTE							cbWeaveCount;							//�����Ŀ
	BYTE							cbCardCount[MAX_WEAVE];					//�˿���Ŀ
	BYTE							cbPublicWeave[MAX_WEAVE];				//�������
	BYTE							cbCardData[MAX_WEAVE][4];				//����˿�
};

//////////////////////////////////////////////////////////////////////////


//��Ϸ����
class CGameScore : public CDialog
{
	//��������
protected:
	tagScoreInfo					m_ScoreInfo;							//������Ϣ
	CChiHuRight						m_ChiHuRight;							//������Ϣ

	//�ؼ�����
protected:
	BYTE							m_cbWeaveCount;							//�����Ŀ
	CWeaveCard						m_WeaveCard[MAX_WEAVE];					//����˿�
	CSkinButton						m_btCloseScore;							//�رհ�ť

	//��Դ����
protected:
	CBitImage						m_ImageBack;							//����ͼ
	CPngImage						m_ImageZiMo;							//����
	CPngImage						m_ImageFangPao;							//����
	CFont							m_InfoFont;								//��������

	//��������
public:
	//���캯��
	CGameScore();
	//��������
	virtual ~CGameScore();

	//���ܺ���
public:
	//��λ����
	void RestorationData();
	//���û���
	void SetScoreInfo(const tagScoreInfo & ScoreInfo, const tagWeaveInfo & WeaveInfo, const CChiHuRight &ChiHuRight);

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();

	//��Ϣӳ��
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////