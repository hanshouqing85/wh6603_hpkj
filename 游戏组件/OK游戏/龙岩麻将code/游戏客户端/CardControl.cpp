#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define POS_SHOOT					5									//��������
#define POS_SPACE					8									//�ָ����
#define ITEM_COUNT					43									//������Ŀ
#define INVALID_ITEM				0xFFFF								//��Ч����

//�˿˴�С
#define CARD_WIDTH					47									//�˿˿��
#define CARD_HEIGHT					72									//�˿˸߶�

//�滭ƫ��,���ڶ���,�����ƵĻ滭
#define Y_USER_LEFT_EXCUSION		18									//��ʾ����
#define Y_USER_RIGHT_EXCUSION		18									//��ʾ����
#define Y_TABLE_LEFT_EXCUSION		20									//��ʾ����
#define Y_TABLE_BOTTOM_EXCUSION		26									//��ʾ����
#define Y_TABLE_RIGHT_EXCUSION		20									//��ʾ����
#define Y_TABLE_TOP_EXCUSION		25									//��ʾ����
#define Y_HEAP_DOUBLE_V_EXCUSION	18									//��ʾ����

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardListImage::CCardListImage()
{
	//λ�ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;

	return;
}

//��������
CCardListImage::~CCardListImage()
{
}

//������Դ
bool CCardListImage::LoadResource(HINSTANCE hInstance,LPCTSTR pszResourceCard,LPCTSTR pszResourceShadow)
{
	//������Դ
	m_CardListImage.LoadImage( hInstance,pszResourceCard );
	m_ImageCardShadow.LoadImage( hInstance,pszResourceShadow );

	//���ñ���
	m_nItemHeight=m_CardListImage.GetHeight();
	m_nItemWidth=m_CardListImage.GetWidth()/ITEM_COUNT;

	return true;
}

//�ͷ���Դ
bool CCardListImage::DestroyResource()
{
	//���ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;

	//�ͷ���Դ
	m_CardListImage.DestroyImage();
	m_ImageCardShadow.DestroyImage();

	return true;
}

//��ȡλ��
int CCardListImage::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}

