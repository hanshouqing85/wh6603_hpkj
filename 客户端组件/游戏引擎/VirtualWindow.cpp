#include "stdafx.h"
#include "VirtualWindow.h"


CVirtualWindow::CVirtualWindow()
{
	//窗口属性
	m_bActive = false;							
	m_bEnable = true;							
	m_bVisible = false;							

	//属性变量
	m_uWindowID = 0;	

	//位置变量
	m_WindowSize.SetSize(0,0);						
	m_BenchmarkPos.SetPoint(0,0);						

	//内核变量
	m_pParentWindow = NULL;					
	m_pVirtualEngine=NULL;	
	m_VirtualWindowPtrArray.RemoveAll();	
}

CVirtualWindow::~CVirtualWindow()
{
}

//删除窗口
VOID CVirtualWindow::DeleteWindow()
{
	if ( m_pVirtualEngine == NULL ) return;
	//销毁所有子窗口
	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		CVirtualWindow *pVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);
		pVirtualWindow->OnWindowDestory(m_pVirtualEngine->GetD3DDevice());
	}
	m_VirtualWindowPtrArray.RemoveAll();
	//销毁自己
	OnWindowDestory(m_pVirtualEngine->GetD3DDevice());

	//主窗口移除自己
	for (INT_PTR i=0;i<m_pVirtualEngine->m_VirtualWindowPtrArray.GetCount();i++)
	{
		CVirtualWindow *pVirtualWindow = m_pVirtualEngine->m_VirtualWindowPtrArray.GetAt(i);
		if (pVirtualWindow == this)
		{
			m_pVirtualEngine->m_VirtualWindowPtrArray.RemoveAt(i);
			break;
		}
	}
}

//激活窗口
bool CVirtualWindow::ActiveWindow(CRect & rcWindow, DWORD dwStyle, UINT uWindowID, CVirtualEngine * pVirtualEngine, CVirtualWindow * pParentWindow)
{
	m_BenchmarkPos.SetPoint(rcWindow.left,rcWindow.top);
	m_WindowSize.SetSize(rcWindow.Width(),rcWindow.Height());

	m_uWindowID = uWindowID;

	m_pVirtualEngine = pVirtualEngine;
	m_pParentWindow = pParentWindow;
	m_bActive = true;

	if ( dwStyle & WS_VISIBLE ) m_bVisible = true;
	if ( dwStyle & WS_DISABLED ) m_bEnable = false;
	
	//创建控件
	OnWindowCreate(m_pVirtualEngine->GetD3DDevice());

	if ( pParentWindow != NULL )
	{
		pParentWindow->m_VirtualWindowPtrArray.Add(this);
	}
	else 
		m_pVirtualEngine->m_VirtualWindowPtrArray.Add(this);
	return true;
}

//显示窗口
VOID CVirtualWindow::ShowWindow( bool bVisible )
{
	m_bVisible = bVisible;

	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		m_VirtualWindowPtrArray.GetAt(i)->m_bVisible = bVisible;
	}
}

//禁止窗口
VOID CVirtualWindow::EnableWindow( bool bEnable )
{
	m_bEnable = bEnable;

	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		m_VirtualWindowPtrArray.GetAt(i)->m_bEnable = bEnable;
	}
}

//更新窗口
VOID CVirtualWindow::InvalidWindow( bool bCoerce )
{
	if ( !m_bVisible ) return;

	OnEventDrawWindow(m_pVirtualEngine->GetD3DDevice(),m_BenchmarkPos.x,m_BenchmarkPos.y);

	OnWindowMovie();

	if( bCoerce )
	{
		OnEventDrawChildWindow(m_pVirtualEngine->GetD3DDevice(),0,0);
	}
}

//窗口大小
VOID CVirtualWindow::GetClientRect( CRect & rcClient )
{
	rcClient.SetRect(0,0,m_WindowSize.cx,m_WindowSize.cy);
}

//窗口大小
VOID CVirtualWindow::GetWindowRect( CRect & rcWindow )
{
	rcWindow.SetRect(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_WindowSize.cx,m_BenchmarkPos.y+m_WindowSize.cy);
}

