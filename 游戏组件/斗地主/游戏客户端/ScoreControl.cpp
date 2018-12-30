#include "StdAfx.h"
#include "GameClient.h"
#include "ScoreControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ�����
#define IDC_BT_CLOSE				100									//�����˿�

//������Ŀ
#define SCORE_CARTOON_COUNT			4									//������Ŀ

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CScoreControl::CScoreControl()
{
	//���ñ���
	m_cbAlphaIndex=0;
	m_cbCartoonIndex=0;

	//���ñ���
	ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));

	return;
}

//��������
CScoreControl::~CScoreControl()
{
}

//������Ϣ
VOID CScoreControl::OnWindowMovie()
{
	//�ɼ�����
	if ((IsWindowVisible()==true)&&(m_CartoonLapseCount.GetLapseCount(400)>0L))
	{
		m_cbCartoonIndex=(m_cbCartoonIndex+1)%SCORE_CARTOON_COUNT;
	}

	//�Ҷȶ���
	if ((IsWindowVisible()==true)&&((m_cbAlphaIndex<110L)&&(m_AlphaLapseCount.GetLapseCount(20)>0L)))
	{
		m_cbAlphaIndex+=5L;
	}

	return;
}

//������Ϣ
VOID CScoreControl::OnWindowCreate(CD3DDevice * pD3DDevice)
{
	//if(pD3DDevice == NULL) return;
	//m_pD3DDevice = pD3DDevice;

	//��������
	m_FontScore.CreateFont(100,TEXT("����"),0L);

	//�رհ�ť
	CRect rcCreate(0,0,0,0);
	m_btClose.ActiveWindow(rcCreate,WS_VISIBLE,IDC_BT_CLOSE,GetVirtualEngine(),this);
	///m_btClose.SetButtonImage(pD3DDevice,TEXT("BT_CLOSE"),TEXT("PNG"),AfxGetInstanceHandle());

	//������Դ
	m_TextureGameScore.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("GAME_SCORE"),TEXT("bmp"));


	m_TextureBombNumber.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("BOMB_NUMBER"),TEXT("PNG"));
	m_TextureWinLoseFlag.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("WIN_LOSE_FLAG"),TEXT("PNG"));
	m_TextureScoreCartoon.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("SCORE_CARTOON"),TEXT("PNG"));
	m_TextureChunTian.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("CHUN_TIAN"),TEXT("PNG"));

	//��ȡ��С
	CSize SizeGameScore;
	SizeGameScore.SetSize(m_TextureGameScore.GetWidth(),m_TextureGameScore.GetHeight());

	//�ƶ�����
	SetWindowPos(0,0,SizeGameScore.cx,SizeGameScore.cy,0);

	//��ťλ��
	CRect rcButton;
	m_btClose.GetWindowRect(rcButton);
	m_btClose.SetWindowPos(SizeGameScore.cx-rcButton.Width()-12,10,0,0,SWP_NOSIZE);
	
	return;
}

//������Ϣ
VOID CScoreControl::OnWindowDestory(CD3DDevice * pD3DDevice)
{
	//ɾ������
	m_FontScore.DeleteFont();

	//������Դ
	if (m_TextureGameScore.IsNull()==false) m_TextureGameScore.Destory();
	if (m_TextureBombNumber.IsNull()==false) m_TextureBombNumber.Destory();
	if (m_TextureWinLoseFlag.IsNull()==false) m_TextureWinLoseFlag.Destory();
	if (m_TextureScoreCartoon.IsNull()==false) m_TextureScoreCartoon.Destory();
	if (m_TextureChunTian.IsNull()==false) m_TextureChunTian.Destory();
	

	return;
}

//����¼�
BOOL CScoreControl::OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)
{
	return FALSE;
}

//��ť�¼�
BOOL CScoreControl::OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)
{
	//�رմ���
	if (uButtonID==IDC_BT_CLOSE)
	{
		CloseControl();
		return TRUE;
	}

	return FALSE;
}