//�滭�˿�
bool CCardListImage::DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest, bool bShadow)
{
	//Ч��״̬
	ASSERT(m_CardListImage.IsNull()==false);
	ASSERT((m_nItemWidth!=0)&&(m_nItemHeight!=0));

	//�滭����
	if(cbCardData<=0x3F)
	{
		int nImageXPos=GetImageIndex(cbCardData)*m_nItemWidth;
		//�滭��Ӱ
		if( bShadow )
			m_ImageCardShadow.DrawImage( pDestDC,xDest,yDest );
		m_CardListImage.DrawImage( pDestDC,xDest,yDest,m_nItemWidth,m_nItemHeight,nImageXPos,0 );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardResource::CCardResource()
{
}

//��������
CCardResource::~CCardResource()
{
}

//������Դ
bool CCardResource::LoadResource()
{
	//��������
	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�û��˿�
	m_ImageUserTop.LoadImage( hInstance,TEXT("CARD_USER_TOP") );
	m_ImageUserLeft.LoadImage( hInstance,TEXT("CARD_USER_LEFT") );
	m_ImageUserRight.LoadImage( hInstance,TEXT("CARD_USER_RIGHT") );
	m_ImageUserBottom.LoadResource( hInstance,TEXT("CARD_USER_BOTTOM"),TEXT("USER_BOTTOM_SHADOW") );
	m_ImageUserBtGrayed.LoadResource( hInstance,TEXT("CARD_USER_BOTTOM_GRAYED"),TEXT("USER_BOTTOM_SHADOW") );

	//�����˿�
	m_ImageTableTop.LoadResource( hInstance,TEXT("CARD_TABLE_TOP"),TEXT("TABLE_TOP_SHADOW") );
	m_ImageTableLeft.LoadResource( hInstance,TEXT("CARD_TABLE_LEFT"),TEXT("TABLE_LEFT_SHADOW") );
	m_ImageTableRight.LoadResource( hInstance,TEXT("CARD_TABLE_RIGHT"),TEXT("TABLE_RIGHT_SHADOW") );
	m_ImageTableBottom.LoadResource( hInstance,TEXT("CARD_TABLE_BOTTOM"),TEXT("TABLE_BOTTOM_SHADOW") );
	m_ImageTableBottomBig.LoadResource( hInstance,TEXT("CARD_USER_BOTTOMBIG"),TEXT("USER_BOTTOM_SHADOW") );

	//�ƶ��˿�
	m_ImageHeapSingleV.LoadImage( hInstance,TEXT("CARD_HEAP_SINGLE_V") );
	m_ImageHeapSingleH.LoadImage( hInstance,TEXT("CARD_HEAP_SINGLE_H") );
	m_ImageHeapDoubleV.LoadImage( hInstance,TEXT("CARD_HEAP_DOUBLE_V") );
	m_ImageHeapDoubleH.LoadImage( hInstance,TEXT("CARD_HEAP_DOUBLE_H") );

	//�˿���Ӱ
	m_ImageUserTopShadow.LoadImage( hInstance,TEXT("USER_TOP_SHADOW") );
	m_ImageUserLeftShadow.LoadImage( hInstance,TEXT("USER_LEFT_SHADOW") );
	m_ImageUserRightShadow.LoadImage( hInstance,TEXT("USER_RIGHT_SHADOW") );
	m_ImageHeapSingleVShadow.LoadImage( hInstance,TEXT("HEAP_SINGLE_V_SHADOW") );
	m_ImageHeapSingleHShadow.LoadImage( hInstance,TEXT("HEAP_SINGLE_H_SHADOW") );
	m_ImageHeapDoubleHShadow.LoadImage( hInstance,TEXT("HEAP_DOUBLE_H_SHADOW") );
	m_ImageHeapDoubleVShadow.LoadImage( hInstance,TEXT("HEAP_DOUBLE_V_SHADOW") );

	return true;
}

//������Դ
bool CCardResource::DestroyResource()
{
	//�û��˿�
	m_ImageUserTop.DestroyImage();
	m_ImageUserLeft.DestroyImage();
	m_ImageUserRight.DestroyImage();
	m_ImageUserBottom.DestroyResource();

	//�����˿�
	m_ImageTableTop.DestroyResource();
	m_ImageTableLeft.DestroyResource();
	m_ImageTableRight.DestroyResource();
	m_ImageTableBottom.DestroyResource();
	m_ImageTableBottomBig.DestroyResource();

	//�ƶ��˿�
	m_ImageHeapSingleV.DestroyImage();
	m_ImageHeapSingleH.DestroyImage();
	m_ImageHeapDoubleV.DestroyImage();
	m_ImageHeapDoubleH.DestroyImage();

	//�˿���Ӱ
	m_ImageUserTopShadow.DestroyImage();
	m_ImageUserLeftShadow.DestroyImage();
	m_ImageUserRightShadow.DestroyImage();
	m_ImageHeapSingleVShadow.DestroyImage();
	m_ImageHeapSingleHShadow.DestroyImage();
	m_ImageHeapDoubleHShadow.DestroyImage();
	m_ImageHeapDoubleVShadow.DestroyImage();

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CHeapCard::CHeapCard()
{
	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//�˿˱���
	m_wFullCount=HEAP_FULL_COUNT;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;
	ZeroMemory( m_cbCardData,sizeof(m_cbCardData) );

	return;
}

//��������
CHeapCard::~CHeapCard()
{
}

//�滭�˿�
void CHeapCard::DrawCardControl(CDC * pDC)
{
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				int nYExcusion = Y_HEAP_DOUBLE_V_EXCUSION;
				int nCardHeight = g_CardResource.m_ImageHeapDoubleV.GetHeight()-nYExcusion;

				//β���˿�
				nXPos = m_ControlPoint.x;
				nYPos = m_ControlPoint.y-(m_wFullCount/2*nYExcusion+nCardHeight)+m_wMinusLastCount/2*nYExcusion;
				WORD wCardIndex = (m_wMinusLastCount/2)*2;
				if (m_wMinusLastCount%2!=0)
				{
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos+10,true );
					else
					{
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos+3,nYPos+3 );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos+3 );
					}
					wCardIndex += 2;
					nYPos += nYExcusion;
				}

				//�м��˿�
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wDoubleIndex = (m_wMinusLastCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					g_CardResource.m_ImageHeapDoubleVShadow.DrawImage( pDC,nXPos,nYPos );
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos+5,false );
						else
							g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos+5 );
						if( m_cbCardData[wCardIndex] != 0 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						else
							g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos );
					}
					else
						g_CardResource.m_ImageHeapDoubleV.DrawImage( pDC,nXPos,nYPos );
					wCardIndex += 2;
					nYPos += nYExcusion;
				}
				
				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nYPos += 3;
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos+1,nYPos,true );
					else
					{
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos+3,nYPos );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleVShadow.GetWidth();
						int nShadowHeight = g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-g_CardResource.m_ImageHeapSingleV.GetHeight();
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							0,g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-nShadowHeight );
					}
				}
			}
			break;
		}
	case Direction_South:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;

				//β���˿�
				int nXExcusion = g_CardResource.m_ImageHeapDoubleH.GetWidth()-5;
				nYPos = m_ControlPoint.y;
				nXPos = m_ControlPoint.x-m_wFullCount/2*nXExcusion+m_wMinusLastCount/2*nXExcusion;
				WORD wCardIndex = (m_wMinusLastCount/2)*2;
				if (m_wMinusLastCount%2!=0)
				{
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos-1,nYPos+2,true );
					else
					{
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos-1,nYPos+2 );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos-1,nYPos+2 );
					}
					wCardIndex += 2;
					nXPos += nXExcusion;
				}

				//�м��˿�
				WORD wFinallyIndex=(m_wFullCount-m_wMinusHeadCount)/2;
				WORD wDoubleIndex = (m_wMinusLastCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					g_CardResource.m_ImageHeapDoubleHShadow.DrawImage( pDC,nXPos,nYPos );
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
							g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,false );
						else
							g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos,nYPos );
						if( m_cbCardData[wCardIndex] != 0 )
							g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						else
							g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos,nYPos );
					}
					else
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXPos,nYPos );
					wCardIndex += 2;
					nXPos += nXExcusion;
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					nYPos += 2;
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,true );
					else
					{
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nXExcusion;
						int nShadowHeight = g_CardResource.m_ImageHeapSingleHShadow.GetHeight();
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos-9,nShadowWidth,nShadowHeight,
							g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nShadowWidth,0 ); 
					}
				}
			}
			
			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;

				//ͷ���˿�
				int nYExcusion = Y_HEAP_DOUBLE_V_EXCUSION;
				nXPos = m_ControlPoint.x;
				nYPos = m_ControlPoint.y+m_wMinusHeadCount/2*nYExcusion;
				WORD wCardIndex = m_wFullCount-(m_wMinusHeadCount/2)*2-2;
				if (m_wMinusHeadCount%2!=0)
				{
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
					{
						g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,true );
					}
					else
					{
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos+3,nYPos+1 );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos+1 );
					}
					wCardIndex -= 2;
					nYPos += nYExcusion;
				}

				//�м��˿�
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wDoubleIndex = (m_wMinusHeadCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					g_CardResource.m_ImageHeapDoubleVShadow.DrawImage( pDC,nXPos,nYPos );
					//����з���
					if( m_cbCardData[wCardIndex] != 0 || m_cbCardData[wCardIndex+1] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,false );
						else 
							g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos );
						if( m_cbCardData[wCardIndex] != 0 )
							g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						else 
							g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos );
					}
					else
					{
						g_CardResource.m_ImageHeapDoubleV.DrawImage( pDC,nXPos,nYPos );
					}
					nYPos += nYExcusion;
					wCardIndex -= 2;
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nYPos += 3;
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,true );
					else
					{
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos+3,nYPos );
						g_CardResource.m_ImageHeapSingleV.DrawImage( pDC,nXPos+3,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleVShadow.GetWidth();
						int nShadowHeight = g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-g_CardResource.m_ImageHeapSingleV.GetHeight();
						g_CardResource.m_ImageHeapSingleVShadow.DrawImage( pDC,nXPos,nYPos,nShadowWidth,nShadowHeight,
							0,g_CardResource.m_ImageHeapSingleVShadow.GetHeight()-nShadowHeight );
					}
				}
			}
			
			break;
		}
	case Direction_North:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;

				//ͷ���˿�
				int nXExcusion = g_CardResource.m_ImageHeapDoubleH.GetWidth()-5;
				nYPos = m_ControlPoint.y;
				nXPos = m_ControlPoint.x+m_wMinusHeadCount/2*nXExcusion;
				WORD wCardIndex = m_wFullCount-(m_wMinusHeadCount/2)*2-2;
				if (m_wMinusHeadCount%2!=0)
				{
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos+1,nYPos+2,true );
					else
					{
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos+1,nYPos+2 );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos+1,nYPos+2 );
					}
					wCardIndex -= 2;
					nXPos += nXExcusion;
				}

				//�м��˿�
				WORD wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;
				WORD wDoubleIndex = (m_wMinusHeadCount+1)/2;
				for (WORD i=wDoubleIndex;i<wFinallyIndex;i++)
				{
					g_CardResource.m_ImageHeapDoubleHShadow.DrawImage( pDC,nXPos,nYPos );
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 || m_cbCardData[wCardIndex] != 0 )
					{
						if( m_cbCardData[wCardIndex+1] != 0 )
							g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos,nYPos,false );
						else 
							g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos,nYPos );
						if( m_cbCardData[wCardIndex] != 0 )
							g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_cbCardData[wCardIndex],nXPos,nYPos,false );
						else
							g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos,nYPos );
					}
					else
						g_CardResource.m_ImageHeapDoubleH.DrawImage( pDC,nXPos,nYPos );
					wCardIndex -= 2;
					nXPos += nXExcusion;
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nYPos += 2;
					//����з���
					if( m_cbCardData[wCardIndex+1] != 0 )
						g_CardResource.m_ImageTableTop.DrawCardItem( pDC,m_cbCardData[wCardIndex+1],nXPos-2,nYPos,true );
					else
					{
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos-2,nYPos );
						g_CardResource.m_ImageHeapSingleH.DrawImage( pDC,nXPos-2,nYPos );
					}
					if( wDoubleIndex < wFinallyIndex )
					{
						int nShadowWidth = g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nXExcusion;
						int nShadowHeight = g_CardResource.m_ImageHeapSingleHShadow.GetHeight();
						g_CardResource.m_ImageHeapSingleHShadow.DrawImage( pDC,nXPos,nYPos-9,nShadowWidth,nShadowHeight,
							g_CardResource.m_ImageHeapSingleHShadow.GetWidth()-nShadowWidth,0 );
					}
				}
			}
			
			break;
		}
	}

	return;
}

