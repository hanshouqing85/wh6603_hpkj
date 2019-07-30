#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include ".\gameclientview.h"
#include "gameclientengine.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

#ifdef DEBUG

//#define DEBUG_TABLE
//#define DEBUG_CONTROL
	
#endif


//������ʶ
#define IDI_HIDE_CARD					100								//�����˿�
#define IDI_SEND_CARD					101								//���Ʊ�ʶ
#define IDI_USER_ACTION					102								//������ʶ
#define IDI_MOVE_CHIP					103								//�����ƶ�

//������Ŀ
#define SEND_CARD_COUNT					5								//������Ŀ

//λ�ö���
#define CARD_EXCURSION_X				283								//�˿�ƫ��
#define CARD_EXCURSION_Y				177								//�˿�ƫ��

//���ƶ���
#define SEND_STEP_COUNT					8								//����
#define SPEED_SEND_CARD					10								//�����ٶ�

//���붨��
#define CHIP_COUNT						14								//��������
#define PILING_HEIGHT					6								//�ѻ��߶�


//��ť��ʶ
#define IDC_BT_ADMIN					221
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//��ť��Ϣ
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{

	//״̬����
	m_lCellScore=SCORE_ZERO;
	m_lDrawMaxScore=SCORE_ZERO;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));
	ZeroMemory(m_cbUserCard,sizeof(m_cbUserCard));
	ZeroMemory(m_cbUserCardCount,sizeof(m_cbUserCardCount));	

	//��������
	m_SendCardPos.SetPoint(0,0);
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = SEND_STEP_COUNT;
	m_nXStep = 0;
	m_nYStep = 0;

	//��������
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//�˿˳���
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));

	m_lALLBetChip = SCORE_ZERO;

	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		m_CardControl[i].Initialize( i != 2 );
	}

	//������Դ
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_ImageReady.LoadImage(AfxGetInstanceHandle(), TEXT("READY"));

	m_ImageGameCard.LoadImage(AfxGetInstanceHandle(), TEXT("GAME_CARD"));
	m_ImageViewFill.LoadFromResource(hResInstance,IDB_VIEW_FILL);
	m_ImageViewBack.LoadImage(hResInstance,TEXT("VIEW_BACK"));
	m_ImgaeScoreTotalNumber.LoadImage(hResInstance,TEXT("SCORE_NUMBER"));
	m_ImageActionBack.LoadImage(hResInstance,TEXT("ACTION_BACK"));
	m_ImageActionFont.LoadImage(hResInstance,TEXT("ACTION_FONT"));
	m_ImageShowHand.LoadImage(hResInstance,TEXT("SHOW_HAND"));

	m_ImageChip.LoadImage(hResInstance,TEXT("JETTON_VIEW"));

	m_ImageCellScore.LoadImage(hResInstance,TEXT("CELL_SCORE"));

	m_ImageCellNumber.LoadImage(hResInstance,TEXT("CELL_NUMBER"));

	m_ImgaeScoreBackR.LoadImage(hResInstance,TEXT("GAME_SCORE_R"));
	m_ImgaeScoreBackL.LoadImage(hResInstance,TEXT("GAME_SCORE_L"));
	m_ImgaeScoreBackM.LoadImage(hResInstance,TEXT("GAME_SCORE_M"));

	m_ImgaeScoreWin.LoadImage(hResInstance,TEXT("WIN_NUMBER"));
	m_ImgaeScoreLose.LoadImage(hResInstance,TEXT("LOSE_NUMBER"));

	m_ImageCradType.LoadImage(hResInstance,TEXT("CARD_TYPE"));

	m_ImageUserInfo.LoadImage(hResInstance,TEXT("USER_INFO"));
	m_ImageClock.LoadImage(hResInstance,TEXT("TIME"));

	m_PngOffLine.LoadImage(hResInstance,TEXT("OFF_LINE"));

	//��ȡ��С
	m_SizeGameCard.SetSize(m_ImageGameCard.GetWidth()/13,m_ImageGameCard.GetHeight()/5);

	return;
}

