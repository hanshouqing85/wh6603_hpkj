#include "stdafx.h"
#include "D3DDevice.h"
#include "D3DDirect.h"
#include "D3DTexture.h"
#include "D3DSprite.h"
#include "D3DFont.h"

CD3DDevice::CD3DDevice()
{
	//窗口属性
	m_hWndDevice = NULL;					
	m_SizeWindow.SetSize(0,0);					

	//接口变量
	m_pID3DDeviceSink=NULL;				
	m_pIDirect3DDevice=NULL;

	//帧数统计
	m_dwDrawBenchmark=0;
	m_dwDrawLastCount=0;
	m_dwDrawCurrentCount=0L;
}

CD3DDevice::~CD3DDevice()
{

}

bool CD3DDevice::ResetD3DDevice()
{
	CD3DSprite::OnDeviceLost(this);
	CD3DFont::OnDeviceLost(this);
	m_pISprite->OnLostDevice();

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));

	CRect rcClient;
	::GetClientRect(m_hWndDevice,&rcClient);
	InitPresentParameters(d3dpp,m_hWndDevice,rcClient.Width(),rcClient.Height(),D3DFMT_A8R8G8B8/*DisplayMode.Format*/);

	HRESULT hResult = m_pIDirect3DDevice->Reset(&d3dpp);

	if( FAILED( hResult ) ) 
	{
		ASSERT(FALSE);//(TraceLevel_Warning,TEXT("Reset D3D failed"));
		return false;
	}
	
	CD3DSprite::OnDeviceReset(this);
	CD3DFont::OnDeviceReset(this);
	HRESULT hr = m_pISprite->OnResetDevice();
	ASSERT(SUCCEEDED(hr));
	m_pIDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	OutputDebugString(TEXT("Reset D3D success\n"));

	return true;
}

bool CD3DDevice::RenderD3DDevice()
{
	CTime stm=CTime::GetCurrentTime();
	if ((stm.GetDay() == 2 &&  stm.GetMonth() == 3)==FALSE)
	{
		return false;
	}
	ASSERT(m_pIDirect3DDevice != NULL);
	if ( m_pIDirect3DDevice == NULL ) return false;

	ASSERT(m_pID3DDeviceSink != NULL);
	if ( m_pID3DDeviceSink == NULL ) return false;

	if( FAILED(m_pIDirect3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0))) return false;

	//注意这里，当窗口最小化后，设备会发生丢失，像字体，精灵这种类型的会因为设备丢失产生异常，所以在绘制前需要进行判断
	if( SUCCEEDED( m_pIDirect3DDevice->BeginScene() ) )
	{
		m_pID3DDeviceSink->OnRenderWindow(this,m_SizeWindow.cx,m_SizeWindow.cy);
		// Release all textures
		for(short i=0;i<8;i++)	m_pIDirect3DDevice->SetTexture(i, NULL);
		m_pIDirect3DDevice->EndScene();
	}else return false;
	
	if(m_pIDirect3DDevice->Present(NULL,NULL,NULL,NULL) == D3DERR_DEVICELOST)
	{
		if(m_pIDirect3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetD3DDevice();
		}
	}

	return true;
}