//�����˿�
bool CHeapCard::SetHeapCardInfo(WORD wMinusHeadCount, WORD wMinusLastCount)
{
	ASSERT( wMinusHeadCount+wMinusLastCount<=m_wFullCount );
	if( wMinusHeadCount + wMinusLastCount > m_wFullCount ) return false;

	//���ñ���
	m_wMinusHeadCount=wMinusHeadCount;
	m_wMinusLastCount=wMinusLastCount;

	return true;
}

//���÷���,wIndex����β��������ֻ������
void CHeapCard::SetCardData( WORD wIndex, BYTE cbCardData )
{
	ASSERT( wIndex < m_wFullCount );
	if( wIndex >= m_wFullCount ) return;

	m_cbCardData[wIndex] = cbCardData;
}

//���÷���
void CHeapCard::ResetCardData()
{
	ZeroMemory( m_cbCardData,sizeof(m_cbCardData) );
	return ;
}

//��ȡ����λ��
CPoint CHeapCard::GetDispatchCardPos( bool bTail )
{
	CPoint ptHead = m_ControlPoint;

	switch( m_CardDirection )
	{
	case Direction_East:	//��
		{
			int nCardHeight = g_CardResource.m_ImageHeapDoubleV.GetHeight()-Y_HEAP_DOUBLE_V_EXCUSION;
			int nYExcusion = 0;
			if( !bTail )
			{
				nYExcusion = (m_wMinusHeadCount+2)/2;
				ptHead.y -= nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION+nCardHeight;
				if( m_wMinusHeadCount%2 != 0 )	ptHead.y += 9;
			}
			else
			{
				nYExcusion = (m_wFullCount-m_wMinusLastCount+1)/2;
				ptHead.y -= nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION+nCardHeight;
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	case Direction_South:	//��
		{
			int nXExcusion = 0;
			if( !bTail )
			{
				nXExcusion = (m_wMinusHeadCount+2)/2;
				ptHead.x -= nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 9;
			}
			else
			{
				nXExcusion = (m_wFullCount-m_wMinusLastCount+1)/2;
				ptHead.x -= nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	case Direction_West:	//��
		{
			int nYExcusion = 0;
			if( !bTail )
			{
				nYExcusion = m_wMinusHeadCount/2;
				ptHead.y += nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION;
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 10;
			}
			else
			{
				nYExcusion = (m_wFullCount-m_wMinusLastCount)/2;
				ptHead.y += nYExcusion*Y_HEAP_DOUBLE_V_EXCUSION;
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 10;
			}
		}
		break;
	case Direction_North:	//��
		{
			int nXExcusion = 0;
			if( !bTail )
			{
				nXExcusion = m_wMinusHeadCount/2;
				ptHead.x += nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusHeadCount%2 != 0 ) ptHead.y += 9;
			}
			else
			{
				nXExcusion = (m_wFullCount-m_wMinusLastCount)/2;
				ptHead.x += nXExcusion*g_CardResource.m_ImageHeapDoubleH.GetWidth();
				if( m_wMinusLastCount%2 != 0 ) ptHead.y += 9;
			}
		}
		break;
	}

	return ptHead;
}

//���ƶ��Ƴ���,bTailָ�Ƿ����β�Ƴ�
bool CHeapCard::DeductionHeapCard( bool bTail )
{
	if( m_wMinusHeadCount+m_wMinusLastCount >= m_wFullCount ) return false;

	if( !bTail )
		m_wMinusHeadCount++;
	else m_wMinusLastCount++;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CWeaveCard::CWeaveCard()
{
	//״̬����
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_ControlPointNew.SetPoint(0,0);
	m_CardDirection=Direction_South;
	m_cbAction = WIK_NULL;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));

	return;
}

//��������
CWeaveCard::~CWeaveCard()
{
}

//�滭�˿�
void CWeaveCard::DrawCardControl(CDC * pDC, bool bShadow,bool bBig)
{
	//��ʾ�ж�
	if (m_wCardCount==0) return;

	//��������
	int nXScreenPos=0,nYScreenPos=0;

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//��������
			int nYExcusion = Y_TABLE_LEFT_EXCUSION-3;

			nXScreenPos = m_ControlPointNew.x;
			nYScreenPos = m_ControlPointNew.y;

		
			//��һ�˿�
			if(m_cbAction==WIK_LEFT)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos+5,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}
			//�ڶ��˿�
			if(m_cbAction==WIK_CENTER)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos+5,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}

			//�����˿�
			if(m_cbAction==WIK_RIGHT)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos+5,nYScreenPos,bShadow);

			}
			else
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);

			}
			
			//�����˿�
			if( m_wCardCount == 4 )
			{
				//����
				if(m_bDisplayItem==false)
					g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,GetCardData(3),nXScreenPos,nYScreenPos-nYExcusion-12,bShadow );
				else
				{
					nYScreenPos += nYExcusion;
					g_CardResource.m_ImageTableLeft.DrawCardItem( pDC,GetCardData(3),nXScreenPos,nYScreenPos,bShadow );
				}
				
			}


			break;
		}
	case Direction_South:	//����
		{
			if(bBig==true)
			{
				//��������
			int nXExcusion = g_CardResource.m_ImageTableBottomBig.GetViewWidth()-5;

			nXScreenPos = m_ControlPointNew.x;
			nYScreenPos = m_ControlPointNew.y;

			//��һ�˿�
			if(m_cbAction==WIK_LEFT)
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos-10,bShadow);
				nXScreenPos += nXExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);
				nXScreenPos += nXExcusion;

			}

			//�ڶ��˿�
			if(m_cbAction==WIK_CENTER)
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos-10,bShadow);
				nXScreenPos += nXExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
				nXScreenPos += nXExcusion;

			}

			//�����˿�
			if(m_cbAction==WIK_RIGHT)
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos-10,bShadow);
			}
			else
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
			}

			//�����˿�
			if( m_wCardCount == 4 )
			{
				if(m_bDisplayItem==false)
					g_CardResource.m_ImageTableBottomBig.DrawCardItem( pDC,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-12,bShadow );
				else
				{
					nXScreenPos += nXExcusion;
					g_CardResource.m_ImageTableBottomBig.DrawCardItem( pDC,GetCardData(3),nXScreenPos,nYScreenPos,bShadow );
				}
			}
			}
			else
			{
				//��������
				int nXExcusion = g_CardResource.m_ImageTableBottom.GetViewWidth()-3;

				nXScreenPos = m_ControlPointNew.x;
				nYScreenPos = m_ControlPointNew.y;

				//��һ�˿�
				if(m_cbAction==WIK_LEFT)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos-10,bShadow);
					nXScreenPos += nXExcusion;
				}
				else
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);
					nXScreenPos += nXExcusion;

				}

				//�ڶ��˿�
				if(m_cbAction==WIK_CENTER)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos-10,bShadow);
					nXScreenPos += nXExcusion;
				}
				else
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
					nXScreenPos += nXExcusion;

				}

				//�����˿�
				if(m_cbAction==WIK_RIGHT)
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos-10,bShadow);

				}
				else
				{
					g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);


				}

				//�����˿�
				if( m_wCardCount == 4 )
				{
					if(m_bDisplayItem==false)
						g_CardResource.m_ImageTableBottom.DrawCardItem( pDC,GetCardData(3),nXScreenPos,nYScreenPos,bShadow );
					else
					{
						nXScreenPos += nXExcusion;
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,GetCardData(3),nXScreenPos,nYScreenPos,bShadow);

					}

				}
			}


			break;
		}
	case Direction_West:	//����
		{
			//��������
			int nYExcusion = Y_TABLE_RIGHT_EXCUSION-3;

			nXScreenPos = m_ControlPointNew.x;
			nYScreenPos = m_ControlPointNew.y-nYExcusion*2-g_CardResource.m_ImageTableRight.GetViewHeight();


			//�����˿�
			if( m_wCardCount == 4 )
			{
				if(m_bDisplayItem)
					g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos-nYExcusion,bShadow);

			}
			//�����˿�
			if(m_cbAction==WIK_RIGHT)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos-5,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;

			}

			//�ڶ��˿�
			if(m_cbAction==WIK_CENTER)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(1),nXScreenPos-5,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
				nYScreenPos += nYExcusion;

			}

			//��һ�˿�
			if(m_cbAction==WIK_LEFT)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos-5,nYScreenPos,bShadow);

			}
			else
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);


			}

			//�����˿�
			if( m_wCardCount == 4 )
			{
				if(m_bDisplayItem==false)
					g_CardResource.m_ImageTableRight.DrawCardItem( pDC,GetCardData(3),nXScreenPos,nYScreenPos-nYExcusion-12,bShadow );

			}

			break;
		}
	case Direction_North:	//����
		{
			//��������
			int nXExcusion = g_CardResource.m_ImageTableTop.GetViewWidth()-3;

			nXScreenPos = m_ControlPointNew.x-nXExcusion*3;
			nYScreenPos = m_ControlPointNew.y;

			//�����˿�
			if( m_wCardCount == 4 )
			{
				if(m_bDisplayItem)
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(0),nXScreenPos-nXExcusion,nYScreenPos,bShadow);
			}

			//�����˿�
			if(m_cbAction==WIK_RIGHT)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos+5,bShadow);
				nXScreenPos += nXExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(2),nXScreenPos,nYScreenPos,bShadow);
				nXScreenPos += nXExcusion;

			}

			//�ڶ��˿�
			if(m_cbAction==WIK_CENTER)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos+5,bShadow);
				nXScreenPos += nXExcusion;
			}
			else
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(1),nXScreenPos,nYScreenPos,bShadow);
				nXScreenPos += nXExcusion;

			}

			//��һ�˿�
			if(m_cbAction==WIK_LEFT)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos+5,bShadow);

			}
			else
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(0),nXScreenPos,nYScreenPos,bShadow);

			}

			//�����˿�
			if( m_wCardCount == 4 )
			{
				if(m_bDisplayItem==false)
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,GetCardData(3),nXScreenPos-nXExcusion,nYScreenPos-5,bShadow);
			}

			break;
		}
	}

	return;
}