//��������
CGameClientView::~CGameClientView()
{
	//ɾ������
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();

	m_Font.DeleteFont();
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN 
		&& m_PlayOperate.IsWindowVisible() && m_PlayOperate.IsAddShow() )
	{
		//������Ϣ
		SCORE sFollowCount = m_PlayOperate.GetBetCount();
		SendEngineMessage(IDM_FOLLOW, (WPARAM)&sFollowCount, 0);
	}

	if ( m_PlayOperate.PreTranslateMessage(pMsg) )
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		return TRUE;
	}

	BOOL bReturn = __super::PreTranslateMessage(pMsg);

	return bReturn;
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	__super::ResetGameView();

	//ɾ��ʱ��
	KillTimer(IDI_HIDE_CARD);
	KillTimer(IDI_SEND_CARD);
	KillTimer(IDI_USER_ACTION);

	//״̬����
	m_lCellScore=SCORE_ZERO;
	m_lDrawMaxScore=SCORE_ZERO;
	ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
	ZeroMemory(m_lUserAddScore,sizeof(m_lUserAddScore));
	ZeroMemory(m_lUserAddAllScore,sizeof(m_lUserAddAllScore));
	ZeroMemory(m_lUserSaveScore,sizeof(m_lUserSaveScore));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lOverScore,sizeof(m_lOverScore));
	ZeroMemory(m_lOverSaveScore,sizeof(m_lOverSaveScore));
	ZeroMemory(m_cbUserCard,sizeof(m_cbUserCard));
	ZeroMemory(m_cbUserCardCount,sizeof(m_cbUserCardCount));

	//��������
	m_SendCardCurPos = m_SendCardPos;
	m_nStepCount = 0;
	m_nXStep = 0;
	m_nYStep = 0;
	m_SendCardItemArray.RemoveAll();

	//��������
	m_cbUserAction=0;
	m_wActionUser=INVALID_CHAIR;

	//���ư�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_PlayOperate.ShowOperate(false);

	//��ֹ��ť
	m_PlayOperate.m_btFollow.EnableWindow(FALSE);
	m_PlayOperate.m_btAddScore.EnableWindow(FALSE);
	m_PlayOperate.m_btShowHand.EnableWindow(FALSE);

	//�˿˳���
	ZeroMemory(m_lPalyBetChip,sizeof(m_lPalyBetChip));
	m_lALLBetChip = SCORE_ZERO;
	

	//����ؼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
		m_CardControl[i].SetCardData(NULL,0);

	//�û��˿�
	m_CardControl[MYSELF_VIEW_ID].SetPositively(false);
	m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

	//ɾ������
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
		SafeDelete(m_ArrayPlayChip[i]);
	m_ArrayPlayChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
		SafeDelete(m_ArrayBetChip[i]);
	m_ArrayBetChip.RemoveAll();

	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
		SafeDelete(m_ArrayOverChip[i]);
	m_ArrayOverChip.RemoveAll();

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//�û���Ϣ
	m_ptUserInfo[4].SetPoint(nWidth/2+325 ,nHeight/2-195);
	m_ptUserInfo[3].SetPoint(nWidth/2+325 ,nHeight/2);
	m_ptUserInfo[2].SetPoint(nWidth/2-76,nHeight/2+200);
	m_ptUserInfo[1].SetPoint(nWidth/2-480,nHeight/2);
	m_ptUserInfo[0].SetPoint(nWidth/2-480,nHeight/2-195);

	m_ptOffLine[0].SetPoint(m_ptUserInfo[0].x,m_ptUserInfo[0].y+90);
	m_ptOffLine[1].SetPoint(m_ptUserInfo[1].x,m_ptUserInfo[1].y+90);
	m_ptOffLine[2].SetPoint(m_ptUserInfo[2].x,m_ptUserInfo[2].y+90);
	m_ptOffLine[3].SetPoint(m_ptUserInfo[3].x,m_ptUserInfo[3].y+90);
	m_ptOffLine[4].SetPoint(m_ptUserInfo[4].x,m_ptUserInfo[4].y+90);

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		m_ptAvatar[i].SetPoint(m_ptUserInfo[i].x+14,m_ptUserInfo[i].y+13);
		m_ptNickName[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+12);
		m_ptClock[i].SetPoint(m_ptUserInfo[i].x+13,m_ptUserInfo[i].y+70);
		m_ptLevel[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+32);
		m_ptScore[i].SetPoint(m_ptUserInfo[i].x+72,m_ptUserInfo[i].y+32);

		//����λ��
		if( i < 2 )
			m_ptUserAction[i].SetPoint(m_ptUserInfo[i].x+m_ImageUserInfo.GetWidth(),m_ptUserInfo[i].y-m_ImageActionBack.GetHeight() + 40);
		else
			m_ptUserAction[i].SetPoint(m_ptUserInfo[i].x-m_ImageActionBack.GetWidth()/2,m_ptUserInfo[i].y-m_ImageActionBack.GetHeight() + 40);
	}

	//׼��λ��
	m_ptReady[4].SetPoint(nWidth/2+165,m_ptUserInfo[4].y + 43);
	m_ptReady[3].SetPoint(nWidth/2+165,m_ptUserInfo[3].y + 43);
	m_ptReady[2].SetPoint(nWidth/2,nHeight/2+140);
	m_ptReady[1].SetPoint(nWidth/2-165,m_ptUserInfo[1].y + 43);
	m_ptReady[0].SetPoint(nWidth/2-165,m_ptUserInfo[0].y + 43);

	//�˿˿ؼ�
	m_CardControl[0].SetBenchmarkPos(nWidth/2 - 220, nHeight/2 - 200,	enXCenter,enYTop);
	m_CardControl[1].SetBenchmarkPos(nWidth/2 - 310, nHeight/2 + 30,	enXLeft,enYCenter);
	m_CardControl[2].SetBenchmarkPos(nWidth/2, nHeight/2 + 193 + 18,	enXCenter,enYBottom);
	m_CardControl[3].SetBenchmarkPos(nWidth/2 + 310, nHeight/2 + 30,	enXRight,enYCenter);
	m_CardControl[4].SetBenchmarkPos(nWidth/2 + 220, nHeight/2 - 200,	enXCenter,enYTop);

	//��ҳ���
	m_ptPalyBetChip[0].SetPoint(nWidth/2 - 122, nHeight/2 - 125);
	m_ptPalyBetChip[1].SetPoint(nWidth/2 - 120,	nHeight/2 + 30);
	m_ptPalyBetChip[2].SetPoint(nWidth/2,		nHeight/2 + 38/*45 + 15*/);
	m_ptPalyBetChip[3].SetPoint(nWidth/2 + 120,	nHeight/2 + 30);
	m_ptPalyBetChip[4].SetPoint(nWidth/2 + 122, nHeight/2 - 125);

	// ��������
	m_ptOverScore[0].SetPoint(nWidth/2 - 220, nHeight/2 - 75);
	m_ptOverScore[1].SetPoint(nWidth/2 - 210, nHeight/2 + 95);
	m_ptOverScore[2].SetPoint(nWidth/2,       nHeight/2 + 290);
	m_ptOverScore[3].SetPoint(nWidth/2 + 210, nHeight/2 + 95);
	m_ptOverScore[4].SetPoint(nWidth/2 + 220, nHeight/2 - 75);

	//������ʼλ��
	m_SendCardPos.SetPoint(nWidth / 2 + 20,nHeight / 2 - 200);
	
	//��ʼ��ť
	m_btStart.SetWindowPos( NULL,m_ptUserInfo[MYSELF_VIEW_ID].x+m_ImageUserInfo.GetWidth() + 20,m_ptUserInfo[MYSELF_VIEW_ID].y - 50,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE);

	//���ƴ���
	m_PlayOperate.SetOperatePos( nWidth/2 + 100, nHeight/2+120 + 18, PYOE_LEFT|PYOE_TOP);

	// �������
	m_rectBetChip.SetRect( nWidth / 2 - 75,nHeight / 2 - 140, nWidth / 2 + 75,nHeight / 2 - 10 + 18);

	// �任����λ��
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		CPoint ptOffset( (nWidth - m_sizeWin.cx)/2,(nHeight - m_sizeWin.cy)/2 ); 
		m_ArrayBetChip[i]->ptBeginPos += ptOffset;
		m_ArrayBetChip[i]->ptEndPos += ptOffset;
		m_ArrayBetChip[i]->ptCurrentPos += ptOffset;
	}
	
	// ���ڴ�С
	m_sizeWin.SetSize(nWidth, nHeight);

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	//�滭����
	DrawViewImage(pDC,m_ImageViewFill,DRAW_MODE_SPREAD);
	m_ImageViewBack.DrawImage( pDC, nWidth/2 - m_ImageViewBack.GetWidth()/2, nHeight/2 - m_ImageViewBack.GetHeight()/2);

	// ��Ϣ��ʾ
	m_ImageCellScore.DrawImage( pDC, nWidth/2 - 360, 0 );

	CString strScore;
	strScore.Empty();
	//��Ϣ����
	if( m_lCellScore > 0.001f )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("+-.0123456789"), m_lCellScore, nWidth/2 - 292, 7);
	if( m_lDrawMaxScore > 0.001f  )
		DrawNumber(pDC, &m_ImageCellNumber, TEXT("+-.0123456789"), m_lDrawMaxScore,nWidth/2 - 292, 37);

	//������ע
  	SCORE lTableScore = m_lALLBetChip;
  
  	for( INT i = 0; i < GAME_PLAYER; i++ )
  		lTableScore += m_lPalyBetChip[i];

	//��ע��Ϣ
	if ( lTableScore > 0.001f )
		DrawNumber(pDC, &m_ImgaeScoreTotalNumber, TEXT("+-.0123456789"), lTableScore, nWidth/2 - 43, nHeight/2-166,DT_CENTER);

	//�û��˿�
	for (BYTE i=0;i<GAME_PLAYER;i++) 
	{
		m_CardControl[i].DrawCardControl(pDC);
	}

	TCHAR szbuff[256]=TEXT("");
	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;
		//�׿�
		m_ImageUserInfo.DrawImage(pDC,m_ptUserInfo[i].x,m_ptUserInfo[i].y);

		//ʱ��
		DrawUserClockEx(pDC,m_ptClock[i].x,m_ptClock[i].y,i);

		//����
		CRect rc(m_ptNickName[i].x,m_ptNickName[i].y,m_ptNickName[i].x+68,m_ptNickName[i].y+16);
		m_Font.DrawText(pDC, pIClientUserItem->GetNickName(),&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);

		// ͷ��
		DrawUserAvatar(pDC, m_ptAvatar[i].x, m_ptAvatar[i].y, pIClientUserItem);

		//����
		ZeroMemory(szbuff,sizeof(szbuff));
		_sntprintf(szbuff,CountArray(szbuff),SCORE_STRING,pIClientUserItem->GetUserScore());

		rc.SetRect(m_ptScore[i].x,m_ptScore[i].y,m_ptScore[i].x+68,m_ptScore[i].y+16);
		m_Font.DrawText(pDC, szbuff,&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);

		//��ַ����
		rc.SetRect(m_ptScore[i].x,m_ptScore[i].y+20,m_ptScore[i].x+68,m_ptScore[i].y+32);;
		m_Font.DrawText(pDC, pIClientUserItem->GetAddrDescribe(),&rc, RGB(250,250,0), DT_LEFT|DT_END_ELLIPSIS);

		CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
		IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;

		if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
		{
			_sntprintf(szbuff,CountArray(szbuff),TEXT("%d"),pIClientUserItem->GetUserID());
			rc.SetRect(m_ptScore[i].x,m_ptScore[i].y+46,m_ptScore[i].x+68,m_ptScore[i].y+60);
			m_Font.DrawText(pDC,szbuff,&rc,RGB(255,255,255),DT_LEFT|DT_END_ELLIPSIS);
		}


	}

	// �ƶ�����
	for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// �滭����
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayBetChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayBetChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayBetChip[i]->wChipIndex, 0 );
	}

	// ���ճ���
	for ( int i = 0 ; i < m_ArrayOverChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		// �滭����
		if ( !m_ArrayOverChip[i]->bShow )
			continue;

		m_ImageChip.DrawImage( pDC, (INT)m_ArrayOverChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayOverChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayOverChip[i]->wChipIndex, 0 );
	}

	//�û���ǰ����
	int nChipCount[GAME_PLAYER] = {0};
	for ( int i = 0 ; i < m_ArrayPlayChip.GetCount(); ++i )
	{
		// ��ȡ����
		INT nChipHeight = m_ImageChip.GetHeight();
		INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;

		if( m_ArrayPlayChip[i]->wChipChairID < GAME_PLAYER && nChipCount[m_ArrayPlayChip[i]->wChipChairID] <= PILING_HEIGHT )
			nChipCount[m_ArrayPlayChip[i]->wChipChairID]++;

		if( nChipCount[m_ArrayPlayChip[i]->wChipChairID] >= PILING_HEIGHT )
			continue;

		// �滭����
		m_ImageChip.DrawImage( pDC, (INT)m_ArrayPlayChip[i]->ptCurrentPos.x - nChipWidth/2, (INT)m_ArrayPlayChip[i]->ptCurrentPos.y - nChipHeight / 2,
			nChipWidth, nChipHeight, 
			nChipWidth * m_ArrayPlayChip[i]->wChipIndex, 0 );
	}

	//�û���ǰֹͣ����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;

		SCORE lPalyBetChip = m_lPalyBetChip[i];
		SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
		SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
		UINT	 nChipCount    = 0;
		while ( lPalyBetChip >= lScoreArray[0]  && nChipCount < PILING_HEIGHT )
		{
			for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
			{
				if ( lPalyBetChip >= lScoreArray[j] )
				{
					// ��ȡ����
					INT nChipHeight = m_ImageChip.GetHeight();
					INT nChipWidth  = m_ImageChip.GetWidth() / CHIP_COUNT;
					INT nXPos = m_ptPalyBetChip[i].x - nChipWidth/2;
					INT nYPos = m_ptPalyBetChip[i].y - nChipHeight/2 - nChipCount * 5;
					m_ImageChip.DrawImage(pDC, nXPos, nYPos, nChipWidth, nChipHeight, nChipWidth * j, 0);
					lPalyBetChip -= lScoreArray[j];
					nChipCount++;
					break;
				}
			}
		}

		CRect rc(m_ptPalyBetChip[i].x - 100, m_ptPalyBetChip[i].y + 15, m_ptPalyBetChip[i].x + 100, m_ptPalyBetChip[i].y + 29);
		
		//��������
		if( m_lPalyBetChip[i] > 0.001f )
		{			
			strScore.Format(/*TEXT("����ע��")*/SCORE_STRING,m_lPalyBetChip[i]);
			DrawTextStringEx(pDC, strScore, RGB(200,249,248), RGB(0,0,0), rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		
		//��������
		if( m_lTableScore[i] > 0.001f )
		{
			rc.OffsetRect(0, 20);
			strScore.Format(TEXT("����ע��")SCORE_STRING,m_lTableScore[i]);
			DrawTextStringEx(pDC, strScore, RGB(200,249,248), RGB(0,0,0), rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		
	}

	//�滭�����˿�
	if ( m_SendCardItemArray.GetCount() > 0 )
	{
		//��������
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[0];

		//��ȡ��С
		int nItemWidth=m_SizeGameCard.cx;
		int nItemHeight=m_SizeGameCard.cy;

		//�滭�˿�
		m_ImageGameCard.DrawImage(pDC,m_SendCardCurPos.x,m_SendCardCurPos.y,nItemWidth,nItemHeight,nItemWidth*2,nItemHeight*4);
	}

	//�û�����
	if ( m_wActionUser != INVALID_CHAIR && m_lOverScore[m_wActionUser] == 0 )
	{
		if(m_cbUserAction == AC_SHOW_HAND)
		{
			m_ImageShowHand.DrawImage(pDC,m_ptUserAction[m_wActionUser].x,m_ptUserAction[m_wActionUser].y+25);
		}
		else
		{
			//�׿�
			INT nIndex =(m_wActionUser>1?0:1);
			m_ImageActionBack.DrawImage(pDC,m_ptUserAction[m_wActionUser].x,m_ptUserAction[m_wActionUser].y,m_ImageActionBack.GetWidth()/2,m_ImageActionBack.GetHeight(),
				nIndex*m_ImageActionBack.GetWidth()/2,0);

			static BYTE cbAction[]={AC_GIVE_UP,AC_FOLLOW,AC_NO_ADD,AC_ADD_SCORE};

			for(BYTE i=0;i<CountArray(cbAction);i++)
			{
				if(m_cbUserAction == cbAction[i])
				{
					m_ImageActionFont.DrawImage(pDC,m_ptUserAction[m_wActionUser].x+15,m_ptUserAction[m_wActionUser].y+15,m_ImageActionFont.GetWidth()/4,m_ImageActionFont.GetHeight(),
						i*m_ImageActionFont.GetWidth()/4,0);
					break;
				}
			}
		}

	}
	
	// ������Ϣ
	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		if( m_lOverScore[i] > 0.001f || m_lOverScore[i] < -0.001f )
		{
			DrawOverScore(pDC, m_lOverScore[i], m_ptOverScore[i].x, m_ptOverScore[i].y);
		}

		//��ȡ�û�
		IClientUserItem * pIClientUserItem = GetClientUserItem(i);

		if ( (m_lOverScore[i] > 0.001f || m_lOverScore[i] < -0.001f)  && m_CardControl[i].GetCardCount() == MAX_CARD_COUNT && m_CardControl[i].GetDisplayHead()
			&& ( pIClientUserItem == NULL || pIClientUserItem->GetUserStatus() != US_READY))
		{
			BYTE cbCrad[MAX_CARD_COUNT] = {0};
			m_CardControl[i].GetCardData( cbCrad, MAX_CARD_COUNT );

			//����λ��
			CPoint ptCenterPoint;
			m_CardControl[i].GetCenterPoint(ptCenterPoint);
			INT nXPos = ptCenterPoint.x - m_ImageCradType.GetWidth()/18;
			INT nYPos = ptCenterPoint.y - m_ImageCradType.GetHeight()/2;

			//�滭����
			m_GameLogic.SortCardList( cbCrad, MAX_CARD_COUNT);
			m_ImageCradType.DrawImage(pDC,nXPos, nYPos ,
				m_ImageCradType.GetWidth() / 9,	m_ImageCradType.GetHeight(),
				(m_GameLogic.GetCardGenre( cbCrad, (BYTE)m_CardControl[i].GetCardCount() ) - 1) * m_ImageCradType.GetWidth() / 9, 0);
		}
	}
	
	//׼����־
	for( int i = 0; i < GAME_PLAYER; ++i )
	{
//#ifdef DEBUG
//		m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
//#endif
		//��ȡ�û�
		IClientUserItem * pIClientUserItem=GetClientUserItem(i);
		if (pIClientUserItem == NULL) continue;

		if (pIClientUserItem->GetUserStatus() == US_READY)
			m_ImageReady.DrawImage(pDC, m_ptReady[i].x - m_ImageReady.GetWidth()/2, m_ptReady[i].y - m_ImageReady.GetHeight()/2);
		if(pIClientUserItem->GetUserStatus() ==US_OFFLINE)
			m_PngOffLine.DrawImage(pDC,m_ptOffLine[i].x,m_ptOffLine[i].y);
	}

	// ��������
	m_PlayOperate.DrawPlayOperate(this,pDC,nWidth,nHeight);

	return;
}

//�Ƿ���
bool CGameClientView::IsDispatchStatus()
{
	return m_SendCardItemArray.GetCount()>0;
}

//ֹͣ����
VOID CGameClientView::ConcludeDispatch()
{
	//����ж�
	if (m_SendCardItemArray.GetCount()==0) return;

	//ɾ������
	KillTimer(IDI_SEND_CARD);

	//��ɶ���
	for (INT_PTR i=0;i<m_SendCardItemArray.GetCount();i++)
	{
		//��ȡ����
		tagSendCardItem * pSendCardItem=&m_SendCardItemArray[i];

		WORD wChairID=pSendCardItem->wChairID;
		m_CardControl[wChairID].SetCardData(m_cbUserCard[wChairID],m_cbUserCardCount[wChairID]);

	}

	m_SendCardItemArray.RemoveAll();

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//�����˿�
VOID CGameClientView::DispatchUserCard(WORD wChairID, BYTE cbCardData)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return;
	
	if(m_cbUserCardCount[wChairID]<MAX_COUNT)
	{
		m_cbUserCard[wChairID][m_cbUserCardCount[wChairID]++] = cbCardData;
	}

	//���ñ���
	tagSendCardItem SendCardItem;
	SendCardItem.wChairID=wChairID;
	SendCardItem.cbCardData=cbCardData;
	m_SendCardItemArray.Add(SendCardItem);

	//��������
	if (m_SendCardItemArray.GetCount()==1) 
	{
		//���ö�ʱ��
		SetTimer(IDI_SEND_CARD,SPEED_SEND_CARD,NULL);

		//
		m_nStepCount = SEND_STEP_COUNT;
		m_SendCardCurPos = m_SendCardPos;
		m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
		m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

		InvalidGameView(0,0,0,0);

		//��������
#ifndef  DEBUG
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
#endif
	}

	return;
}
VOID CGameClientView::SetUserCard(WORD wViewID,BYTE cbCardData[MAX_COUNT],BYTE cbCount)
{
	if(wViewID != INVALID_CHAIR)
	{
		m_cbUserCardCount[wViewID]=cbCount;
		for(BYTE i =0;i<cbCount;i++)
		{
			m_cbUserCard[wViewID][i]=cbCardData[i];
		}
	}
	else
	{
		ZeroMemory(m_cbUserCard,sizeof(m_cbUserCard));
		ZeroMemory(m_cbUserCardCount,sizeof(m_cbUserCardCount));	
	}
}

//��Ϸ��ע
bool CGameClientView::SetCellScore(SCORE lCellScore)
{
	m_PlayOperate.SetCellScore(lCellScore);

	if (abs(m_lCellScore-lCellScore)>0.001f)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//�����ע
bool CGameClientView::SetDrawMaxScore(SCORE lDrawMaxScore)
{
	if (abs(m_lDrawMaxScore-lDrawMaxScore)>0.001f)
	{
		//���ñ���
		m_lDrawMaxScore=lDrawMaxScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���û���
bool CGameClientView::SetUserScore(WORD wChairID, SCORE lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (abs(m_lUserScore[wChairID] - lUserScore)>0.001f)
	{
		m_lUserScore[wChairID] = lUserScore;
		m_lUserSaveScore[wChairID] = lUserScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���û���
bool CGameClientView::SetUserSaveScore(WORD wChairID, SCORE lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (abs(m_lUserSaveScore[wChairID] - lUserScore)>0.001f)
	{
		m_lUserAddScore[wChairID] = 0;
		m_lUserAddAllScore[wChairID] = lUserScore - m_lUserScore[wChairID];
		m_lUserSaveScore[wChairID] = lUserScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return true;
}

//���ý�������
bool CGameClientView::SetOverScore(WORD wChairID, SCORE lUserScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���û���
	if (abs(m_lOverScore[wChairID]-lUserScore)>0.001f)
	{
		m_lOverScore[wChairID] = SCORE_ZERO;
		m_lOverSaveScore[wChairID] = lUserScore;
	}

	return true;
}

//������ע
void CGameClientView::SetUserTableScore(WORD wChairID, SCORE lTableScore)
{
	//��������
	if (wChairID!=INVALID_CHAIR) m_lTableScore[wChairID]=lTableScore;
	else ZeroMemory(m_lTableScore,sizeof(m_lTableScore));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}

//���ö���
bool CGameClientView::SetUserAction(WORD wActionUser, BYTE cbUserAction)
{
	//���ö���
	if ((m_wActionUser!= wActionUser)||(m_cbUserAction!=cbUserAction))
	{
		//���ñ���
		m_wActionUser=wActionUser;
		m_cbUserAction=cbUserAction;

		//���½���
		InvalidGameView(0,0,0,0);

		//����ʱ��
		if (m_wActionUser!=INVALID_CHAIR) SetTimer(IDI_USER_ACTION,3000,NULL);
	}

	return true;
}

// �滭����
void CGameClientView::DrawNumber( CDC * pDC, CPngImage* ImageNumber, TCHAR * szImageNum, SCORE lOutNum,INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);
	DrawNumber(pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos, uFormat);
}

// �滭����
void CGameClientView::DrawNumber(CDC * pDC , CPngImage* ImageNumber, TCHAR * szImageNum, TCHAR* szOutNum ,INT nXPos, INT nYPos,  UINT uFormat /*= DT_LEFT*/)
{
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
				ImageNumber->DrawImage(pDC, nXPos, nYPos, nNumberWidth, nNumberHeight, j * nNumberWidth, 0, nNumberWidth, nNumberHeight);
				nXPos += nNumberWidth;
				break;
			}
		}
	}
}


// �滭��������
void CGameClientView::DrawOverScore( CDC * pDC, SCORE lOutNum, INT nXPos, INT nYPos )
{
	if( lOutNum < 0.001f && lOutNum > -0.001f )
		return;

	CPngImage* ImageNumber = NULL;
	TCHAR szOutNum[128] = {0};
	TCHAR szImageNum[128] = { TEXT("+-.0123456789") };
	if ( lOutNum >0.0 )
		_sntprintf(szOutNum,CountArray(szOutNum),TEXT("+")SCORE_STRING,lOutNum);
	else
		_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);

	if ( lOutNum >0.0 )
		ImageNumber = &m_ImgaeScoreWin;
	else
		ImageNumber = &m_ImgaeScoreLose;

	INT nNumberHeight = ImageNumber->GetHeight();
	INT nNumberWidth  = ImageNumber->GetWidth() / lstrlen(szImageNum);

	int nBackX = nXPos - ((INT)(((DOUBLE)(lstrlen(szOutNum)) / 2.0) * nNumberWidth) ) + 35;
	int nBackY = nYPos;
	int nBackWidth = (int)lstrlen(szOutNum) * nNumberWidth - 35;

	m_ImgaeScoreBackL.DrawImage( pDC, nBackX - 50, nBackY);
	for( int i = 0; i < nBackWidth; i += m_ImgaeScoreBackM.GetWidth() )
	{
		m_ImgaeScoreBackM.DrawImage( pDC, nBackX, nBackY);
		nBackX += m_ImgaeScoreBackM.GetWidth();
	}
	m_ImgaeScoreBackR.DrawImage( pDC, nBackX, nBackY);

	DrawNumber( pDC, ImageNumber, szImageNum, szOutNum, nXPos, nYPos + 3, DT_CENTER);
}


//��ʼ��ť
VOID CGameClientView::OnBnClickedStart()
{
	//������Ϣ
	SendEngineMessage(IDM_START,0,0);

	return;
}
//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��Ϣ����
	switch (nIDEvent)
	{
	case IDI_HIDE_CARD:		//�����˿�
		{
			//ɾ��ʱ��
			KillTimer(IDI_HIDE_CARD);

			//�����ж�
			IClientUserItem * pIClientUserItem=GetClientUserItem(MYSELF_VIEW_ID);
			if( pIClientUserItem == NULL )
				return;

			if ( m_lOverScore[MYSELF_VIEW_ID]> -0.001f &&m_lOverScore[MYSELF_VIEW_ID]<0.001f && m_CardControl[MYSELF_VIEW_ID].GetDisplayHead() && m_CardControl[MYSELF_VIEW_ID].GetPositively() && pIClientUserItem->GetUserStatus() == US_PLAYING )
			{
				//���ÿؼ�
				m_CardControl[MYSELF_VIEW_ID].SetDisplayHead(false);

				//���½���
				InvalidGameView(0,0,0,0);
			}

			return;
		}
	case IDI_SEND_CARD:		//���ƶ���
		{
			//����λ��
			m_SendCardCurPos.x += m_nXStep;
			m_SendCardCurPos.y += m_nYStep;

			//���½���
			InvalidGameView(0,0,0,0);

			//ֹͣ�ж�
			if ( --m_nStepCount == 0 )
			{
				//��ȡ�˿�
				WORD wChairID=m_SendCardItemArray[0].wChairID;

				m_CardControl[wChairID].SetCardData(m_cbUserCard[wChairID],__min((m_CardControl[wChairID].GetCardCount()+1),m_cbUserCardCount[wChairID]));

				//ɾ������
				m_SendCardItemArray.RemoveAt(0);

				//��������
				if (m_SendCardItemArray.GetCount()>0)
				{
					//
					wChairID = m_SendCardItemArray[0].wChairID;
					m_nStepCount = SEND_STEP_COUNT;
					m_SendCardCurPos = m_SendCardPos;
					m_nXStep = (m_CardControl[wChairID].GetTailPos().x-m_SendCardPos.x)/m_nStepCount;
					m_nYStep = (m_CardControl[wChairID].GetTailPos().y-m_SendCardPos.y)/m_nStepCount;

					//��������
#ifndef  DEBUG
					CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
					pGlobalUnits->PlayGameSound(AfxGetInstanceHandle(),TEXT("SEND_CARD"));
#endif
					return;
				}
				else 
				{
					//��ɴ���
					KillTimer(IDI_SEND_CARD);
					//������Ϣ
					SendEngineMessage(IDM_SEND_CARD_FINISH,0,0);

					return;
				}
			}

			return;
		}
	case IDI_USER_ACTION:	//�û�����
		{
			//���ñ���
			m_cbUserAction=0;
			m_wActionUser=INVALID_CHAIR;

			//���½���
			InvalidGameView(0,0,0,0);

			return;
		}
	case IDI_MOVE_CHIP:
		{
			BOOL bUpdateGameView = FALSE;
			for ( int i = 0 ; i < m_ArrayBetChip.GetCount(); ++i )
			{
				if( m_ArrayBetChip[i]->Move() )
				{
					bUpdateGameView = TRUE;
				}
			}

			int nIndex = 0;
			while ( nIndex < m_ArrayPlayChip.GetCount())
			{
				if( m_ArrayPlayChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					bUpdateGameView = TRUE;

					// ��������
					SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
					SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
					// ��ӷ���
					m_lPalyBetChip[m_ArrayPlayChip[nIndex]->wChipChairID] += lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];
					
					// ɾ������
					SafeDelete(m_ArrayPlayChip[nIndex]);
					m_ArrayPlayChip.RemoveAt(nIndex);

				}
			}
			
			// ��ӻ���
			if ( m_ArrayOverChip.GetCount() )
			{
				// ����λ��
				WORD   wChairID = m_ArrayOverChip[0]->wChipChairID;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );

				// ��ӻ���
				if ( m_ArrayBetChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayBetChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayBetChip[0]);
						m_ArrayBetChip.RemoveAt(0);
						nMaxIndex--;
					}

				}

				// ��ӻ���
				if ( m_ArrayPlayChip.GetCount() )
				{
					INT_PTR nMaxIndex = m_ArrayPlayChip.GetCount()/5;
					if ( nMaxIndex <= 0 )
						nMaxIndex = 1;

					while ( nMaxIndex > 0 )
					{
						stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayPlayChip[0]->wChipIndex, wChairID, 10, m_ArrayPlayChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
						m_ArrayOverChip.Add(pMoveChip);
						SafeDelete(m_ArrayPlayChip[0]);
						m_ArrayPlayChip.RemoveAt(0);
						nMaxIndex--;
					}
				}
				

				// ��ӻ���
				SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
				SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
				for ( int nSite = 0; nSite < GAME_PLAYER; ++nSite )
				{
					int nChipCount = 0;
					while ( m_lPalyBetChip[nSite] >= lScoreArray[0] )
					{
						for( int j = CountArray(lScoreArray) - 1; j >=0; j-- )
						{
							if ( m_lPalyBetChip[nSite] >= lScoreArray[j] )
							{	
								CPoint ptBegin(m_ptPalyBetChip[nSite]);
								CPoint ptEnd(ptBenchmarkPos);
								ptEnd.y -= nChipCount * 5;
								ptBegin.y -= nChipCount * 5;
								stuMoveChip* pMoveChip = new stuMoveChip(j, wChairID, 10, ptBegin, ptEnd, FALSE);
								if ( nChipCount >= PILING_HEIGHT )
									pMoveChip->SetShow(FALSE);
								
								m_ArrayOverChip.Add(pMoveChip);
								m_lPalyBetChip[nSite] -= lScoreArray[j];
								nChipCount++;
								break;
							}
						}
					}
				}
			}

			// �ƶ����ն���
			nIndex = 0;
			while ( nIndex < m_ArrayOverChip.GetCount())
			{
				if( m_ArrayOverChip[nIndex]->Move() )
				{
					bUpdateGameView = TRUE;
					nIndex++;
				}
				else
				{
					// ��������
					WORD wChipChairID = m_ArrayOverChip[nIndex]->wChipChairID;
					SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
					SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
					// ��ӷ���
					m_lUserScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];
					m_lUserAddScore[wChipChairID] += lScoreArray[m_ArrayOverChip[nIndex]->wChipIndex];

					if ( m_lUserAddScore[wChipChairID]-m_lUserAddAllScore[wChipChairID]>0.001f  )
						m_lUserAddScore[wChipChairID] = m_lUserAddAllScore[wChipChairID];


					for ( int i = 0; i < GAME_PLAYER; ++i )
					{
						if ( (m_lUserAddAllScore[i] >-0.001f&&m_lUserAddAllScore[i] <0.001f) || 
							(m_lOverSaveScore[i] >-0.001f&&m_lOverSaveScore[i] <0.001f) )
							continue;

						m_lOverScore[i] =  ( m_lOverSaveScore[i] * m_lUserAddScore[wChipChairID] / m_lUserAddAllScore[wChipChairID]);
					}

					// ɾ������
					SafeDelete(m_ArrayOverChip[nIndex]);
					m_ArrayOverChip.RemoveAt(nIndex);

					bUpdateGameView = TRUE;

					if ( m_ArrayOverChip.GetCount() == 0 )
					{
						ZeroMemory(m_lUserScore,sizeof(m_lUserScore));
						memcpy(m_lOverScore, m_lOverSaveScore, sizeof(m_lOverScore));
					}
				}
			}


			//���½���
			if ( bUpdateGameView )
			{
				InvalidGameView( 0,0,0,0 );
			}
			
			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_Font.CreateFont(this, TEXT("����"), 12, 40);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_PlayOperate.Initialization(this);

	//��Դ����
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//���ư�ť
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);

	//���ư�ť
	m_btStart.SetButtonImage(IDB_BT_START,hResInstance,false,false);
	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btStart,TEXT("��ʼ��Ϸ"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btFollow,TEXT("��ע"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btGiveUp,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btAddScore,TEXT("��ע"));
	m_ToolTipCtrl.AddTool(&m_PlayOperate.m_btShowHand,TEXT("���"));


	// ���������ƶ�
	SetTimer(IDI_MOVE_CHIP, 30, NULL);

	return 0;
}

