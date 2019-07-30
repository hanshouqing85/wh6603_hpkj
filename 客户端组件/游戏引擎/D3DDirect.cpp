#include "stdafx.h"
#include "D3DDirect.h"

CD3DDirect * CD3DDirect::m_pD3DDirect = NULL;

CD3DDirect::CD3DDirect()
{
	m_pIDirect3D = NULL;

	m_pD3DDirect = this;
}

CD3DDirect::~CD3DDirect()
{
	SafeRelease(m_pIDirect3D);	
}

bool CD3DDirect::CreateD3DDirect()
{
	m_pIDirect3D = Direct3DCreate9( D3D_SDK_VERSION );

	if ( m_pIDirect3D == NULL ) 
	{
		ASSERT(FALSE);
		return false;
	}
	return true;
}

bool CD3DDirect::GetDeviceCaps( D3DCAPS9 & D3DCaps )
{	
	ASSERT( m_pIDirect3D!= NULL );
	if( m_pIDirect3D == NULL ) return false;

	HRESULT hResult = m_pIDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps);

	if ( hResult != D3D_OK )return false;

	return true;
}

bool CD3DDirect::GetDisplayMode( D3DDISPLAYMODE & DisplayMode )
{
	ASSERT( m_pIDirect3D!= NULL );
	if( m_pIDirect3D == NULL ) return false;

	HRESULT hResult = m_pIDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &DisplayMode );
	if(FAILED(hResult)) {MessageBox(NULL,TEXT("获得显示适配器模式失败!"),TEXT(""),MB_OK);return FALSE;}

	return true;
}