//�����˿�
bool CWeaveCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	if(wCardCount==0)
	{

		m_ControlPointNew=m_ControlPoint;
	}

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(BYTE)*wCardCount);

	return true;
}

//��ȡ����
BYTE CWeaveCard::GetCardCount()
{
	if(m_bDisplayItem==false && m_wCardCount==4) 
		return 3;
	else
		return BYTE(m_wCardCount);
}

void CWeaveCard::OffPoint(bool bBig)
{
		//�滭�˿�
		switch (m_CardDirection)
		{
		case Direction_East:	//����
			{
				//��������
				m_ControlPointNew.Offset(0,Y_TABLE_LEFT_EXCUSION-3);
				break;
			}
		case Direction_South:	//����
			{
				if(bBig==true)
				{
					m_ControlPointNew.Offset( g_CardResource.m_ImageTableBottomBig.GetViewWidth()-5,0);
				}
				else
				{
					m_ControlPointNew.Offset(g_CardResource.m_ImageTableBottom.GetViewWidth()-3,0);

				}
				break;
			}
		case Direction_West:	//����
			{
				m_ControlPointNew.Offset(0,-Y_TABLE_RIGHT_EXCUSION+3);
				break;
			}
		case Direction_North:	//����
			{
				//��������
				m_ControlPointNew.Offset( -g_CardResource.m_ImageTableTop.GetViewWidth()+3,0);
				break;
			}
		}
}

//��ȡ�˿�
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));
	//��ʾ����
	if(m_wCardCount==4)
	{
		if(wIndex==3)
		{
			return m_cbCardData[wIndex];
		}
	}
	return m_cbCardData[wIndex];
}

//�滭�˿�
void CWeaveCard::DrawCardControl(CDC * pDC, int nXPos, int nYPos, bool bShadow,bool bBig)
{
	//����λ��
	SetControlPoint(nXPos,nYPos);

	DrawCardControl( pDC,bShadow ,bBig);

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserCard::CUserCard()
{
	//�˿�����
	m_wCardCount=0;
	m_bCurrentCard=false;

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CUserCard::~CUserCard()
{
}

//�滭�˿�
void CUserCard::DrawCardControl(CDC * pDC)
{
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			int nYExcusion = Y_USER_LEFT_EXCUSION;

			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y-nYExcusion*m_wCardCount-g_CardResource.m_ImageUserLeft.GetHeight()-8;

			//�����˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageUserLeftShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserLeft.DrawImage( pDC,nXPos,nYPos );
				nYPos += nYExcusion;
			}

			//��ǰ�˿�
			if( m_bCurrentCard )
			{
				g_CardResource.m_ImageUserLeftShadow.DrawImage( pDC,nXPos,nYPos+8 );
				g_CardResource.m_ImageUserLeft.DrawImage( pDC,nXPos,nYPos+8 );
			}

			break;
		}
	case Direction_West:	//����
		{
			int nYExcusion = Y_USER_RIGHT_EXCUSION;

			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y;

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				g_CardResource.m_ImageUserRightShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserRight.DrawImage( pDC,nXPos,nYPos );
			}
			nYPos += nYExcusion+10;

			//�����˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageUserRightShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserRight.DrawImage( pDC,nXPos,nYPos );
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_North:	//����
		{
			int nXExcusion = g_CardResource.m_ImageUserTop.GetWidth();

			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y;

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				g_CardResource.m_ImageUserTopShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserTop.DrawImage( pDC,nXPos,nYPos );
			}
			nXPos += nXExcusion+6;

			//�����˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageUserTopShadow.DrawImage( pDC,nXPos,nYPos );
				g_CardResource.m_ImageUserTop.DrawImage( pDC,nXPos,nYPos );
				nXPos += nXExcusion-3;
			}

			break;
		}
	}

	return;
}

//�����˿�
bool CUserCard::SetCurrentCard(bool bCurrentCard)
{
	//���ñ���
	m_bCurrentCard=bCurrentCard;

	return true;
}

//�����˿�
bool CUserCard::SetCardData(WORD wCardCount, bool bCurrentCard)
{
	//���ñ���
	m_wCardCount=wCardCount;
	m_bCurrentCard=bCurrentCard;

	return true;
}

