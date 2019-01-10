#include "StdAfx.h"
#include "resource.h"
#include "ZhuangControl.h"
#include "GameClientEngine.h"


CZhuangControl::CZhuangControl(void)
{
	m_CurrentIndex = 0;
	m_pointOrigin.x = 0;
	m_pointOrigin.y = 0;
	m_bVisbale = true;					//�Ƿ�ɼ�
	m_nPerFrameCount = 0;				///ÿ������������ һ��5������

	//������Դ
	HINSTANCE hResInstance = AfxGetInstanceHandle();

	m_FrameSize.cx = 8364 / 41;
	m_FrameSize.cy = 218;

	m_ImageZhuang.LoadBitmap(IDB_ZHUANG);

	m_sizeRECT = CSize(8364/41,218);
}



CZhuangControl::~CZhuangControl(void)
{
	SetVisable(false);
}


//����֡����
void CZhuangControl::SetFrameIndex(int nIndex)
{
	if (nIndex < 0)
		nIndex = 0;

	if (nIndex >= ZHUANG_FLAME_COUNT)
		m_CurrentIndex = 1;
	else
		m_CurrentIndex = nIndex;

	//if (m_CurrentIndex != nIndex)
	//	m_CurrentIndex = nIndex;
}

//��ȡ֡����
int CZhuangControl::GetFrameIndex()
{
	return m_CurrentIndex;
}

//��ʾ���
bool CZhuangControl::IsFinish()
{
	return m_CurrentIndex >= SICE_FLAME_COUNT;
}

//���û�׼λ��
void CZhuangControl::SetPointBase(int x, int y)
{
	m_pointBase.x = x;
	m_pointBase.y = y;
}

//���û�׼λ��
void CZhuangControl::SetPointBase(CPoint pos)
{
	m_pointBase = pos;
}

//������ʾ��Χ
void CZhuangControl::SetSize(int w, int h)
{
	if (w <= 0)
		w = 100;
	if (h <= 0)
		h = 100;

	m_sizeRECT = CSize(w, h);
}

//������ʾ��Χ
void CZhuangControl::SetSize(CSize size)
{
	if (size.cx <= 0)
		size.cx = 100;
	if (size.cy <= 0)
		size.cy = 100;

	m_sizeRECT = size;
}

//�滭�˿�
VOID CZhuangControl::DrawCardControl(CDC * pDC)
{
	//if (!IsVisable() || m_SiceValue <= 0 || m_SiceValue > 6 || m_CurrentIndex < 0 || m_SiceValue >= SICE_FLAME_COUNT) return;

	//�滭����
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&m_ImageZhuang);

	int nFrame = m_CurrentIndex;
	
	pDC->TransparentBlt(m_pointBase.x-6, m_pointBase.y+10,
		m_FrameSize.cx, m_FrameSize.cy,&memDC,nFrame*m_FrameSize.cx,0, m_FrameSize.cx, m_FrameSize.cy,RGB(255,0,255));

	memDC.DeleteDC();
}