//�����Ϣ
VOID CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//for ( int i = 0 ; i < GAME_PLAYER; ++i )
	//{
	//	BYTE cbCrad[5] = { 0x09,0x19,0x29,0x0C,0x0A };
	//	m_CardControl[i].SetCardData( cbCrad, 5);
	//}
	//
	//InvalidGameView(0,0,0,0);

	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˵��
	if (m_CardControl[MYSELF_VIEW_ID].OnEventLeftMouseDown(MousePoint)==true)
	{
		//���½���
		InvalidGameView(0,0,0,0);

		//����ʱ��
		if (m_CardControl[MYSELF_VIEW_ID].GetDisplayHead()==true)
		{
			SetTimer(IDI_HIDE_CARD,2000,NULL);
		}
		else KillTimer(IDI_HIDE_CARD);
	}

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�˿˲���
	if (m_CardControl[MYSELF_VIEW_ID].OnEventSetCursor(MousePoint)==true) return TRUE;

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//�滭�ַ�
bool CGameClientView::DrawTextStringEx(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, INT nDrawFormat)
{
	//��������
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};
	//INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_SINGLELINE;

	//�滭�߿�
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//����λ��
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//�滭�ַ�
		m_Font.DrawText(pDC, pszString,&rcFrame, crFrame, nDrawFormat);
	}

	//�滭�ַ�
	m_Font.DrawText(pDC, pszString,&rcRect, crText, nDrawFormat);

	return true;
}