//��ȡ����λ��
CPoint CUserCard::GetDispatchCardPos()
{
	CPoint ptTail = m_ControlPoint;
	if( m_CardDirection == Direction_East )
		ptTail.y -= g_CardResource.m_ImageUserLeft.GetHeight();
	return ptTail;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDiscardCard::CDiscardCard()
{
	//�˿�����
	m_cbCurrentCount = 0;
	m_cbCurrentMaxCount = 12;

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CDiscardCard::~CDiscardCard()
{
}

//�滭�˿�
void CDiscardCard::DrawCardControl(CDC * pDC)
{
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
			for( int i = 0; i < m_ItemArray.GetCount(); i++ )
			{
				tagDiscardItem &di = m_ItemArray[i];
				int nXPos=m_ControlPoint.x+di.ptExcusion.x;
				int nYPos=m_ControlPoint.y+di.ptExcusion.y;
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,true);
			}

			break;
		}
	case Direction_South:	//����
		{
			if( m_ItemArray.GetCount() == 0 ) break;

			//�滭�˿�
			int nDrawCount = 0;
			int nMaxCount = 12;
			while( nDrawCount+nMaxCount < m_ItemArray.GetCount() )
			{
				nDrawCount += nMaxCount;
				nMaxCount -= 2;
			}
			int nBeginIndex = nDrawCount;
			int nEndIndex = (int)m_ItemArray.GetCount()-1;
			while( nDrawCount >= 0 )
			{
				//�滭��
				for( ; nBeginIndex <= nEndIndex; nBeginIndex++ )
				{
					tagDiscardItem &di = m_ItemArray[nBeginIndex];
					int nXPos=m_ControlPoint.x+di.ptExcusion.x;
					int nYPos=m_ControlPoint.y+di.ptExcusion.y;
					if(m_bBig==true)
					{
						g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,true);

					}
					else
					{
						g_CardResource.m_ImageTableBottom.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,true);
					}
				}
				nEndIndex = nDrawCount-1;
				nMaxCount += 2;
				nBeginIndex = nDrawCount-nMaxCount;
				nDrawCount -= nMaxCount;
			};

			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			for( int i = (int)m_ItemArray.GetCount()-1; i >= 0; i-- )
			{
				tagDiscardItem &di = m_ItemArray[i];
				int nXPos=m_ControlPoint.x+di.ptExcusion.x;
				int nYPos=m_ControlPoint.y+di.ptExcusion.y;
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,true);
			}

			break;
		}
	case Direction_North:	//����
		{
			if( m_ItemArray.GetCount() == 0 ) break;

			//�滭�˿�
			int nMaxCount = 12;
			if( nMaxCount > m_ItemArray.GetCount() )
				nMaxCount = (int)m_ItemArray.GetCount();
			int nBeginIndex = nMaxCount-1;
			int nEndIndex = 0;
			int nDrawCount = 0;
			while( nDrawCount < m_ItemArray.GetCount() )
			{
				nDrawCount += nBeginIndex-nEndIndex+1;
				//�滭��
				for( ; nBeginIndex >= nEndIndex; nBeginIndex-- )
				{
					tagDiscardItem &di = m_ItemArray[nBeginIndex];
					int nXPos=m_ControlPoint.x+di.ptExcusion.x;
					int nYPos=m_ControlPoint.y+di.ptExcusion.y;
					g_CardResource.m_ImageTableTop.DrawCardItem(pDC,di.cbCardData,nXPos,nYPos,true);
				}
				nEndIndex = nDrawCount;
				nMaxCount -= 2;
				nBeginIndex = nDrawCount+nMaxCount-1;
				if( nDrawCount + nMaxCount > m_ItemArray.GetCount() )
					nBeginIndex = (int)m_ItemArray.GetCount()-1;
			}
			break;
		}
	}

	return;
}

//�����˿�
bool CDiscardCard::AddCardItem(BYTE cbCardData)
{
	//�������
	tagDiscardItem di;
	di.cbCardData = cbCardData;

	//��������
	//���������ǰ�������
	if( ++m_cbCurrentCount > m_cbCurrentMaxCount )
	{
		//����Ѿ��ŵ���С��
		if( m_cbCurrentMaxCount == 2 )
		{
			//�滻���һ����
			m_ItemArray[m_ItemArray.GetCount()-1].cbCardData = cbCardData;
			return true;
		}
		//���¿�ʼһ��
		m_cbCurrentMaxCount -= 2;
		m_cbCurrentCount = 1;
	}

	//����ƫ��
	CPoint pt;
	switch( m_CardDirection )
	{
	case Direction_East:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nItemHeight = Y_TABLE_LEFT_EXCUSION-3;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			pt.x = nItemWidth*nRowIndex;
			pt.y = (m_cbCurrentCount-1+nRowIndex)*nItemHeight;
		}
		break;
	case Direction_South:	//��
		{
			if(m_bBig==true)
			{
				int nItemWidth = g_CardResource.m_ImageTableBottomBig.GetViewWidth()-5;
				int nItemHeight = g_CardResource.m_ImageTableBottomBig.GetViewHeight();
				int nRowIndex = (12-m_cbCurrentMaxCount)/2;
				pt.x = (m_cbCurrentCount-1+nRowIndex)*nItemWidth;
				pt.y = -nItemHeight*nRowIndex;

			}
			else
			{

				int nItemWidth = g_CardResource.m_ImageTableBottom.GetViewWidth()-3;
				int nItemHeight = Y_TABLE_BOTTOM_EXCUSION;
				int nRowIndex = (12-m_cbCurrentMaxCount)/2;
				pt.x = (m_cbCurrentCount-1+nRowIndex)*nItemWidth;
				pt.y = -nItemHeight*nRowIndex;
			}
		}
		break;
	case Direction_West:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableRight.GetViewWidth();
			int nItemHeight = Y_TABLE_RIGHT_EXCUSION-3;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			pt.x = -nItemWidth*nRowIndex;
			pt.y = -(m_cbCurrentCount-1+nRowIndex)*nItemHeight - g_CardResource.m_ImageTableRight.GetViewHeight();
		}
		break;
	case Direction_North:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth()-3;
			int nItemHeight = Y_TABLE_TOP_EXCUSION;
			int nRowIndex = (12-m_cbCurrentMaxCount)/2;
			pt.x = -(m_cbCurrentCount+nRowIndex)*nItemWidth;
			pt.y = nItemHeight*nRowIndex;
		}
		break;
	}

	//����ƫ��
	di.ptExcusion = pt;

	//���
	m_ItemArray.Add( di );

	return true;
}

//���÷���
void CDiscardCard::SetDirection( enDirection Direction )
{
	m_CardDirection = Direction;

	//���õ�ǰ�������ʾ��Ŀ,ˮƽ�����һ��Ϊ12,�����𲽼�2,��ֱ�����һ��Ϊ10,�����𲽼�2
	if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
		m_cbCurrentMaxCount = 10;
	else m_cbCurrentMaxCount = 12;
}

//�����˿�
bool CDiscardCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	m_cbCurrentCount = 0;

	//���õ�ǰ�������ʾ��Ŀ,ˮƽ�����һ��Ϊ12,�����𲽼�2,��ֱ�����һ��Ϊ10,�����𲽼�2
	if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
		m_cbCurrentMaxCount = 10;
	else m_cbCurrentMaxCount = 12;

	m_ItemArray.RemoveAll();
	for( WORD i = 0; i < wCardCount; i++ )
		AddCardItem( cbCardData[i] );

	return true;
}