//�����¼�
BOOL CScoreControl::OnEventKeyboard(UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

//�滭����
VOID CScoreControl::OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)
{
	//�һ�����
	CSize SizeWindow=pD3DDevice->GetSizeWindow();
	pD3DDevice->FillRect(0,0,SizeWindow.cx,SizeWindow.cy,D3DCOLOR_ARGB(m_cbAlphaIndex,0,0,0));

	//�滭����
	m_TextureGameScore.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos);

	//��ȡ��С
	CSize SizeBombNumber;
	CSize SizeWinLoseFlag;
	CSize SizeScoreCartoon;
	CSize SizeChunTian;
	SizeBombNumber.SetSize(m_TextureBombNumber.GetWidth()/11L,m_TextureBombNumber.GetHeight());
	SizeWinLoseFlag.SetSize(m_TextureWinLoseFlag.GetWidth()/2L,m_TextureWinLoseFlag.GetHeight());
	SizeScoreCartoon.SetSize(m_TextureScoreCartoon.GetWidth()/SCORE_CARTOON_COUNT,m_TextureScoreCartoon.GetHeight()/2L);
	SizeChunTian.SetSize(m_TextureChunTian.GetWidth()/2,m_TextureChunTian.GetHeight());

	//�滭����
	if ( m_ScoreInfo.wBankerUser < GAME_PLAYER  )
	{
		m_TextureScoreCartoon.DrawImage(pD3DDevice,nXOriginPos+300,nYOriginPos+185,SizeScoreCartoon.cx,SizeScoreCartoon.cy,
			m_cbCartoonIndex*SizeScoreCartoon.cx,m_ScoreInfo.lGameScore[m_ScoreInfo.wBankerUser] > 0 ? 0 : SizeScoreCartoon.cy);
	}


	//�滭��Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		TCHAR szUserScore[16]=TEXT("");
		TCHAR szCollectScore[16]=TEXT("");
		_sntprintf(szUserScore,CountArray(szUserScore),SCORE_STRING,m_ScoreInfo.lGameScore[i]);
		_sntprintf(szCollectScore,CountArray(szCollectScore),SCORE_STRING,m_ScoreInfo.lCollectScore[i]);

		//λ�ü���
		CRect rcNiceName(nXOriginPos+32,nYOriginPos+102+i*81,nXOriginPos+104,nYOriginPos+115+i*81);
		CRect rcGameScore(nXOriginPos+157,nYOriginPos+64+i*81,nXOriginPos+226,nYOriginPos+76+i*81);
		CRect rcCollectScore(nXOriginPos+157,nYOriginPos+85+i*81,nXOriginPos+226,nYOriginPos+97+i*81);

		//�û��ʺ�
		LPCTSTR pszNiceName=m_ScoreInfo.szNickName[i];
		D3DCOLOR crColor=(m_ScoreInfo.wMeChairID==i)?D3DCOLOR_XRGB(153,255,0):D3DCOLOR_XRGB(255,255,255);
		m_FontScore.DrawText(pD3DDevice,pszNiceName,rcNiceName,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS,crColor);

		//�滭ͷ��
		CGameFrameAvatar * pGameFrameAvatar=CGameFrameAvatar::GetInstance();
		pGameFrameAvatar->DrawUserAvatar(pD3DDevice,nXOriginPos+43,nYOriginPos+49+i*81,m_ScoreInfo.wFaceID[i]);

		//�滭��Ϣ
		m_FontScore.DrawText(pD3DDevice,szUserScore,rcGameScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,255,255));
		m_FontScore.DrawText(pD3DDevice,szCollectScore,rcCollectScore,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS,D3DCOLOR_XRGB(255,255,255));

		//ը����Ϣ
		INT nBombExcursion=SizeBombNumber.cx*(m_ScoreInfo.cbEachBombCount[i]+1);
		m_TextureBombNumber.DrawImage(pD3DDevice,nXOriginPos+302,nYOriginPos+73+i*81,SizeBombNumber.cx,SizeBombNumber.cy,0,0);
		m_TextureBombNumber.DrawImage(pD3DDevice,nXOriginPos+317,nYOriginPos+73+i*81,SizeBombNumber.cx,SizeBombNumber.cy,nBombExcursion,0);

		//��Ӯ��־
		INT nWinLostExcursion=(m_ScoreInfo.lGameScore[i]>0L)?0:SizeWinLoseFlag.cx;
		m_TextureWinLoseFlag.DrawImage(pD3DDevice,nXOriginPos+76,nYOriginPos+40+i*81,SizeWinLoseFlag.cx,SizeWinLoseFlag.cy,nWinLostExcursion,0);
	
		//�׷�
		DrawNumber(pD3DDevice, &m_TextureBombNumber, TEXT("x0123456789"), m_ScoreInfo.lCellScore, nXOriginPos+116,nYOriginPos+295);
	}

	//����
	if ( m_ScoreInfo.bChunTian)
	{
		m_TextureChunTian.DrawImage(pD3DDevice,nXOriginPos+250,nYOriginPos+290,SizeChunTian.cx,SizeChunTian.cy,
			0,0);
	}
	else if ( m_ScoreInfo.bFanChunTian )
	{
		m_TextureChunTian.DrawImage(pD3DDevice,nXOriginPos+250,nYOriginPos+290,SizeChunTian.cx,SizeChunTian.cy,
			SizeChunTian.cx,0);
	}

	return;
}

//���ش���
VOID CScoreControl::CloseControl()
{
	//�رմ���
	if (IsWindowActive()==true)
	{
		//�رմ���
		DeleteWindow();
		
		//��������
		ZeroMemory(&m_ScoreInfo,sizeof(m_ScoreInfo));
	}

	return;
}

//���û���
VOID CScoreControl::SetScoreInfo(tagScoreInfo & ScoreInfo)
{
	//���ñ���
	m_ScoreInfo=ScoreInfo;

	//�һ�����
	m_cbAlphaIndex=0;
	m_AlphaLapseCount.Initialization();

	//�ɼ�����
	m_cbCartoonIndex=0;
	m_CartoonLapseCount.Initialization();

	return;
}

// �滭����
void CScoreControl::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
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