// ���������ע
void CGameClientView::AddDeskChip( SCORE lScore, WORD wChairID )
{

	// ��������
	SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
		SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
	UINT	 nChipCount    = 0;

	// ɾ����ע
	if( wChairID <= GAME_PLAYER )
	{
		int nIndex = 0;
		while( nIndex < m_ArrayPlayChip.GetCount() )
		{	
			if( m_ArrayPlayChip[nIndex]->wChipChairID == wChairID )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = m_ArrayPlayChip[nIndex]->wChipIndex;
				nAllTime = 8;
				CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
				ptBeginPos.SetPoint( m_ArrayPlayChip[nIndex]->ptCurrentPos.x, m_ArrayPlayChip[nIndex]->ptCurrentPos.y);
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[m_ArrayPlayChip[nIndex]->wChipIndex];
				SafeDelete(m_ArrayPlayChip[nIndex]);
				m_ArrayPlayChip.RemoveAt(nIndex);
				continue;
			}
			nIndex++;
		}
	}

	// ��ȡ����
	nChipCount    = 0;
	while ( lScore >= lScoreArray[0] )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore -lScoreArray[i] >=-0.001f )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				if ( wChairID >= GAME_PLAYER )
				{
					wChipIndex = i;
					nAllTime = 0;
					ptBeginPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
					ptEndPos = ptBeginPos;
				}
				else
				{
					wChipIndex = i;
					nAllTime = 8;
					CPoint ptBenchmarkPos = m_ptPalyBetChip[wChairID];
					ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
					ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());
				}
				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;

				break;
			}
		}
	}
}