//��ȡλ��
CPoint CDiscardCard::GetLastCardPosition()
{
	if( m_ItemArray.GetCount() == 0 ) return CPoint(0,0);

	//����ƫ��
	CPoint pt;
	switch( m_CardDirection )
	{
	case Direction_East:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nItemHeight = Y_TABLE_LEFT_EXCUSION-3;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			pt.x = nItemWidth*nRowIndex+1;
			pt.y = (m_cbCurrentCount-1+nRowIndex)*nItemHeight-3;
		}
		break;
	case Direction_South:	//��
		{
			if(m_bBig==true)
			{
				int nItemWidth = g_CardResource.m_ImageTableBottomBig.GetViewWidth()-5;
				int nItemHeight = g_CardResource.m_ImageTableBottomBig.GetViewHeight();
				int nRowIndex = (12-m_cbCurrentMaxCount)/2;
				pt.x = (m_cbCurrentCount-1+nRowIndex)*nItemWidth;
				pt.y = -nItemHeight*nRowIndex;

			}
			else
			{

				int nItemWidth = g_CardResource.m_ImageTableBottom.GetViewWidth()-3;
				int nItemHeight = Y_TABLE_BOTTOM_EXCUSION;
				int nRowIndex = (12-m_cbCurrentMaxCount)/2;
				pt.x = (m_cbCurrentCount-1+nRowIndex)*nItemWidth-2;
				pt.y = -nItemHeight*nRowIndex;
			}
		}
		break;
	case Direction_West:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableRight.GetViewWidth();
			int nItemHeight = Y_TABLE_RIGHT_EXCUSION-3;
			int nRowIndex = (10-m_cbCurrentMaxCount)/2;
			pt.x = -nItemWidth*nRowIndex+1;
			pt.y = -(m_cbCurrentCount-1+nRowIndex)*nItemHeight - g_CardResource.m_ImageTableRight.GetViewHeight()-2;
		}
		break;
	case Direction_North:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth()-3;
			int nItemHeight = Y_TABLE_TOP_EXCUSION;
			int nRowIndex = (12-m_cbCurrentMaxCount)/2;
			pt.x = -(m_cbCurrentCount+nRowIndex)*nItemWidth-2;
			pt.y = nItemHeight*nRowIndex;
		}
		break;
	}

	pt.x += m_ControlPoint.x;
	pt.y += m_ControlPoint.y;

	return pt;
}

//��ȡ����λ��
CPoint CDiscardCard::GetDispatchCardPos()
{
	CPoint pt;

	//���������ǰ�������
	BYTE cbCurrentCount = m_cbCurrentCount;
	BYTE cbCurrentMaxCount = m_cbCurrentMaxCount;
	if( ++cbCurrentCount > cbCurrentMaxCount )
	{
		//����Ѿ��ŵ���С��
		if( cbCurrentMaxCount == 2 )
		{
			//�滻���һ����
			pt = m_ItemArray[m_ItemArray.GetCount()-1].ptExcusion;
			pt += m_ControlPoint;
			return pt;
		}
		//���¿�ʼһ��
		cbCurrentMaxCount -= 2;
		cbCurrentCount = 1;
	}

	//����ƫ��
	switch( m_CardDirection )
	{
	case Direction_East:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableLeft.GetViewWidth();
			int nItemHeight = Y_TABLE_LEFT_EXCUSION;
			int nRowIndex = (10-cbCurrentMaxCount)/2;
			pt.x = nItemWidth*nRowIndex;
			pt.y = (cbCurrentCount-1+nRowIndex)*nItemHeight;
		}
		break;
	case Direction_South:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableBottom.GetViewWidth();
			int nItemHeight = Y_TABLE_BOTTOM_EXCUSION;
			int nRowIndex = (12-cbCurrentMaxCount)/2;
			pt.x = (cbCurrentCount-1+nRowIndex)*nItemWidth;
			pt.y = -nItemHeight*nRowIndex;
		}
		break;
	case Direction_West:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableRight.GetViewWidth();
			int nItemHeight = Y_TABLE_RIGHT_EXCUSION;
			int nRowIndex = (10-cbCurrentMaxCount)/2;
			pt.x = -nItemWidth*nRowIndex;
			pt.y = -(cbCurrentCount-1+nRowIndex)*nItemHeight - g_CardResource.m_ImageTableRight.GetViewHeight();
		}
		break;
	case Direction_North:	//��
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth();
			int nItemHeight = Y_TABLE_TOP_EXCUSION;
			int nRowIndex = (12-cbCurrentMaxCount)/2;
			pt.x = -(cbCurrentCount+nRowIndex)*nItemWidth;
			pt.y = nItemHeight*nRowIndex;
		}
		break;
	}

	//�ӻ�׼λ��
	pt += m_ControlPoint;

	return pt;
}

//�Ƴ���
void CDiscardCard::RemoveCard( BYTE cbRemoveCount )
{
	ASSERT( cbRemoveCount <= m_ItemArray.GetCount() );
	if( cbRemoveCount > m_ItemArray.GetCount() ) return;

	INT nCount = (INT)m_ItemArray.GetCount();
	m_ItemArray.RemoveAt( nCount-cbRemoveCount,cbRemoveCount );

	if( m_cbCurrentCount < cbRemoveCount )
	{
		cbRemoveCount -= m_cbCurrentCount;
		if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
			m_cbCurrentMaxCount = __min( 10,m_cbCurrentMaxCount+2 );
		else m_cbCurrentMaxCount = __min( 12,m_cbCurrentMaxCount+2 );
		while( cbRemoveCount > m_cbCurrentMaxCount )
		{
			cbRemoveCount -= m_cbCurrentMaxCount;
			if( m_CardDirection == Direction_East || m_CardDirection == Direction_West )
				m_cbCurrentMaxCount = __min( 10,m_cbCurrentMaxCount+2 );
			else m_cbCurrentMaxCount = __min( 12,m_cbCurrentMaxCount+2 );
		}
		m_cbCurrentCount = m_cbCurrentMaxCount-cbRemoveCount;
	}
	else m_cbCurrentCount -= cbRemoveCount;

	return;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableCard::CTableCard()
{
	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	return;
}

//��������
CTableCard::~CTableCard()
{
}

//�滭�˿�
void CTableCard::DrawCardControl(CDC * pDC)
{
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			int nYExcusion = Y_TABLE_LEFT_EXCUSION;
			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y+(CountArray(m_cbCardData)-m_wCardCount)*nYExcusion;

			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableLeft.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos,true);
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_South:	//����
		{
			int nXExcusion = g_CardResource.m_ImageTableBottomBig.GetViewWidth()-5;
			int nXPos = m_ControlPoint.x+(CountArray(m_cbCardData)-m_wCardCount)*nXExcusion;
			int nYPos = m_ControlPoint.y;

			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableBottomBig.DrawCardItem(pDC,m_cbCardData[i],nXPos,nYPos,true);
				nXPos += nXExcusion;
			}

			break;
		}
	case Direction_West:	//����
		{
			int nYExcusion = Y_TABLE_RIGHT_EXCUSION;
			int nXPos = m_ControlPoint.x;
			int nYPos = m_ControlPoint.y-(CountArray(m_cbCardData)-1)*nYExcusion-g_CardResource.m_ImageTableRight.GetViewHeight();

			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableRight.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos,true);
				nYPos += nYExcusion;
			}

			break;
		}
	case Direction_North:	//����
		{
			int nItemWidth = g_CardResource.m_ImageTableTop.GetViewWidth();
			int nXPos = m_ControlPoint.x-CountArray(m_cbCardData)*nItemWidth;
			int nYPos = m_ControlPoint.y;

			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				g_CardResource.m_ImageTableTop.DrawCardItem(pDC,m_cbCardData[m_wCardCount-i-1],nXPos,nYPos,true);
				nXPos += nItemWidth;
			}

			break;
		}
	}

	return;
}

//�����˿�
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//״̬����
	m_bPositively=false;
	m_bDisplayItem=false;
	m_bSelectMode = false;
	m_cbSelectInfoCount = 0;
	m_cbCurSelectIndex = CountArray(m_SelectCardInfo);
	ZeroMemory( m_SelectCardInfo,sizeof(m_SelectCardInfo) );
	for( BYTE i = 0; i < CountArray(m_rcSelectCard); i++ )
		m_rcSelectCard[i].SetRectEmpty();

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//�˿�����
	m_wCardCount=0;
	m_wHoverItem=INVALID_ITEM;
	ZeroMemory(&m_CurrentCard,sizeof(m_CurrentCard));
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//��������
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy = CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;

	//��ӰͼƬ
	m_ImageCardShadow.LoadImage( AfxGetInstanceHandle(),TEXT("USER_BOTTOM_SHADOW") );

	return;
}

//��������
CCardControl::~CCardControl()
{
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��ȡ�����˿�
BYTE CCardControl::GetHoverCard()
{
	//��ȡ�˿�
	if (m_wHoverItem!=INVALID_ITEM)
	{
		if (m_wHoverItem==CountArray(m_CardItemArray))
			return m_CurrentCard.cbCardData;
		else return m_CardItemArray[m_wHoverItem].cbCardData;
	}

	return 0;
}

//�����˿�
bool CCardControl::SetCurrentCard(BYTE cbCardData)
{
	//���ñ���
	m_CurrentCard.bShoot=false;
	m_CurrentCard.bGrayed = false;
	m_CurrentCard.cbCardData=cbCardData;

	return true;
}

//�����˿�
bool CCardControl::SetCurrentCard(tagCardItem CardItem)
{
	//���ñ���
	m_CurrentCard = CardItem;

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) 
		return false;

	//��ǰ�˿�
	m_CurrentCard.bShoot=false;
	m_CurrentCard.bGrayed = false;
	m_CurrentCard.cbCardData=cbCurrentCard;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].bGrayed = false;
		m_CardItemArray[i].cbCardData=cbCardData[i];
	}

	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i] = CardItemArray[i];
	}

	return true;
}

//�滭�˿�
void CCardControl::DrawCardControl(CDC * pDC)
{
	//�滭׼��
	int nXExcursion=m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//����λ��
		int nXScreenPos=nXExcursion+CARD_WIDTH*i;
		int nYScreenPos=m_ControlPoint.y+(m_CardItemArray[i].bShoot==false?POS_SHOOT:0);

		//�滭��Ӱ
		m_ImageCardShadow.DrawImage( pDC,nXScreenPos+((m_CurrentCard.bShoot||m_wHoverItem==i)?3:0),
			m_ControlPoint.y+POS_SHOOT );

		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		if( !m_CardItemArray[i].bGrayed )
			g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos,false);
		else
			g_CardResource.m_ImageUserBtGrayed.DrawCardItem( pDC,cbCardData,nXScreenPos,nYScreenPos,false );
	}

	//��ǰ�˿�
	if (m_CurrentCard.cbCardData!=0)
	{
		//����λ��
		int nXScreenPos=m_ControlPoint.x+CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE;
		int nYScreenPos=m_ControlPoint.y+(m_CurrentCard.bShoot==false?POS_SHOOT:0);
		
		//�滭��Ӱ
		m_ImageCardShadow.DrawImage( pDC,nXScreenPos+(m_CurrentCard.bShoot?3:0),m_ControlPoint.y+POS_SHOOT );

		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
		if( !m_CurrentCard.bGrayed )
			g_CardResource.m_ImageUserBottom.DrawCardItem(pDC,cbCardData,nXScreenPos,nYScreenPos,false);
		else 
			g_CardResource.m_ImageUserBtGrayed.DrawCardItem( pDC,cbCardData,nXScreenPos,nYScreenPos,false );
	}

	return;
}

//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
	//��׼λ��
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) 
		return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) 
		return INVALID_ITEM;

	//��������
	if (nXPos<CARD_WIDTH*(int)CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray))
		{
			wViewIndex -= CountArray(m_CardItemArray);
			//������Ʋ��ǻ�ɫ״̬,�򷵻ظ�����
			if( !m_CardItemArray[wViewIndex].bGrayed )
				return wViewIndex;
		}
		return INVALID_ITEM;
	}

	//��ǰ����
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))&&!m_CurrentCard.bGrayed) 
		return CountArray(m_CardItemArray);

	return INVALID_ITEM;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point, CRect &rcRePaint)
{
	rcRePaint.SetRectEmpty();

	if( !m_bSelectMode && m_bPositively==true )
	{
		//��ȡ����
		WORD wHoverItem=SwitchCardPoint(Point);
		//��Ӧ�ж�
		if ((m_bPositively==false)&&(m_wHoverItem!=INVALID_ITEM))
		{
			m_wHoverItem=INVALID_ITEM;
		}
		if( wHoverItem != m_wHoverItem )
		{
			//��������
			CRect rcOld = GetCardRect(m_wHoverItem);
			CRect rcNew = GetCardRect(wHoverItem);
			rcRePaint.UnionRect( rcOld,rcNew );
			rcRePaint.InflateRect( 0,0,40,0 );
			rcRePaint.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );

			if( m_wHoverItem != INVALID_ITEM )
			{
				if( m_wHoverItem == CountArray(m_CardItemArray) )
					m_CurrentCard.bShoot = false;
				else m_CardItemArray[m_wHoverItem].bShoot = false;
			}
			m_wHoverItem=wHoverItem;
			if( m_wHoverItem != INVALID_ITEM )
			{
				if( m_wHoverItem == CountArray(m_CardItemArray) )
					m_CurrentCard.bShoot = true;
				else m_CardItemArray[m_wHoverItem].bShoot = true;
			}
		}

		if( wHoverItem!=INVALID_ITEM )
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
		else return false;
	}
	//�����ѡ��ģʽ��,����ǰѡ�����
	else if( m_bSelectMode && m_bPositively )
	{
		//�����ػ���
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
		{
			CRect rc = m_rcSelectCard[m_cbCurSelectIndex];
			rc.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
			rcRePaint = rc;
		}

		//��ȡ��ǰѡ��λ��
		BYTE cbCurIndex = CountArray(m_SelectCardInfo);
		for( BYTE i = 0; i < m_cbSelectInfoCount; i++ )
		{
			CRect rcValid = m_rcSelectCard[i];
			rcValid.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
			if( rcValid.PtInRect(Point) )
			{
				cbCurIndex = i;
				break;
			}
		}

		//���õ�ǰ����
		if( m_cbCurSelectIndex != cbCurIndex )
		{
			//���������Ʒǵ���
			m_CurrentCard.bShoot = false;
			for( WORD i = 0; i < m_wCardCount; i++ )
				m_CardItemArray[i].bShoot = false;
			//���õ�ǰѡ��λ��
			m_cbCurSelectIndex = cbCurIndex;
			//���õ�ǰ����
			if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
			{
				for( BYTE i = 0; i < m_SelectCardInfo[cbCurIndex].cbCardCount; i++ )
				{
					WORD j= 0;
					for(j = 0; j < m_wCardCount; j++ )
					{
						if( m_CardItemArray[j].cbCardData == m_SelectCardInfo[cbCurIndex].cbCardData[i] &&
							!m_CardItemArray[j].bShoot)
						{
							m_CardItemArray[j].bShoot = true;
							break;
						}
					}
					if( j == m_wCardCount && 
						m_CurrentCard.cbCardData == m_SelectCardInfo[cbCurIndex].cbCardData[i] )
					{
						m_CurrentCard.bShoot = true;
					}
				}
				//�����ػ�����
				CRect rc = m_rcSelectCard[cbCurIndex];
				rc.OffsetRect( m_ControlPoint.x,m_ControlPoint.y );
				rcRePaint |= rc;
			}
		}

		if( m_cbCurSelectIndex!=CountArray(m_SelectCardInfo) )
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
			return true;
		}
		else return false;
	}

	return false;
}

