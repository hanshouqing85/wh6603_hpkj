#include "stdafx.h"
#include "VirtualEngine.h"
#include "VirtualWindow.h"

CVirtualEngine::CVirtualEngine()
{
	m_pD3DDevice = NULL;
	m_pWindowLeave = NULL;
	m_VirtualWindowPtrArray.RemoveAll();
}

CVirtualEngine::~CVirtualEngine()
{

}

//绘画窗口
bool CVirtualEngine::DrawWindows( CD3DDevice * pD3DDevice )
{
	for(INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount(); i++)
	{
		CVirtualWindow * pVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);

		if ( pVirtualWindow->GetParentWindow() != NULL ) continue;
		
		pVirtualWindow->InvalidWindow(true);
	}
	return true;
}

//消息处理
bool CVirtualEngine::DefWindowProc( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	BOOL bHandled = false;

	switch ( uMessage )
	{
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			//消息循环：当某个窗口接收到了
			POINT mousePoint = { short( LOWORD( lParam ) ), short( HIWORD( lParam ) ) };

			CVirtualWindow * pVirtualWindow = SwitchToWindow( mousePoint.x,mousePoint.y );
			
			if( pVirtualWindow != NULL && pVirtualWindow->IsWindowEnable() )
			{
				if( pVirtualWindow != m_pWindowLeave )
				{
					if( m_pWindowLeave ) 
					{
						m_pWindowLeave->OnEventMouse( WM_MOUSELEAVE, wParam, mousePoint.x,mousePoint.y );
					}

					m_pWindowLeave = pVirtualWindow;

					if( pVirtualWindow != NULL ) 
					{
						pVirtualWindow->OnEventMouse( uMessage, wParam, mousePoint.x,mousePoint.y );
						//if( bHandled ) return true;
					}
				}
				pVirtualWindow->OnEventMouse( uMessage, wParam, mousePoint.x,mousePoint.y );
				//if( bHandled ) return true;
			}

			switch( uMessage )
			{
			case WM_MOUSEMOVE:
				{
					CVirtualWindow * pVirtualWindow = SwitchToWindow( mousePoint.x,mousePoint.y );

					if( pVirtualWindow == m_pWindowLeave ) return false;

					if( m_pWindowLeave ) 
					{
						m_pWindowLeave->OnEventMouse( WM_MOUSELEAVE, wParam, mousePoint.x,mousePoint.y );
					}
			
					m_pWindowLeave = pVirtualWindow;

					if( pVirtualWindow != NULL )
					{
						m_pWindowLeave->OnEventMouse( WM_MOUSEHOVER, wParam, mousePoint.x,mousePoint.y );
					}

					return false;
				}
			}

			return false;
		}
		//键盘消息
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_CHAR:
		{
			/*if( m_pWindowCapture && m_pWindowCapture->IsWindowEnable() && (m_pWindowCapture->GetVirtualEngine() == this) )
			{
			if( m_pWindowCapture->OnEventKeyboard( uMessage, wParam, lParam ) )
			return true;
			}*/
			break;
		}
	}

	return false;
}

//消息解释
bool CVirtualEngine::PreTranslateMessage( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc(uMessage,wParam,lParam);
}

//更新窗口
VOID CVirtualEngine::InvalidWindow()
{
 	for(INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount(); i++)
 	{
 		m_VirtualWindowPtrArray.GetAt(i)->InvalidWindow(true);
 	}
}

//更新窗口
VOID CVirtualEngine::InvalidWindow( INT nXPos, INT nYPos, INT nWidth, INT nHeight )
{
	//定义变量
	CRect rcRegion(nXPos,nYPos,nXPos+nWidth,nYPos+nHeight);
	CRect rcWindow(0,0,0,0);
	CRect rcDes(0,0,0,0);

 	for(INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount(); i++)
 	{
 		rcWindow.SetRect(0,0,0,0);
 		m_VirtualWindowPtrArray.GetAt(i)->GetWindowRect(rcWindow);
 
 		//规范化客户区
 		rcWindow.NormalizeRect();
 		if( rcDes.IntersectRect(rcWindow,rcRegion) )
 		{
 			m_VirtualWindowPtrArray.GetAt(i)->InvalidWindow(true);
 		}
 	}
}

//获取窗口
CVirtualWindow * CVirtualEngine::SwitchToWindow( INT nXMousePos, INT nYMousePos )
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
			CVirtualWindow *pChildWindow = pVirtualWindow->SwitchToWindow(nXMousePos,nYMousePos);
			if (pChildWindow == NULL)
 				return pVirtualWindow;
			else return pChildWindow;
  		}
 	}

	return NULL;
}

//获取窗口
CVirtualWindow * CVirtualEngine::SwitchToWindow( CVirtualWindow * pVirtualWindow, INT nXMousePos, INT nYMousePos )
{
	if( pVirtualWindow == NULL ) return NULL;

	//定义变量
	CPoint ptMouse(nXMousePos,nYMousePos);
	CRect rcWindow(0,0,0,0);

	for(INT_PTR i=0;i<m_VirtualWindowPtrArray.GetCount(); i++)
	{
		rcWindow.SetRect(0,0,0,0);
		CVirtualWindow *pOwnVirtualWindow = m_VirtualWindowPtrArray.GetAt(i);

		if ( pOwnVirtualWindow->GetParentWindow() == pVirtualWindow )
		{
			pOwnVirtualWindow->GetWindowRect(rcWindow);

			//规范化客户区
			rcWindow.NormalizeRect();

			if ( PtInRect(&rcWindow,ptMouse) && pOwnVirtualWindow->IsWindowEnable() && pOwnVirtualWindow->IsWindowVisible() )
			{ 
				return pOwnVirtualWindow;
			}
		}
	}

	return NULL;
}