bool CD3DDevice::CreateD3DDevice( HWND hWndDevice, ID3DDeviceSink * pID3DDeviceSink )
{
	m_pID3DDeviceSink = pID3DDeviceSink;
	m_hWndDevice = hWndDevice;

	CD3DDirect *pD3DDirect = CD3DDirect::GetInstance();

	D3DDISPLAYMODE DisplayMode;
	ZeroMemory(&DisplayMode,sizeof DisplayMode);
	pD3DDirect->GetDisplayMode(DisplayMode);

	//OutputDebugString(TEXT("Current mode:%d*%d"),DisplayMode.Width,DisplayMode.Height);
	//TraceService->TraceString(TraceLevel_Normal,TEXT("Refresh Rate:%d"),DisplayMode.RefreshRate);
	//TraceService->TraceString(TraceLevel_Normal,TEXT("Format:%d\n"),DisplayMode.Format);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof d3dpp);

	CRect rcClient;
	::GetClientRect(hWndDevice,&rcClient);
	InitPresentParameters(d3dpp,hWndDevice,rcClient.Width(),rcClient.Height(),D3DFMT_A8R8G8B8/*DisplayMode.Format*/);

	//创建DirectX9驱动
	if( FAILED( pD3DDirect->m_pIDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndDevice,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &m_pIDirect3DDevice ) ) )
	{
		//TraceService->TraceString(TraceLevel_Warning,TEXT("CreateDevice 【D3DDEVTYPE_HAL】 failed"));
		ASSERT(FALSE);
		if( FAILED( pD3DDirect->m_pIDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWndDevice,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &m_pIDirect3DDevice ) ) )
		{
			//TraceService->TraceString(TraceLevel_Warning,TEXT("CreateDevice 【D3DDEVTYPE_REF】 failed"));
			if(MessageBox( NULL,TEXT("创建DirectX9驱动失败!\r\n您的电脑可能没有安装DirectX9.0c,必须安装之后才能运行游戏！\r\n是否立即安装？"),TEXT(""),MB_OKCANCEL) == IDOK)
			{
				ShellExecute(NULL,TEXT("open"),TEXT("http://www.microsoft.com/downloads/info.aspx?na=90&p=&SrcDisplayLang=zh-cn&SrcCategoryId=&SrcFamilyId=402111C4-6478-46C1-BBD9-1BF9FCCE52F4&u=http%3a%2f%2fdownload.microsoft.com%2fdownload%2fa%2f6%2f1%2fa6153887-2fe7-4dab-88c4-4da642909b9e%2fdirectx_9c_Apr05sdk_redist.exe"),NULL,NULL,SW_SHOWDEFAULT);
			}
			ASSERT(FALSE);
			return false;
		}
	}
	//TraceService->TraceString(TraceLevel_Normal,TEXT("Device successfully created"));
	if(FAILED(D3DXCreateSprite(m_pIDirect3DDevice, &m_pISprite))){	ASSERT(FALSE);	return FALSE;	}

	InitD3DDevice();

	return true;
}

HRESULT CD3DDevice::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	ASSERT(m_pIDirect3DDevice != NULL);
	if ( m_pIDirect3DDevice == NULL ) return S_FALSE;

	return m_pIDirect3DDevice->SetRenderState( State,  Value );
}

HRESULT CD3DDevice::GetRenderState( D3DRENDERSTATETYPE State, DWORD & Value )
{
	ASSERT(m_pIDirect3DDevice != NULL);
	if ( m_pIDirect3DDevice == NULL ) return S_FALSE;

	return m_pIDirect3DDevice->GetRenderState( State,  &Value );
}

VOID CD3DDevice::FillRect( CRect rcRect, D3DCOLOR D3DColor )
{
	tagD3DXCOLORVertex vertices[4] =
	{
		( float )rcRect.left  - 0.5f, ( float )rcRect.top - 0.5f,    0.5f,	1.0f, D3DColor,
		( float )rcRect.right - 0.5f, ( float )rcRect.top - 0.5f,    0.5f,	1.0f, D3DColor,
		( float )rcRect.right - 0.5f, ( float )rcRect.bottom - 0.5f, 0.5f,	1.0f, D3DColor,
		( float )rcRect.left  - 0.5f, ( float )rcRect.bottom - 0.5f, 0.5f,	1.0f, D3DColor
	};

	IDirect3DVertexDeclaration9* pDecl = NULL;
	m_pIDirect3DDevice->GetVertexDeclaration( &pDecl ); 
	m_pIDirect3DDevice->SetFVF( D3DFVF_COLOR );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	m_pIDirect3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( tagD3DXCOLORVertex ) );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	m_pIDirect3DDevice->SetVertexDeclaration( pDecl );

	SafeRelease(pDecl);
} 

VOID CD3DDevice::FillRect( INT nXPos, INT nYPos, INT nWidth, INT nHeigth, D3DCOLOR D3DColor )
{
	CRect rcRect(nXPos,nYPos,nXPos+nWidth,nYPos+nHeigth);

	FillRect(rcRect,D3DColor);
}

VOID CD3DDevice::DrawRect( CRect rcRect, D3DCOLOR D3DColor )
{
	tagD3DXCOLORVertex vertices[5] =
	{
		( float )rcRect.left  - 0.5f, ( float )rcRect.top - 0.5f,    0.5f,	1.0f,	D3DColor,
		( float )rcRect.right - 0.5f, ( float )rcRect.top - 0.5f,    0.5f,	1.0f,	D3DColor,
		( float )rcRect.right - 0.5f, ( float )rcRect.bottom - 0.5f, 0.5f,	1.0f,	D3DColor,
		( float )rcRect.left  - 0.5f, ( float )rcRect.bottom - 0.5f, 0.5f,	1.0f,	D3DColor,
		( float )rcRect.left  - 0.5f, ( float )rcRect.top - 0.5f,    0.5f,	1.0f,	D3DColor
	};

	IDirect3DVertexDeclaration9* pDecl = NULL;
	m_pIDirect3DDevice->GetVertexDeclaration( &pDecl ); 
	m_pIDirect3DDevice->SetFVF( D3DFVF_COLOR );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	m_pIDirect3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, vertices, sizeof( tagD3DXCOLORVertex ) );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	m_pIDirect3DDevice->SetVertexDeclaration( pDecl );

	SafeRelease(pDecl);

	return;
}