//�����������Ƿ��ɫ
void CCardControl::SetAllCardGray( bool bGray )
{
	for( WORD i = 0; i < m_wCardCount; i++ )
		m_CardItemArray[i].bGrayed = bGray;
	
	if( m_CurrentCard.cbCardData != 0 )
		m_CurrentCard.bGrayed = bGray;
}

//�����ƻ�ɫ
void CCardControl::SetCardGray( BYTE cbCardData[], BYTE cbCardCount, bool bGray )
{
	ASSERT( cbCardCount <= m_wCardCount+1 );
	if( cbCardCount > m_wCardCount+1 ) return;

	for( WORD i = 0; i < cbCardCount; i++ )
	{
		//������Ӧ��
        WORD j = 0;
		for(j = 0; j < m_wCardCount; j++ )
		{
			if( m_CardItemArray[j].cbCardData == cbCardData[i] )
			{
				m_CardItemArray[j].bGrayed = bGray;
				break;
			}
		}
		if( j == m_wCardCount && m_CurrentCard.cbCardData == cbCardData[i] )
			m_CurrentCard.bGrayed = bGray;
	}
}

//��ȡ��ɫ��
BYTE CCardControl::GetGrayCard( BYTE cbCardData[], BYTE cbMaxCount, bool bGetGray )
{
	//��0
	ZeroMemory( cbCardData,sizeof(BYTE)*cbMaxCount );

	BYTE cbGetCount = 0;
	//���ҷ�����������
	for( WORD i = 0; i < m_wCardCount; i++ )
	{
		if( m_CardItemArray[i].bGrayed == bGetGray )
		{
			cbCardData[cbGetCount++] = m_CardItemArray[i].cbCardData;
			if( cbGetCount == cbMaxCount ) return cbGetCount;
		}
	}
	if( m_CurrentCard.bGrayed == bGetGray )
		cbCardData[cbGetCount++] = m_CurrentCard.cbCardData;

	return cbGetCount;
}

//���ò���ģʽ
void CCardControl::SetSelectMode( bool bSelectMode )
{
	if( m_bSelectMode != bSelectMode )
	{
		m_bSelectMode = bSelectMode;
		if( !m_bSelectMode )
			SetAllCardGray(false);
	}
}

//��ȡ����λ��
CPoint CCardControl::GetDispatchCardPos()
{
	CPoint pt = m_ControlPoint;
	pt.x += CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE;
	return pt;
}

//��갴��
bool CCardControl::OnEventLeftHitCard()
{
	if( !m_bSelectMode )
	{
		BYTE cbHoverCard = GetHoverCard();
		if( cbHoverCard != 0 )
			AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,0);
		return cbHoverCard!=0;
	}
	//ѡ��ģʽ��
	else
	{
		//��ǰѡ��
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
		{
			AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,0,0);
		}
		else return false;
	}
	return false;
}

//��ȡ����
CRect CCardControl::GetCardRect( WORD wItemIndex )
{
	//��������
	CRect rc;
	rc.SetRectEmpty();

	if( wItemIndex == INVALID_ITEM ) return rc;

	//��ǰ��
	if( wItemIndex == CountArray(m_CardItemArray) )
	{
		CPoint ptTopLeft( CountArray(m_CardItemArray)*CARD_WIDTH+POS_SPACE,0 );
		CPoint ptRightBottom( ptTopLeft.x+CARD_WIDTH,ptTopLeft.y+CARD_HEIGHT );
		rc.SetRect( ptTopLeft,ptRightBottom );
	}
	else
	{
		WORD wViewIndex = CountArray(m_CardItemArray)-m_wCardCount+wItemIndex;
		CPoint ptTopLeft( wViewIndex*CARD_WIDTH,0 );
		CPoint ptRightBottom( ptTopLeft.x+CARD_WIDTH,ptTopLeft.y+CARD_HEIGHT );
		rc.SetRect( ptTopLeft,ptRightBottom );
	}

	//��������
	rc.InflateRect( 0,0,0,POS_SHOOT );
	return rc;
}

//��ҳ���
bool CCardControl::OnEventUserAction( const tagSelectCardInfo SelectInfo[MAX_WEAVE], BYTE cbInfoCount )
{
	//��ʼ��
	m_cbSelectInfoCount = cbInfoCount;
	CopyMemory( m_SelectCardInfo,SelectInfo,sizeof(tagSelectCardInfo)*cbInfoCount );

	//���������ƻ�ɫ
	SetAllCardGray( false );

	//���ñ���
	m_bSelectMode = true;
	m_cbCurSelectIndex = CountArray(m_SelectCardInfo);
	
	//���������Ϣ��������������
	for( BYTE i = 0; i < m_cbSelectInfoCount; i++ )
	{
		//��������
		m_rcSelectCard[i].SetRectEmpty();

		for( BYTE j = 0; j < m_SelectCardInfo[i].cbCardCount; j++ )
		{
			WORD k = 0;
			for(k = 0; k < m_wCardCount; k++ )
			{
				if( m_CardItemArray[k].cbCardData == m_SelectCardInfo[i].cbCardData[j] )
				{
					CRect rcCard = GetCardRect(k);

					if( rcCard != (m_rcSelectCard[i]&rcCard) )
					{
						//���ò���
						m_CardItemArray[k].bGrayed = true;

						//��������
						m_rcSelectCard[i] |= rcCard;

						break;
					}
				}
			}
			if( k == m_wCardCount && m_CurrentCard.cbCardData == m_SelectCardInfo[i].cbCardData[j] )
			{
				//���ò���
				m_CurrentCard.bGrayed = true;

				//��������
				m_rcSelectCard[i] |= GetCardRect(CountArray(m_CardItemArray));
			}
		}
	}

	return m_cbSelectInfoCount==1;
}

//��ȡ��Ҳ������
void CCardControl::GetUserSelectResult( tagSelectCardInfo &SelectInfo )
{
	if( m_bSelectMode )
	{
		ASSERT( m_cbSelectInfoCount > 0 );
		if( m_cbSelectInfoCount == 0 ) return;
		if( m_cbCurSelectIndex < CountArray(m_SelectCardInfo) )
			SelectInfo = m_SelectCardInfo[m_cbCurSelectIndex];
		else SelectInfo = m_SelectCardInfo[0];
	}
	else SelectInfo.cbCardCount = 0;
	return;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��������
CCardResource						g_CardResource;						//�˿���Դ

//////////////////////////////////////////////////////////////////////////
