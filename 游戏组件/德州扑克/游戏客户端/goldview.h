#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
//�궨��
#define GOLD_IMAGE_WIDTH		20										//�����
#define GOLD_IMAGE_HEIGHT		19										//�����

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
	//��������
public:
	LONGLONG							m_lGold;						//������Ŀ
	LONGLONG							m_lMaxLayer;					//������
	LONGLONG							m_lGoldCount[7];	 			//������Ŀ

	//��������
protected:
	static bool							m_bInit;						//��ʼ��־
	static CPngImageEx					m_ImageGold;					//����ͼƬ
	CBitImageEx							m_ImageNumber;					//����ͼƬ
	CWnd								*m_pGameViewWnd;				//��ͼ����
	//��������
public:
	//���캯��
	CGoldView();
	//��������
	virtual ~CGoldView();

	//���ܺ���
public:
	//������Դ
	bool LoadResource(CWnd* pWnd);
	//���ó���
	void SetGold(LONGLONG lGold);
	//��ȡ����
	LONGLONG GetGold() { return m_lGold; };
	//���ò���
	void SetMaxGoldLayer(LONGLONG lMaxLayer);
	//��ȡ����
	LONGLONG GetMaxGoldLayer(){return m_lMaxLayer;}
	//�滭����
	void DrawGoldView(CDC * pDC, int nXPos, int nYPos, bool bCount,bool bCenter=false,BYTE bDUser=0);
	//���Ա��ַ�
	LPCTSTR GetGlodString(LONGLONG lGold, TCHAR szString[]);

	//�ڲ�����
private:
	//���������
	void RectifyGoldLayer();
};

//////////////////////////////////////////////////////////////////////////

#endif
