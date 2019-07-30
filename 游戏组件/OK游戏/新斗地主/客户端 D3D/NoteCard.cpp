#include "StdAfx.h"
#include "GameClient.h"
#include "NoteCard.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ�����
#define IDC_BT_CLOSE				100									//�����˿�

//������Ŀ
#define SCORE_CARTOON_COUNT			4									//������Ŀ

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CNoteCard::CNoteCard()
{
	//���ñ���
	m_cbAlphaIndex=0;

	//���ñ���
	SetCardInfo();

	return;
}

//��������
CNoteCard::~CNoteCard()
{
}

//������Ϣ
VOID CNoteCard::OnWindowMovie()
{
	//�Ҷȶ���
	if ((IsWindowVisible()==true)&&((m_cbAlphaIndex<110L)&&(m_AlphaLapseCount.GetLapseCount(20)>0L)))
	{
		m_cbAlphaIndex+=5L;
	}

	return;
}

//������Ϣ
VOID CNoteCard::OnWindowCreate(CD3DDevice * pD3DDevice)
{
	//��������
	//m_FontScore.CreateFont(100,TEXT("����"),0L);

	//�رհ�ť
	CRect rcCreate(0,0,0,0);
	//������Դ
	m_TextureNoteCard.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("NOTE_CARD"),TEXT("PNG"));
	m_TextureMiniCard.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("MINI_CARD"),TEXT("PNG"));

	//��ȡ��С
	m_SizeMiniCard.SetSize(m_TextureMiniCard.GetWidth() / 13,m_TextureMiniCard.GetHeight() / 5);

	CSize SizeNoteCard;
	SizeNoteCard.SetSize(m_TextureNoteCard.GetWidth(),m_TextureNoteCard.GetHeight());

	//�ƶ�����
	SetWindowPos(0,0,SizeNoteCard.cx,SizeNoteCard.cy,0);

	return;
}

//������Ϣ
VOID CNoteCard::OnWindowDestory(CD3DDevice * pD3DDevice)
{
	//ɾ������
	//m_FontScore.DeleteFont();

	//������Դ
	if (m_TextureNoteCard.IsNull()==false) m_TextureNoteCard.Destory();
	if (m_TextureMiniCard.IsNull()==false) m_TextureMiniCard.Destory();

	return;
}

//����¼�
VOID CNoteCard::OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)
{
	return;
}

//��ť�¼�
VOID CNoteCard::OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)
{
	//�رմ���
	if (uButtonID==IDC_BT_CLOSE)
	{
		CloseControl();
		return;
	}

	return;
}

//�滭����
VOID CNoteCard::OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)
{
	//�һ�����
	//CSize SizeWindow=pD3DDevice->GetSizeWindow();
	//pD3DDevice->FillRect(0,0,SizeWindow.cx,SizeWindow.cy,D3DCOLOR_ARGB(m_cbAlphaIndex,0,0,0));

	//�滭����
	m_TextureNoteCard.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos);

	int nX;
	int nY;
	for(int i = 0; i < 54; i++)
	{
		nX = i % 13;
		nY = i / 13;
		m_TextureMiniCard.DrawImage(pD3DDevice,nXOriginPos + 42 + nX * 15 ,nYOriginPos + 5 + nY * 32, m_SizeMiniCard.cx, m_SizeMiniCard.cy,
			m_SizeMiniCard.cx * nX, m_SizeMiniCard.cy * nY);

		if(0 == m_cbCardData[i])
		{
			m_TextureMiniCard.DrawImage(pD3DDevice,nXOriginPos + 42 + nX * 15 ,nYOriginPos + 5 + nY * 32, m_SizeMiniCard.cx, m_SizeMiniCard.cy,
				m_SizeMiniCard.cx * 12, m_SizeMiniCard.cy * 4);
		}
	}

	return;
}

//���ش���
VOID CNoteCard::CloseControl()
{
	//�رմ���
	if (IsWindowActive()==true)
	{
		//�رմ���
		DeleteWindow();

		//��������

	}

	return;
}
//�����˿�
VOID CNoteCard::SetCardInfo()
{
	for(int i = 0; i < 54; i++)
	{
		m_cbCardData[i] = 1;
	}
}

//�����˿�
VOID CNoteCard::SetCardInfo(BYTE *cbCardData, BYTE cbCardCount)
{
	ZeroMemory(m_cbCardData, sizeof(m_cbCardData));

	int nIndex = -1;
	//���ñ���
	for(int i = 0; i < cbCardCount; i++)
	{
		nIndex = SwitchToIndex(cbCardData[i]);
		if(nIndex < 0 || nIndex >= 54)
		{
			continue;
		}
		m_cbCardData[nIndex] = 1;
	}

	//�һ�����
	m_cbAlphaIndex=0;
	m_AlphaLapseCount.Initialization();

	return;
}

//ɾ���˿�
VOID CNoteCard::RemoveCard(BYTE cbCardData[], BYTE cbCardCount)
{
	int nIndex = -1;
	//���ñ���
	for(int i = 0; i < cbCardCount; i++)
	{
		nIndex = SwitchToIndex(cbCardData[i]);
		
		if(nIndex < 0 || nIndex >= 54)
		{
			continue;
		}
		m_cbCardData[nIndex] = 0;
	}
}

//ת������
int CNoteCard::SwitchToIndex(BYTE cbCardData)
{
	int nIndex;
	
	if (cbCardData==0x4E)
	{
		nIndex = 52;
	}
	else if (cbCardData==0x4F)
	{
		nIndex = 53;
	}
	else
	{
		nIndex = ((cbCardData & 0xF0) >> 4) * 13 + ((cbCardData & 0x0F) - 1);
	}
	ASSERT(nIndex >= 0 && nIndex < 54);
	return nIndex;

}
// �滭����
void CNoteCard::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);

	// ������Դ
	INT nNumberHeight=ImageNumber->GetHeight();
	INT nNumberWidth=ImageNumber->GetWidth()/lstrlen(szImageNum);

	if ( uFormat == DT_CENTER )
	{
		nXPos -= (INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth);
	}
	else if ( uFormat == DT_RIGHT )
	{
		nXPos -= lstrlen(szOutNum) * nNumberWidth;
	}

	for ( INT i = 0; i < lstrlen(szOutNum); ++i )
	{
		for ( INT j = 0; j < lstrlen(szImageNum); ++j )
		{
			if ( szOutNum[i] == szImageNum[j] && szOutNum[i] != '\0' )
			{
				ImageNumber->DrawImage(pD3DDevice, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
