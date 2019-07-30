#include "StdAfx.h"
#include ".\configwnd.h"



CConfigWnd::CConfigWnd(void)
{
	m_isvisable = false;
	m_value1 = 0;
	m_value2 = 0;
	m_ptx1 = 330;
	m_ptx2 = 330;
	m_s1 = false;
	m_s2 = false;
	m_ismove = false;
	m_bt2 = 1;
	m_bt1 = 1;
}

CConfigWnd::~CConfigWnd(void)
{
}
//
void CConfigWnd::InitDlg(LPDIRECT3DDEVICE9 g_pd3dDevice,int x,int y)
{
	m_pd3dDevice = g_pd3dDevice;
	//载入背景图
	LoadNorImg1(&m_LoadBgTexture,L"setDlg.png");
	LoadNorImg1(&m_btText[0],L"BT_OK.png");
	LoadNorImg1(&g_maxbt,L"setDlgCheck1.png");
	LoadNorImg1(&g_minbt,L"setDlgCheck1.png");
	m_ptx = x;
	m_pty = y;

};
//
void CConfigWnd::InitRender(LPD3DXSPRITE  m_pSprite)
{
	if(!m_isvisable)return;
	//背景
	m_pSprite->Draw(m_LoadBgTexture, NULL, NULL, &D3DXVECTOR3(m_ptx,m_pty, 0), D3DCOLOR_ARGB(255,255,255,255));

			CRect rcClient;
	        rcClient.top=0;
		    rcClient.left=0;
		    rcClient.bottom= rcClient.top + 30 ;
	        rcClient.right=  rcClient.left +100;

	m_pSprite->Draw(m_btText[0], rcClient, NULL, &D3DXVECTOR3(m_ptx+150,m_pty+180, 0), D3DCOLOR_ARGB(255,255,255,255));

	        rcClient.top=0;
		    rcClient.left=m_bt1*22;
		    rcClient.bottom= rcClient.top + 24 ;
	        rcClient.right=  rcClient.left +22;

	m_pSprite->Draw(g_minbt, rcClient, NULL, &D3DXVECTOR3(m_ptx+95,m_pty+120, 0), D3DCOLOR_ARGB(255,255,255,255));

        	 rcClient.top=0;
		    rcClient.left=m_bt2*22;
		    rcClient.bottom= rcClient.top + 24 ;
	        rcClient.right=  rcClient.left +22;

	m_pSprite->Draw(g_maxbt, rcClient, NULL, &D3DXVECTOR3(m_ptx+230,m_pty+120, 0), D3DCOLOR_ARGB(255,255,255,255));


   
}
void  CConfigWnd::LMouseDown(int ptx,int pty)
{
	if(!m_isvisable)return;
	POINT m_point;
	m_point.x = ptx;
	m_point.y = pty;

    CRect m_rect;
	m_rect.top = m_pty+180;
	m_rect.bottom = m_rect.top + 30;
	m_rect.left = m_ptx+150;
	m_rect.right = m_rect.left + 100;
	if(m_rect.PtInRect(m_point))m_isvisable=false;
	//
	m_rect.top = m_pty+225;
	m_rect.bottom = m_rect.top + 52;
	m_rect.left = m_ptx+70;
	m_rect.right = m_rect.left + 150;
	if(m_rect.PtInRect(m_point))m_isvisable=false;
	//
	m_rect.top = m_pty+86;
	m_rect.bottom = m_rect.top + 30;
	m_rect.left = m_ptx+35+m_ptx1;
	m_rect.right = m_rect.left + 30;
	if(m_rect.PtInRect(m_point)){m_s1 =  true;}
	//
	m_rect.top = m_pty+167;
	m_rect.bottom = m_rect.top + 30;
	m_rect.left = m_ptx+35+(m_ptx2);
	m_rect.right = m_rect.left + 30;
	if(m_rect.PtInRect(m_point))m_s2 =  true;
	
	m_rect.top = m_pty+120;
	m_rect.bottom = m_rect.top + 24;
	m_rect.left = m_ptx+95;
	m_rect.right = m_rect.left + 22;
	if(m_rect.PtInRect(m_point))
	{
	   
		
		if(m_bt1==1)
		{
			CBassMusicManager::GetInstance()->SetVolumn(0);
			m_bt1 = 0;
			//m_bt2 = 0;
		}
		else
		{
		    CBassMusicManager::GetInstance()->SetVolumn(100);
			m_bt1 = 1;
			//m_bt2 = 1;
		}
		
	}
	m_rect.top = m_pty+120;
	m_rect.bottom = m_rect.top + 24;
	m_rect.left = m_ptx+230;
	m_rect.right = m_rect.left + 222;
	if(m_rect.PtInRect(m_point))
	{
        if(m_bt2==1)
		{
			CBassMusicManager::GetInstance()->SetVolumn(0);
			//m_bt1 = 0;
			m_bt2 = 0;
		}
		else
		{
		    CBassMusicManager::GetInstance()->SetVolumn(100);
			//m_bt1 = 1;
			m_bt2 = 1;
		}
		
	}
	
	m_ismove = true;
	

}
void  CConfigWnd::LMouseUp(int ptx,int pty)
{
	m_s1 =  false;
	m_s2 =  false;
	m_ismove= false;

  
}
void  CConfigWnd::LMouseMove(int ptx,int pty)
{
	if(!m_isvisable)return;
	//if(m_ismove)
	//{
	  // this->m_ptx = ptx - m_ptx;
	  // return;
	//}
	if(m_s1)
	{
		m_ptx1 =  ptx - m_ptx-40;
		if(m_ptx1<40)m_ptx1=40;
		if(m_ptx1>330)m_ptx1=330;
		CBassMusicManager::GetInstance()->SetVolumn(m_ptx1/3);
		if(m_ptx1==40)CBassMusicManager::GetInstance()->SetVolumn(0);

	}
	if(m_s2)
	{
		m_ptx2 =  ptx - m_ptx-40;
		if(m_ptx2<40)m_ptx2=40;
		if(m_ptx2>330)m_ptx2=330;
		CBassMusicManager::GetInstance()->SetVolumn(m_ptx2/3);
		if(m_ptx2==40)CBassMusicManager::GetInstance()->SetVolumn(0);


	}
   
}

