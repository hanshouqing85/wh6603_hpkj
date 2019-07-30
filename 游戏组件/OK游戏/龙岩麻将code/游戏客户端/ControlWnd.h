#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_USER_ACTION				(WM_USER+300)						//�û�����
#define IDM_USER_MOVE				(WM_USER+301)						//��ť����
//////////////////////////////////////////////////////////////////////////

//���ƴ���
class CControlWnd : public CWnd
{
	//�ؼ�����
protected:
	CSkinButton						m_btPeng;							//���ư�ť
	CSkinButton						m_btGang;							//���ư�ť
	//CSkinButton						m_btHear;							//���ư�ť
	CSkinButton						m_btChiHu;							//���ư�ť
	CSkinButton						m_btGiveUp;							//���ư�ť
	CSkinButton						m_btLeftChi;						//��ͷ��ť
	CSkinButton						m_btCenterChi;						//���а�ť
	CSkinButton						m_btRightChi;						//��β��ť

	//��Դ����
protected:
	CBitImage						m_ImageControlBack;					//���ڱ���

	//��������
public:
	//���캯��
	CControlWnd();
	//��������
	virtual ~CControlWnd();

	//���ܺ���
public:
	//����״̬
	VOID SetControlInfo(BYTE cbAcitonMask);

	//�ڲ�����
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��ť��Ϣ
protected:
	//���ư�ť
	VOID OnBnClickedPeng();
	//���ư�ť
	VOID OnBnClickedGang();
	//���ư�ť
	VOID OnBnClickedHear();
	//�Ժ���ť
	VOID OnBnClickedChiHu();
	//������ť
	VOID OnBnClickedGiveUp();
	//���ư�ť
	VOID OnBnClickedLeftChi();
	//���ư�ť
	VOID OnBnClickedCenterChi();
	//���ư�ť
	VOID OnBnClickedRightChi();

public:
	//����뿪
	virtual HRESULT __cdecl OnEventMouseLeft(UINT uControlID, WPARAM wParam, LPARAM lParam);
	//����ƶ�
	virtual HRESULT __cdecl OnEventMouseMove(UINT uControlID, WPARAM wParam, LPARAM lParam);
	//��갴ť
	virtual HRESULT __cdecl OnEventLButtonUp(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//��갴ť
	virtual HRESULT __cdecl OnEventLButtonDown(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//��갴ť
	virtual HRESULT __cdecl OnEventRButtonUp(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//��갴ť
	virtual HRESULT __cdecl OnEventRButtonDown(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif