#include "StdAfx.h"
#include "GameClient.h"
#include "DoubleControl.h"

//////////////////////////////////////////////////////////////////////////////////
#define DOUBLE_STRING				TEXT("*%I64d")						//��������

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDoubleControl::CDoubleControl()
{
	//���ñ���
	m_cbAlphaIndex=0;
	SetDoubleInfo();

	return;
}

//��������
CDoubleControl::~CDoubleControl()
{
}

//������Ϣ
VOID CDoubleControl::OnWindowMovie()
{
	//�Ҷȶ���
	if ((IsWindowVisible()==true)&&((m_cbAlphaIndex<110L)&&(m_AlphaLapseCount.GetLapseCount(20)>0L)))
	{
		m_cbAlphaIndex+=5L;
	}

	return;
}

//������Ϣ
VOID CDoubleControl::OnWindowCreate(CD3DDevice * pD3DDevice)
{
	//��������
	LOGFONT logfont; 

	logfont.lfHeight = 12;
	logfont.lfWidth = 6;
	logfont.lfEscapement = 0; 
	logfont.lfOrientation = 0; 
	logfont.lfWeight = FW_BOLD; 
	logfont.lfItalic = FALSE; 
	logfont.lfUnderline = FALSE; 
	logfont.lfStrikeOut = FALSE; 
	logfont.lfCharSet = ANSI_CHARSET ; 
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	logfont.lfQuality = DEFAULT_QUALITY; 
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	lstrcpy (logfont.lfFaceName, TEXT("����"));
	m_Font.CreateFont(logfont,0L);

	//	m_FontScore.CreateFont(100,TEXT("����"),0L);

	//������Դ
	m_TextureGameDouble.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("DOUBLE_BACK"),TEXT("PNG"));
	m_TextureBaseNumber.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("BASE_DOUBLE"),TEXT("PNG"));
	m_TextureProcessNumber.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PROCESS_DOUBLE"),TEXT("PNG"));
	m_TexturePlayersNumber.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("PLAYERS_DOUBLE"),TEXT("PNG"));;

	//��ȡ��С
	CSize SizeGameDouble;
	SizeGameDouble.SetSize(m_TextureGameDouble.GetWidth(),m_TextureGameDouble.GetHeight());

	//�ƶ�����
	SetWindowPos(0,0,SizeGameDouble.cx,SizeGameDouble.cy,0);

	return;
}

//������Ϣ
VOID CDoubleControl::OnWindowDestory(CD3DDevice * pD3DDevice)
{
	//ɾ������
	m_Font.DeleteFont();

	//������Դ
	if (m_TextureGameDouble.IsNull()==false) m_TextureGameDouble.Destory();
	if (m_TextureBaseNumber.IsNull()==false) m_TextureBaseNumber.Destory();
	if (m_TextureProcessNumber.IsNull()==false) m_TextureProcessNumber.Destory();
	if (m_TexturePlayersNumber.IsNull()==false) m_TexturePlayersNumber.Destory();

	return;
}

//����¼�
VOID CDoubleControl::OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)
{
	return;
}

//��ť�¼�
VOID CDoubleControl::OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)
{
	return;
}