//字符转换
BOOL WCharToMByte2(LPCWSTR lpcwszStr, string &str) 
{  
	DWORD dwMinSize = 0;   
	LPSTR lpszStr = NULL;  
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);   
	if(0 == dwMinSize) 
	   { 
		   return FALSE;  
	   }  
	lpszStr = new char [dwMinSize];
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwMinSize,NULL,FALSE); 
	str = lpszStr;   
	delete [] lpszStr;  
	return TRUE;  
}
void CConfigWnd::LoadNorImg1(LPDIRECT3DTEXTURE9* ppTexture, LPCWSTR filename)
{
    string tempurl;
	WCharToMByte2(filename,tempurl);
	unzipFile(tempurl);
	CString filepath;
	filepath.Format(L"%s%s",DIRPATH,filename);
	OutputDebugString(filepath);
   do{
	    if(!FAILED(D3DXCreateTextureFromFileEx(m_pd3dDevice,filepath,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_FILTER_NONE,D3DX_DEFAULT,0xfffff000,NULL,NULL,ppTexture)))
		{
			break;
		}

	 }while(true);

   DeleteFile(filepath);
}
//解压资源

//解压资源
#define C1 528459
#define C2 227191
bool CConfigWnd::unzipFile(string filenamea)
{
   HZIP hz;
  //载入密码读资源

  CString Result,str,S;
  int i,j;
  WORD Key=56248;
  S=L"QQ44224133QQ" ;
  Result=S;
  for(i=0; i<S.GetLength(); i++) 
    {  
       Result.SetAt(i, S.GetAt(i)^(Key>>8));
       Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2;
    }
    S=Result; 
    Result.Empty();
    for(i=0; i<S.GetLength(); i++) 
    {
      j=(BYTE)S.GetAt(i);
      str="12"; 
      str.SetAt(0, 97+j/26);
      str.SetAt(1, 97+j%26);
      Result += str;
    }

  DWORD dwMinSize = 0;   
  LPSTR lpszStr = NULL;  
  LPCWSTR   m_Password1=Result;
  dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,m_Password1,-1,NULL,0,NULL,FALSE);   
  if(0 == dwMinSize) 
	   { 
		   return FALSE;  
	   }  
  lpszStr = new char [dwMinSize];
  WideCharToMultiByte(CP_OEMCP,NULL,m_Password1,-1,lpszStr,dwMinSize,NULL,FALSE); 
  hz = OpenZip(L"SXfish/Res", lpszStr);
 // OutputDebugStringA(lpszStr);
  delete [] lpszStr;  
  //结束读取

  ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
  for (int zi=0; zi<numitems; zi++)
  {
	GetZipItem(hz,zi,&ze);
	string tempurl;
	WCharToMByte2(ze.name,tempurl);
	if(filenamea==tempurl)
      UnzipItem(hz,zi,ze.name);
	
  }
  CloseZip(hz);
  return 1;
}
//
//bool CConfigWnd::unzipFile(std::string filenamea)
//{
//  HZIP hz;
//  hz = OpenZip(L"SXfish/Res",	RARPASSWORD);
//  ZIPENTRY ze; GetZipItem(hz,-1,&ze); 
//  int numitems=ze.index;
//  for (int zi=0; zi<numitems; zi++)
//  {
//	GetZipItem(hz,zi,&ze);
//	string tempurl;
//	WCharToMByte1(ze.name,tempurl);
//	if(filenamea==tempurl)
//      UnzipItem(hz,zi,ze.name);
//	
//  }
//  CloseZip(hz);
//  return 1;
//}