// ��������ע
void CGameClientView::AddPlayChip( SCORE lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	// ��������
	SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
		SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
	// ��ȡ����
	while ( lScore-lScoreArray[0] >=-0.001f  )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore -lScoreArray[i]>=-0.001f  )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 10;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y );
				ptEndPos.SetPoint( m_rectBetChip.left + rand()%m_rectBetChip.Width(), m_rectBetChip.top + rand()%m_rectBetChip.Height());

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, TRUE);
				m_ArrayBetChip.Add(pMoveChip);
				lScore -= lScoreArray[i];
				
				break;
			}
		}
	}
	
}

// �����Ҽ�ע
void CGameClientView::AddPlayBetChip( SCORE lScore, WORD wChairID )
{
	if( wChairID >= GAME_PLAYER )
		return;

	if ( lScore <= 0.001f && lScore >= -0.001f )
	{
		m_lPalyBetChip[wChairID] = lScore;
		return;
	}

	lScore -= m_lPalyBetChip[wChairID];

	// ��������
	SCORE lCellScore = ((m_lCellScore < 0.01) ? 0.01 : m_lCellScore);
		SCORE lScoreArray[] = { lCellScore, lCellScore * 2.000, lCellScore * 5.000, lCellScore * 10.000, lCellScore * 20.000 ,lCellScore * 50.000,lCellScore * 100.000 ,lCellScore*200,lCellScore*500.000 
								,lCellScore*1000.000,lCellScore*2000.000,lCellScore*5000.000,lCellScore*10000.000,lCellScore*20000.000};
	UINT	 nChipCount    = 0;

	// ��ȡ����
	while ( lScore-lScoreArray[0] >=-0.001f  )
	{
		for( int i = CountArray(lScoreArray) - 1; i >=0; i-- )
		{
			if ( lScore -lScoreArray[i]>=-0.001f  )
			{
				WORD	wChipIndex; 
				UINT	nAllTime; 
				DPOINT	ptBeginPos; 
				DPOINT	ptEndPos;

				wChipIndex = i;
				nAllTime = 15;
				CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
				ptBenchmarkPos.Offset( 25, 25 );
				ptBeginPos.SetPoint( ptBenchmarkPos.x, ptBenchmarkPos.y - nChipCount * 5);
				ptEndPos.SetPoint( m_ptPalyBetChip[wChairID].x, m_ptPalyBetChip[wChairID].y - nChipCount * 5);

				stuMoveChip* pMoveChip = new stuMoveChip(wChipIndex, wChairID, nAllTime, ptBeginPos, ptEndPos, FALSE);
				m_ArrayPlayChip.Add(pMoveChip);
				lScore -= lScoreArray[i];

				if ( nChipCount < PILING_HEIGHT )
					nChipCount++;
				break;
			}
		}
	}
}

// �������
void CGameClientView::ChioRecovery( WORD wChairID )
{
	// ����λ��
	CPoint ptBenchmarkPos = m_ptAvatar[wChairID];
	ptBenchmarkPos.Offset( 25, 25 );

	// ��ӻ���
	if ( m_ArrayBetChip.GetCount() )
	{
		stuMoveChip* pMoveChip = new stuMoveChip(m_ArrayBetChip[0]->wChipIndex, wChairID, 10, m_ArrayBetChip[0]->ptCurrentPos, ptBenchmarkPos, FALSE);
		m_ArrayOverChip.Add(pMoveChip);
		SafeDelete(m_ArrayBetChip[0]);
		m_ArrayBetChip.RemoveAt(0);
	}
}

// ��Ϣ����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(m_PlayOperate.OnCommand(wParam,lParam)==TRUE)
	{
		CRect rect = m_PlayOperate.GetOperateRect();
		InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		return TRUE;
	}
	return CGameFrameView::OnCommand(wParam, lParam);
}
	//������Ϣ
void CGameClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_PlayOperate.IsWindowVisible())
	{
		if(m_PlayOperate.OnKeyDown( nChar,  nRepCnt,  nFlags)==TRUE)
		{
			CRect rect = m_PlayOperate.GetOperateRect();
			InvalidGameView(rect.left, rect.top, rect.Width(), rect.Height());
		}
	}

	__super::OnKeyDown( nChar,  nRepCnt,  nFlags);
}
// ��Ӷ���
CString CGameClientView::AddComma( LONGLONG lScore )
{
	CString strScore;
	CString strReturn;
	strScore.Format(TEXT("%I64d"), lScore);

	int nStrCount = 0;
	for( int i = strScore.GetLength() - 1; i >= 0; )
	{
		if( (nStrCount%4) == 0 && nStrCount != 0 )
		{
			strReturn.Insert(0, ',');
			nStrCount = 0;
		}
		else
		{
			strReturn.Insert(0, strScore.GetAt(i));
			nStrCount++;
			i--;
		}
	}
	return strReturn;
}

void CGameClientView::DrawUserClockEx(CDC *pDC,INT nXPos, INT nYPos,WORD wChairID)
{
	WORD wTime=GetUserClock(wChairID);
	if(wTime !=0)
		m_ImageClock.DrawImage(pDC,nXPos,nYPos,m_ImageClock.GetWidth()*wTime/30,m_ImageClock.GetHeight(),0,0);
}

//////////////////////////////////////////////////////////////////////////
void CGameClientView::OpenAdminWnd()
{
	//����д�Ȩ��
	CGameClientEngine *pGameClientDlg=(CGameClientEngine*)CGameFrameEngine::GetInstance();
	IClientKernel *pIClientKernel=(IClientKernel *)pGameClientDlg->m_pIClientKernel;
	if(CUserRight::IsGameCheatUser(pIClientKernel->GetUserAttribute()->dwUserRight))
	{
		if (NULL==m_AdminDlg.m_hWnd) m_AdminDlg.Create(IDD_DIALOG_ADMIN,this);

		if(!m_AdminDlg.IsWindowVisible()) m_AdminDlg.ShowWindow(SW_SHOW);
		else m_AdminDlg.ShowWindow(SW_HIDE);
	}
}