//设置位置
VOID CVirtualWindow::SetWindowPos( INT nXPos, INT nYPos, INT nWidth, INT nHeight, UINT uFlags )
{
	int xOld = m_BenchmarkPos.x;
	int yOld = m_BenchmarkPos.y;

	if ( uFlags & SWP_HIDEWINDOW ) ShowWindow(SW_HIDE);
	if ( uFlags & SWP_NOACTIVATE ) m_bActive = false;

	if ((uFlags & (SWP_NOMOVE|SWP_NOSIZE)) != (SWP_NOMOVE|SWP_NOSIZE))
	{
		int xNew;
		int yNew;

		if (uFlags & SWP_NOMOVE) { xNew = m_BenchmarkPos.x; yNew = m_BenchmarkPos.y; }
		else { xNew = nXPos; yNew = nYPos; }

		int cxNew;
		int cyNew;

		if (uFlags & SWP_NOSIZE) { cxNew = m_WindowSize.cx; cyNew = m_WindowSize.cy; }
		else { cxNew = nWidth; cyNew = nHeight; }

		CRect rectOld(m_BenchmarkPos.x,m_BenchmarkPos.y,m_BenchmarkPos.x+m_WindowSize.cx,m_BenchmarkPos.y+m_WindowSize.cy);

		m_BenchmarkPos.SetPoint(xNew,yNew);
		m_WindowSize.SetSize(cxNew,cyNew);
	}

	if (uFlags & SWP_SHOWWINDOW) ShowWindow(true);

// 	uFlags &= ~(SWP_HIDEWINDOW|SWP_SHOWWINDOW);
// 	uFlags |= (SWP_NOMOVE|SWP_NOSIZE);

	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		CVirtualWindow *pVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);

		CPoint pt(pVirtualWindow->m_BenchmarkPos.x+nXPos-xOld,pVirtualWindow->m_BenchmarkPos.y+nYPos-yOld);

		pVirtualWindow->SetWindowPos(pt.x,pt.y,0,0,SWP_NOSIZE);
	}

	OnWindowPosition(m_pVirtualEngine->GetD3DDevice());
	if (uFlags == SWP_DRAWFRAME) return ;
	//InvalidWindow(true);
}
//获取窗口
CVirtualWindow * CVirtualWindow::SwitchToWindow(INT nXMousePos, INT nYMousePos)
{
	//定义变量
	CPoint ptMouse(nXMousePos,nYMousePos);
	CRect rcWindow(0,0,0,0);

	for(INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount(); i++)
	{
		CVirtualWindow *pVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);
		rcWindow.SetRect(0,0,0,0);

		pVirtualWindow->GetWindowRect(rcWindow);

		//规范化客户区
		rcWindow.NormalizeRect();

		if ( PtInRect(&rcWindow,ptMouse) && pVirtualWindow->IsWindowEnable() && pVirtualWindow->IsWindowVisible() )
		{ 
			return pVirtualWindow;
		}
	}

	return NULL;
};
//下属窗口
bool CVirtualWindow::IsChildWindow( CVirtualWindow * pVirtualWindow )
{
	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		if ( m_VirtualWindowPtrArray.GetAt(i)->GetParentWindow() == pVirtualWindow )
		{
			return true;
		}
	}
	return false;
}

//重置窗口
VOID CVirtualWindow::ResetWindow()
{
	//窗口属性
	m_bActive = false;							
	m_bEnable = true;							
	m_bVisible = false;							

	//属性变量
	m_uWindowID = 0;						

	//位置变量
	m_WindowSize.SetSize(0,0);						
	m_BenchmarkPos.SetPoint(0,0);						

	//内核变量
	m_pParentWindow = NULL;					
	m_pVirtualEngine=NULL;					
	m_VirtualWindowPtrArray.RemoveAll();	
}

//绘画窗口
VOID CVirtualWindow::OnEventDrawChildWindow( CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos)
{
	CVirtualWindow *pVirtualWindow = NULL;

	for (INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount();i++)
	{
		pVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);
		pVirtualWindow->OnEventDrawWindow(pD3DDevice,pVirtualWindow->m_BenchmarkPos.x,pVirtualWindow->m_BenchmarkPos.y);
		pVirtualWindow->OnWindowMovie();
	}
}

/*
//拖动窗口
BOOL CVirtualWindow::DragWindows( UINT uMessage, INT nXMousePos, INT nYMousePos, UINT nFlags )
{
	if( m_bDrag )
	{
		static INT nOldXMousePos = 0;
		static INT nOldYMousePos = 0;

		if ( uMessage == WM_LBUTTONDOWN )
		{
			nOldXMousePos = nXMousePos-m_BenchmarkPos.x;
			nOldYMousePos = nYMousePos-m_BenchmarkPos.y;

			//SetFocus();
		}

		if ( uMessage == WM_MOUSEMOVE && nFlags == MK_LBUTTON )
		{
			//基准位置
			INT nXPos=nXMousePos-nOldXMousePos;
			INT nYPos=nYMousePos-nOldYMousePos;

			SetWindowPos(nXPos,nYPos,0,0,SWP_NOSIZE);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CVirtualWindow::OnWindowMouse( UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos )
{
	if( m_bCaption )
	{
		if( uMessage == WM_LBUTTONDOWN || uMessage == WM_LBUTTONDBLCLK )
		{
			if( nXMousePos >= m_BenchmarkPos.x && nXMousePos < m_BenchmarkPos.x + m_WindowSize.cx &&
				nYMousePos >= m_BenchmarkPos.y && nYMousePos < m_BenchmarkPos.y + m_WindowSize.cy )
			{
				SetFocus();
				m_bDrag = true;
			}
		}
		else if( uMessage == WM_LBUTTONUP && m_bDrag )
		{
			if( nXMousePos >= m_BenchmarkPos.x && nXMousePos < m_BenchmarkPos.x + m_WindowSize.cx &&
				nYMousePos >= m_BenchmarkPos.y && nYMousePos < m_BenchmarkPos.y + m_WindowSize.cy )
			{
				m_bDrag = false;
			}
		}
	}

	return OnEventMouse(uMessage, nFlags, nXMousePos, nYMousePos);
}

BOOL CVirtualWindow::ContainsPoint( POINT pt )
{
	CRect rcControl(0,0,0,0);
	GetWindowRect(rcControl);

	return PtInRect( &rcControl, pt );
}
*/