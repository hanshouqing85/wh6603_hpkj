#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGoldView
{
	//��������
public:
	__int64								m_lGold;						//������Ŀ
	INT									m_nGoldIndex;					//

	//��������
protected:
	static bool							m_bInit;						//��ʼ��־
	static CBitImage					m_ImageGold;					//����ͼƬ
	CFont								ViewFont1;
	CFont								ViewFont2;
	//��������
public:
	//���캯��
	CGoldView();
	//��������
	virtual ~CGoldView();

	//���ܺ���
public:
	//���ó���
	void SetGold(__int64 lGold);
	//��ȡ����
	__int64 GetGold() { return m_lGold; };
	//�滭����
	void DrawGoldView(CWnd* pWnd, CDC * pDC, int nXPos, int nYPos,int iIndex=0);
	//���û滭��������
	VOID SetGoldIndex( INT nIndex );

	//�ڲ�����
private:
	//���Ա��ַ�
	LPCTSTR GetGlodString(__int64 lGold, TCHAR szString[]);
};

//////////////////////////////////////////////////////////////////////////

#endif