//�滭����
VOID CDoubleControl::OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)
{
	//�һ�����
// 	CSize SizeWindow=pD3DDevice->GetSizeWindow();
// 	pD3DDevice->FillRect(0,0,SizeWindow.cx,SizeWindow.cy,D3DCOLOR_ARGB(m_cbAlphaIndex,0,0,0));

	//�滭����
	m_TextureGameDouble.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos);

	WORD wLeft = nXOriginPos + 24;
	WORD wTop = nYOriginPos + 30;
	CRect rc;

	//��ʼ
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wStartTime != 0 ? TEXT("��ʼ") : TEXT("��ʼ -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wStartTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wStartTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wStartTime, wLeft + 53, wTop - 3, DT_CENTER);
	}	

	//���� 
	wTop += 18;
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wValidCardTime != 0 ? TEXT("����") : TEXT("���� -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wValidCardTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wValidCardTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wValidCardTime, wLeft + 53, wTop - 3, DT_CENTER);
	}

	//������
	wTop += 18;
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wRodBankerTime != 0 ? TEXT("������") : TEXT("������ -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wRodBankerTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wRodBankerTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wRodBankerTime, wLeft + 53, wTop - 3, DT_CENTER);
	}

	wLeft = nXOriginPos + 24 + 80;
	wTop = nYOriginPos + 30;

	//����
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wBackCardTime != 0 ? TEXT("����") : TEXT("���� -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wBackCardTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wBackCardTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wBackCardTime, wLeft + 53, wTop - 3, DT_CENTER);
	}

	//ը��
	wTop += 18;
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wBombTime != 0 ? TEXT("ը��") : TEXT("ը�� -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wBombTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wBombTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wBombTime, wLeft + 53, wTop - 3, DT_CENTER);
	}

	//����
	wTop += 18;
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wChunTianbTime != 0 ? TEXT("����") : TEXT("���� -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wChunTianbTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wChunTianbTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wChunTianbTime, wLeft + 53, wTop - 3, DT_CENTER);
	}

	//����
	wTop += 18;
	rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
	m_Font.DrawText(pD3DDevice, m_DoubleInfo.wCardTypeTime != 0 ? TEXT("����") : TEXT("���� -") ,rc,DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS,
		m_DoubleInfo.wCardTypeTime != 0 ? D3DCOLOR_XRGB(255,255,255) : D3DCOLOR_XRGB(203, 173, 134) );
	if (m_DoubleInfo.wCardTypeTime != 0)
	{
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("*0123456789"), m_DoubleInfo.wCardTypeTime, wLeft + 53, wTop - 3, DT_CENTER);
	}


	wLeft = nXOriginPos + 24 + 140;
	wTop = nYOriginPos + 30;

	//���̱���
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		rc.SetRect(wLeft,wTop,wLeft + 60,wTop + 12);
		m_Font.DrawText(pD3DDevice,m_szNickName[i],rc,DT_VCENTER|DT_RIGHT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255, 255, 255));
		DrawNumber(pD3DDevice, &m_TextureProcessNumber, TEXT("*0123456789"), m_DoubleInfo.wUserDouble[i], wLeft + 82, wTop - 1, DT_CENTER);

		wTop += 20;
	}

	wLeft = nXOriginPos;
	wTop = nYOriginPos + 30 + 105;

	//��������
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		rc.SetRect(wLeft,wTop,wLeft + 50,wTop + 12);
		m_Font.DrawText(pD3DDevice,m_szNickName[i],rc,DT_VCENTER|DT_RIGHT|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255, 255, 255));
		DrawNumber(pD3DDevice, &m_TextureBaseNumber, TEXT("+0123456789"), m_wTotalTime, wLeft + 84, wTop - 3, DT_CENTER);
		if (i == GAME_PLAYER - 1)
		{
			DrawNumber(pD3DDevice, &m_TexturePlayersNumber, TEXT("+0123456789"),  2 * m_wTotalTime * m_wAddDoubleTime,	wLeft + 270, wTop - 3, DT_CENTER);
			DrawNumber(pD3DDevice, &m_TextureProcessNumber, TEXT("+0123456789"),  m_wAddDoubleTime,	wLeft + 130, wTop - 1, DT_CENTER);
		}
		else
		{
			DrawNumber(pD3DDevice, &m_TexturePlayersNumber, TEXT("+0123456789"),  m_wTotalTime * m_wAddDoubleTime,	wLeft + 270, wTop - 3, DT_CENTER);
			DrawNumber(pD3DDevice, &m_TextureProcessNumber, TEXT("+0123456789"),  m_wAddDoubleTime,	wLeft + 130, wTop - 1, DT_CENTER);
			
		}
		

		wTop += 20;
	}
	return;
}

//���ش���
VOID CDoubleControl::CloseControl()
{
	//�رմ���
	if (IsWindowActive()==true)
	{
		//�رմ���
		DeleteWindow();		
	}

	return;
}

//���û���
VOID CDoubleControl::SetDoubleInfo()
{
	m_wTotalTime = 0;
	m_wAddDoubleTime = 1;
	ZeroMemory(&m_DoubleInfo, sizeof(m_DoubleInfo));	
	ZeroMemory(&m_szNickName, sizeof(m_szNickName));	
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_DoubleInfo.wUserDouble[i] = 1;
	}

}
//���û���
VOID CDoubleControl::SetDoubleInfo(tagDoubleInfo &DoubleInfo)
{
	//�һ�����
	m_cbAlphaIndex=0;
	m_wTotalTime = 1;
	m_wAddDoubleTime = 1;
	m_DoubleInfo = DoubleInfo;
	

	TCHAR szNickName[4][LEN_NICKNAME] = {TEXT("��"), TEXT("����"), TEXT("ũ���"), TEXT("ũ����")};	

	if (m_DoubleInfo.wMeChairID >= 0 && m_DoubleInfo.wMeChairID < GAME_PLAYER &&
		m_DoubleInfo.wBankerUser >= 0 && m_DoubleInfo.wBankerUser < GAME_PLAYER)
	{
		//ת���ӱ�
		if (m_DoubleInfo.wMeChairID == m_DoubleInfo.wBankerUser)
		{
			int nIndex = 0;
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				if (i != m_DoubleInfo.wMeChairID)
				{
					lstrcpyn(m_szNickName[nIndex], szNickName[nIndex + 2],CountArray(szNickName[nIndex + 2]));
					m_DoubleInfo.wUserDouble[nIndex] = DoubleInfo.wUserDouble[i];
					nIndex++;
				}
			}
			lstrcpyn(m_szNickName[2], szNickName[0],CountArray(szNickName[0]));	
			m_DoubleInfo.wUserDouble[2] = DoubleInfo.wUserDouble[m_DoubleInfo.wMeChairID];
		}
		else
		{
			for (int i = 0; i < GAME_PLAYER; i++)
			{
				if (i != m_DoubleInfo.wMeChairID && i != m_DoubleInfo.wBankerUser)
				{
					m_DoubleInfo.wUserDouble[0] = DoubleInfo.wUserDouble[i];
					
					break;
				}
			}
			
			lstrcpyn(m_szNickName[0], szNickName[2],CountArray(szNickName[2]));
			lstrcpyn(m_szNickName[1], szNickName[0],CountArray(szNickName[0]));
			lstrcpyn(m_szNickName[2], szNickName[1],CountArray(szNickName[1]));
			m_DoubleInfo.wUserDouble[1] = DoubleInfo.wUserDouble[m_DoubleInfo.wMeChairID];
			m_DoubleInfo.wUserDouble[2] = DoubleInfo.wUserDouble[m_DoubleInfo.wBankerUser];
		}
	}


	//���㱶��
	m_wTotalTime *= (m_DoubleInfo.wStartTime != 0 ? m_DoubleInfo.wStartTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wValidCardTime != 0 ? m_DoubleInfo.wValidCardTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wRodBankerTime != 0 ? m_DoubleInfo.wRodBankerTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wBackCardTime != 0 ? m_DoubleInfo.wBackCardTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wBombTime != 0 ? m_DoubleInfo.wBombTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wChunTianbTime != 0 ? m_DoubleInfo.wChunTianbTime : 1);
	m_wTotalTime *= (m_DoubleInfo.wCardTypeTime != 0 ? m_DoubleInfo.wCardTypeTime : 1);

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_wAddDoubleTime *= m_DoubleInfo.wUserDouble[i];
	}
	
	m_AlphaLapseCount.Initialization();
	
	return;
}

// �滭����
void CDoubleControl::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),DOUBLE_STRING,lOutNum);

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
