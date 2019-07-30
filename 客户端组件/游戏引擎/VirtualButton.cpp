#include "stdafx.h"
#include "VirtualButton.h"

enum CONTROL_STATE
{
	VIRTUAL_STATE_NORMAL = 0,
	VIRTUAL_STATE_MOUSEOVER,
	VIRTUAL_STATE_PRESSED,
	VIRTUAL_STATE_DISABLED,
	VIRTUAL_STATE_HIDDEN,
	VIRTUAL_STATE_FOCUS
};

//常量定义
#define MIN_TRANSITION_INDEX		0									//过渡索引
#define MAX_TRANSITION_INDEX		8									//过渡索引

#define FRAME_COUNT					4

CVirtualButton::CVirtualButton()
{
	m_bMouseDown = false;;				
	m_bMouseMove = false;;					
	m_rcButtonRect.SetRect(0,0,0,0);				
	m_D3DTextureButton.Destory();

	m_wImageIndex = 0;				
}

CVirtualButton::~CVirtualButton()
{
	m_D3DTextureButton.Destory();
	DeleteWindow();
}

VOID CVirtualButton::OnWindowMovie()
{
	if ( (m_ImageLapseCount.GetLapseCount(30) > 0) && ((m_wImageIndex!=MIN_TRANSITION_INDEX)&&(m_wImageIndex!=MAX_TRANSITION_INDEX)) )
	{
		//盘旋过渡
		if ((m_bMouseMove==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX))
		{
			m_wImageIndex++;
		}

		//常态过渡
		if ((m_bMouseMove==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX))
		{
			m_wImageIndex--;
		}
	}
}

VOID CVirtualButton::OnWindowCreate( CD3DDevice * pD3DDevice )
{
	m_ImageLapseCount.Initialization();
}

VOID CVirtualButton::OnWindowDestory( CD3DDevice * pD3DDevice )
{
	if(!m_D3DTextureButton.IsNull())m_D3DTextureButton.Destory();
}

VOID CVirtualButton::OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos)
{
	if( !m_bEnable || !m_bVisible ) return;

	switch( uMessage )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			CRect rcControl;
			GetWindowRect(rcControl);
			if ( rcControl.PtInRect(CPoint(nXMousePos,nYMousePos)) )
			{
				m_bMouseDown = true;
			}
			break;
		}
	case WM_LBUTTONUP:
		{
			if( m_bMouseDown )
			{
				m_wImageIndex = 0;
				m_bMouseDown = false;
				if (GetParentWindow()!=NULL)
				{
					GetParentWindow()->OnEventButton(GetWindowID(),uMessage,nXMousePos,nYMousePos);
				}else
				{
					HWND hWnd=m_pVirtualEngine->GetD3DDevice()->GetWndDevice();
					if ( hWnd != NULL )	::PostMessage(hWnd,WM_COMMAND,GetWindowID(),0);
				}
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			if( !m_bMouseMove )
			{
				m_bMouseMove = true;
				m_wImageIndex = 1;
			}
			break;
		}
	case WM_MOUSELEAVE:
		{
			m_bMouseMove = false;
			m_bMouseDown = false;
			m_wImageIndex = 0;
			break;
		}
	case WM_MOUSEHOVER:
		{
			m_bMouseMove = false;
			m_bMouseDown = false;
			m_wImageIndex = 0;
			break;
		}
	}

	return ;
}

VOID CVirtualButton::OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos)
{
}

