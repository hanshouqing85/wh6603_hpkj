#include "StdAfx.h"
#include "GameClient.h"
#include "NoteCard.h"

//////////////////////////////////////////////////////////////////////////////////

//控件变量
#define IDC_BT_CLOSE				100									//进贡扑克

//动画数目
#define SCORE_CARTOON_COUNT			4									//动画数目

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CNoteCard::CNoteCard()
{
	//设置变量
	m_cbAlphaIndex=0;

	//设置变量
	SetCardInfo();

	return;
}

//析构函数
CNoteCard::~CNoteCard()
{
}

//动画消息
VOID CNoteCard::OnWindowMovie()
{
	//灰度动画
	if ((IsWindowVisible()==true)&&((m_cbAlphaIndex<110L)&&(m_AlphaLapseCount.GetLapseCount(20)>0L)))
	{
		m_cbAlphaIndex+=5L;
	}

	return;
}

//创建消息
VOID CNoteCard::OnWindowCreate(CD3DDevice * pD3DDevice)
{
	//创建字体
	//m_FontScore.CreateFont(100,TEXT("宋体"),0L);

	//关闭按钮
	CRect rcCreate(0,0,0,0);
	//加载资源
	m_TextureNoteCard.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("NOTE_CARD"),TEXT("PNG"));
	m_TextureMiniCard.LoadImage(pD3DDevice,AfxGetInstanceHandle(),TEXT("MINI_CARD"),TEXT("PNG"));

	//获取大小
	m_SizeMiniCard.SetSize(m_TextureMiniCard.GetWidth() / 13,m_TextureMiniCard.GetHeight() / 5);

	CSize SizeNoteCard;
	SizeNoteCard.SetSize(m_TextureNoteCard.GetWidth(),m_TextureNoteCard.GetHeight());

	//移动窗口
	SetWindowPos(0,0,SizeNoteCard.cx,SizeNoteCard.cy,0);

	return;
}

//销毁消息
VOID CNoteCard::OnWindowDestory(CD3DDevice * pD3DDevice)
{
	//删除字体
	//m_FontScore.DeleteFont();

	//销户资源
	if (m_TextureNoteCard.IsNull()==false) m_TextureNoteCard.Destory();
	if (m_TextureMiniCard.IsNull()==false) m_TextureMiniCard.Destory();

	return;
}

//鼠标事件
VOID CNoteCard::OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)
{
	return;
}

//按钮事件
VOID CNoteCard::OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)
{
	//关闭窗口
	if (uButtonID==IDC_BT_CLOSE)
	{
		CloseControl();
		return;
	}

	return;
}

//绘画窗口
VOID CNoteCard::OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)
{
	//灰化背景
	//CSize SizeWindow=pD3DDevice->GetSizeWindow();
	//pD3DDevice->FillRect(0,0,SizeWindow.cx,SizeWindow.cy,D3DCOLOR_ARGB(m_cbAlphaIndex,0,0,0));

	//绘画背景
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

//隐藏窗口
VOID CNoteCard::CloseControl()
{
	//关闭窗口
	if (IsWindowActive()==true)
	{
		//关闭窗口
		DeleteWindow();

		//设置数据

	}

	return;
}
//设置扑克
VOID CNoteCard::SetCardInfo()
{
	for(int i = 0; i < 54; i++)
	{
		m_cbCardData[i] = 1;
	}
}

//设置扑克
VOID CNoteCard::SetCardInfo(BYTE *cbCardData, BYTE cbCardCount)
{
	ZeroMemory(m_cbCardData, sizeof(m_cbCardData));

	int nIndex = -1;
	//设置变量
	for(int i = 0; i < cbCardCount; i++)
	{
		nIndex = SwitchToIndex(cbCardData[i]);
		if(nIndex < 0 || nIndex >= 54)
		{
			continue;
		}
		m_cbCardData[nIndex] = 1;
	}

	//灰化动画
	m_cbAlphaIndex=0;
	m_AlphaLapseCount.Initialization();

	return;
}

//删除扑克
VOID CNoteCard::RemoveCard(BYTE cbCardData[], BYTE cbCardCount)
{
	int nIndex = -1;
	//设置变量
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

//转换索引
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
// 绘画数字
void CNoteCard::DrawNumber( CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat /*= DT_LEFT*/ )
{
	TCHAR szOutNum[128] = {0};
	_sntprintf(szOutNum,CountArray(szOutNum),SCORE_STRING,lOutNum);

	// 加载资源
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