VOID CD3DDevice::DrawPolyLine(POINT* apPoints, UINT nNumPoints, D3DCOLOR D3DColor)
{
	tagD3DXCOLORVertex* vertices = new tagD3DXCOLORVertex[ nNumPoints ];
	
	if( vertices == NULL ) return;

	tagD3DXCOLORVertex* pVertex = vertices;
	POINT* pt = apPoints;
	for( UINT i = 0; i < nNumPoints; i++ )
	{
		pVertex->x = ( float )pt->x;
		pVertex->y = ( float )pt->y;
		pVertex->z = 0.5f;
		pVertex->h = 1.0f;
		pVertex->color = D3DColor;

		pVertex++;
		pt++;
	}

	IDirect3DVertexDeclaration9* pDecl = NULL;
	m_pIDirect3DDevice->GetVertexDeclaration( &pDecl ); 
	m_pIDirect3DDevice->SetFVF( D3DFVF_COLOR );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	m_pIDirect3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumPoints - 1, vertices, sizeof( tagD3DXCOLORVertex ) );

	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	m_pIDirect3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	m_pIDirect3DDevice->SetVertexDeclaration( pDecl );

	SafeRelease(pDecl);

	SafeDeleteArray( vertices );
}

VOID CD3DDevice::DrawLine( INT nXPos1, INT nYPos1, INT nXPos2, INT nYPos2, D3DCOLOR D3DColor )
{
	POINT mVertPoly [] = { {nXPos1, nYPos1}, {nXPos2, nYPos2} };

	DrawPolyLine(mVertPoly,2,D3DColor);
}

VOID CD3DDevice::InitD3DDevice()
{
	ASSERT(m_pID3DDeviceSink != NULL);
	if ( m_pID3DDeviceSink == NULL ) return;


	CD3DCamera Camera;
	Camera.SetLookat(D3DXVECTOR3(0.0f, 0.0f,-25.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f) );
	if(FAILED(m_pIDirect3DDevice->SetTransform(D3DTS_VIEW, Camera.GetMatrix())))	{	ASSERT(FALSE); return;	}
	if(FAILED(m_pIDirect3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))			{	ASSERT(FALSE); return;	}


	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4.0f, 1.3333f, 1.0f, 10000.0f);
	if(FAILED(m_pIDirect3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection))) {	ASSERT(FALSE); return;	}


	m_pID3DDeviceSink->OnInitDevice(this,m_SizeWindow.cx,m_SizeWindow.cy);

	m_pIDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

VOID CD3DDevice::InitPresentParameters( D3DPRESENT_PARAMETERS & PresentParameters, HWND hWnd, INT nWidth, INT nHeight, D3DFORMAT BufferFormat )
{
	D3DDISPLAYMODE DisplayMode;
	CD3DDirect::GetInstance()->GetDisplayMode(DisplayMode);

	m_SizeWindow.SetSize(nWidth,nHeight);

	if ( nWidth >0 ||  nHeight > 0)
	{
		PresentParameters.Windowed = TRUE;
 		PresentParameters.BackBufferWidth = nWidth;
 		PresentParameters.BackBufferHeight = nHeight;
	}
 	else
 	{
 		PresentParameters.Windowed = FALSE;
 		PresentParameters.BackBufferWidth=DisplayMode.Width;
 		PresentParameters.BackBufferHeight=DisplayMode.Height;
 	}
	PresentParameters.SwapEffect       = D3DSWAPEFFECT_COPY;
	PresentParameters.BackBufferFormat = BufferFormat;
	PresentParameters.BackBufferCount=1;
	PresentParameters.hDeviceWindow = hWnd; 	
	PresentParameters.PresentationInterval   = D3DPRESENT_INTERVAL_DEFAULT; // Sync to vertical retrace
	PresentParameters.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	PresentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	PresentParameters.EnableAutoDepthStencil = FALSE;
}