VOID CVirtualButton::OnEventDrawWindow( CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos )
{
	//绘画界面
	if (!m_D3DTextureButton.IsNull() )
	{
		//变量定义
		INT nImagePos=0;
		INT nPartWidth=m_D3DTextureButton.GetWidth()/FRAME_COUNT;

		//按钮状态
		bool bDisable=!m_bEnable;
		bool bButtonDown=m_bMouseDown;

		//计算位置
		if (bDisable==true) nImagePos=nPartWidth*3;
		if ((bDisable==false)&&(bButtonDown==true)) nImagePos=nPartWidth*2;
		if ((bDisable==false)&&(bButtonDown==false)&&(m_bMouseMove==true)) nImagePos=nPartWidth;

		//状态判断
		bool bTransitionDraw=false;
		if ((m_bMouseMove==true)&&(m_wImageIndex<MAX_TRANSITION_INDEX)) bTransitionDraw=true;
		if ((m_bMouseMove==false)&&(m_wImageIndex>MIN_TRANSITION_INDEX)) bTransitionDraw=true;

		//绘画背景
		if ((bDisable==false)&&(bButtonDown==false)&&(bTransitionDraw==true))
		{
			//绘画底图
			if (m_wImageIndex<MAX_TRANSITION_INDEX)
			{
				if ((m_rcButtonRect.Width()==nPartWidth)&&(m_rcButtonRect.Height()==m_D3DTextureButton.GetHeight()))
				{
					m_D3DTextureButton.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos,m_rcButtonRect.Width(),m_rcButtonRect.Height(),0,0);
				}
				else
				{
					m_D3DTextureButton.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos,m_rcButtonRect.Width(),m_rcButtonRect.Height(),0,0,nPartWidth,m_D3DTextureButton.GetHeight());
				}
			}

			//绘画过渡
			if (m_wImageIndex>MIN_TRANSITION_INDEX)
			{
				m_D3DTextureButton.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos,m_rcButtonRect.Width(),m_rcButtonRect.Height(),nPartWidth,0,nPartWidth,m_D3DTextureButton.GetHeight(),(BYTE)(m_wImageIndex*255/MAX_TRANSITION_INDEX));
			}

			return;
		}
		else
		{
			//普通模式
			if ((m_rcButtonRect.Width()==nPartWidth)&&(m_rcButtonRect.Height()==m_D3DTextureButton.GetHeight()))
			{
				m_D3DTextureButton.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos,m_rcButtonRect.Width(),m_rcButtonRect.Height(),nImagePos,0);
			}
			else
			{
				m_D3DTextureButton.DrawImage(pD3DDevice,nXOriginPos,nYOriginPos,m_rcButtonRect.Width(),m_rcButtonRect.Height(),nImagePos,0,nPartWidth,m_D3DTextureButton.GetHeight());
			}
		}
	}
	else
	{
		pD3DDevice->DrawRect(CRect(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_WindowSize.cx,m_BenchmarkPos.y+m_WindowSize.cy),D3DCOLOR_XRGB(255,255,255));
	}
}

VOID CVirtualButton::SetButtonRect( CRect rcButtonRect )
{
	m_rcButtonRect.CopyRect(&rcButtonRect);
}

VOID CVirtualButton::SetButtonImage(CD3DDevice * pD3DDevice, LPCTSTR pszResource, LPCTSTR pszTypeName, HINSTANCE hResInstance)
{
	if( m_D3DTextureButton.IsNull() == false ) return;

	m_D3DTextureButton.LoadImage(pD3DDevice,hResInstance,pszResource,pszTypeName);

	m_rcButtonRect.SetRect(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_D3DTextureButton.GetWidth()/FRAME_COUNT,m_BenchmarkPos.y+m_D3DTextureButton.GetHeight());

	SetWindowPos(m_rcButtonRect.left,m_rcButtonRect.top,m_rcButtonRect.Width(),m_rcButtonRect.Height(),SWP_DRAWFRAME);
}

VOID CVirtualButton::SetButtonImage(CD3DDevice * pD3DDevice, LPCTSTR pszModule,LPCTSTR pszResource)
{
	if( m_D3DTextureButton.IsNull() == false ) return;

	CString strPath;
	strPath.Format(TEXT("%s\\%s"),pszModule,pszResource);
	m_D3DTextureButton.LoadImage(pD3DDevice,strPath);
	m_rcButtonRect.SetRect(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_D3DTextureButton.GetWidth()/FRAME_COUNT,m_BenchmarkPos.y+m_D3DTextureButton.GetHeight());

	SetWindowPos(m_rcButtonRect.left,m_rcButtonRect.top,m_rcButtonRect.Width(),m_rcButtonRect.Height(),SWP_DRAWFRAME);
}

VOID CVirtualButton::RectifyControl( CD3DDevice * pD3DDevice )
{

}