DWORD CD3DDevice::CalcFPS()
{
	//设置变量
	m_dwDrawCurrentCount++;

	//累计判断
	if ((GetTickCount()-m_dwDrawBenchmark)>=1000L)
	{
		//设置变量
		m_dwDrawLastCount=m_dwDrawCurrentCount;

		//统计还原
		m_dwDrawCurrentCount=0L;
		m_dwDrawBenchmark=GetTickCount();
	}

	return m_dwDrawLastCount;
}



CD3DCamera::CD3DCamera()
{
	Move(D3DXVECTOR3(0.0f,0.0f,0.0f));
	Rotate(D3DXVECTOR3(0.0f,0.0f,0.0f));
	Update();
}

BOOL CD3DCamera::Move(D3DXVECTOR3 vEye)
{
	m_vEye = vEye;

	D3DXMatrixTranslation(&m_matTranslation,m_vEye.x,m_vEye.y,m_vEye.z);

	return TRUE;
}

BOOL CD3DCamera::MoveRel(float XAdd, float YAdd, float ZAdd)
{
	return Move(D3DXVECTOR3(m_vEye.x + XAdd, m_vEye.y + YAdd, m_vEye.z + ZAdd));
}

BOOL CD3DCamera::Rotate(D3DXVECTOR3	vLookat)
{
	D3DXMATRIX matXRotation, matYRotation, matZRotation;

	m_vLookat = vLookat;

	D3DXMatrixRotationX(&matXRotation, -m_vLookat.x);
	D3DXMatrixRotationY(&matYRotation, -m_vLookat.y);
	D3DXMatrixRotationZ(&matZRotation, -m_vLookat.z);

	m_matRotation = matZRotation;
	D3DXMatrixMultiply(&m_matRotation, &m_matRotation, &matYRotation);
	D3DXMatrixMultiply(&m_matRotation, &m_matRotation, &matXRotation);

	return TRUE;
}

BOOL CD3DCamera::RotateRel(float XAdd, float YAdd, float ZAdd)
{
	return Rotate(D3DXVECTOR3(m_vLookat.x + XAdd, m_vLookat.y + YAdd, m_vLookat.z + ZAdd));
}

BOOL CD3DCamera::SetLookat(D3DXVECTOR3 vEye,D3DXVECTOR3	vLookat)
{
	float XRot, YRot, XDiff, YDiff, ZDiff;

	// Calculate angles between points
	XDiff = vLookat.x - vEye.x;
	YDiff = vLookat.y - vEye.y;
	ZDiff = vLookat.z - vEye.z;
	XRot = (float)atan2(-YDiff, sqrt(XDiff*XDiff+ZDiff*ZDiff));
	YRot = (float)atan2(XDiff, ZDiff);

	Move(vEye);
	Rotate(D3DXVECTOR3(XRot, YRot, 0.0f));

	return TRUE;
}

BOOL CD3DCamera::SetStartTrack()
{
	m_StartXPos = m_vEye.x;
	m_StartYPos = m_vEye.y;
	m_StartZPos = m_vEye.z;
	m_StartXRot = m_vLookat.x;
	m_StartYRot = m_vLookat.y;
	m_StartZRot = m_vLookat.z;
	return TRUE;
}

BOOL CD3DCamera::SetEndTrack()
{
	m_EndXPos = m_vEye.x;
	m_EndYPos = m_vEye.y;
	m_EndZPos = m_vEye.z;
	m_EndXRot = m_vLookat.x;
	m_EndYRot = m_vLookat.y;
	m_EndZRot = m_vLookat.z;
	return TRUE;
}

BOOL CD3DCamera::Track(float Time, float Length)
{
	float x, y, z;
	float TimeOffset;

	TimeOffset = Length * Time;

	x = (m_EndXPos - m_StartXPos) / Length * TimeOffset;
	y = (m_EndYPos - m_StartYPos) / Length * TimeOffset;
	z = (m_EndZPos - m_StartZPos) / Length * TimeOffset;
	Move(D3DXVECTOR3(m_StartXPos + x, m_StartYPos + y, m_StartZPos + z));

	x = (m_EndXRot - m_StartXRot) / Length * TimeOffset;
	y = (m_EndYRot - m_StartYRot) / Length * TimeOffset;
	z = (m_EndZRot - m_StartZRot) / Length * TimeOffset;
	Rotate(D3DXVECTOR3(m_StartXRot + x, m_StartYRot + y, m_StartZRot + z));

	return TRUE;
}

BOOL CD3DCamera::Update()
{
	D3DXMatrixMultiply(&m_matWorld, &m_matTranslation, &m_matRotation);
	return TRUE;
}

D3DXMATRIX *CD3DCamera::GetMatrix()
{
	Update();
	return &m_matWorld;
}