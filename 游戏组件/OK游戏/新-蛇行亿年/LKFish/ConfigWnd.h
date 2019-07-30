#pragma once
#include "afxwin.h"
#include "resource.h"
#include "BassMusicManager.h"
#include "factory.h"
#define WM_DX_RENDER					WM_APP+100			//��Ⱦ��Ϣ
#include "magic/platform_win_posix.h"
#include "magic/mp_wrap.h"
#include "Zlib/unzip.h"
using namespace std;

//�ַ�ת��
BOOL WCharToMByte2(LPCWSTR lpcwszStr, string &str) ;




class CConfigWnd
{
public:
	CConfigWnd(void);
	~CConfigWnd(void);
	//λ��X����
	int m_ptx;
	//λ��Y����
	int m_pty;
	//�Ƿ���ʾ
	bool m_isvisable;
	//
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	//��ʼ��
	void InitDlg(LPDIRECT3DDEVICE9 g_pd3dDevice,int x,int y);
	//��ʼ����
	void InitRender(LPD3DXSPRITE  m_pSprite);
	//
	//���뱳��
	LPDIRECT3DTEXTURE9      m_LoadBgTexture;
	//��Ť
	LPDIRECT3DTEXTURE9      m_btText[2];
	//����������
	LPDIRECT3DTEXTURE9      g_sliderbgTex ;  
	//����������
	LPDIRECT3DTEXTURE9      g_sliderperTex ;  
	//
	LPDIRECT3DTEXTURE9      g_minbt ;  
	//
	LPDIRECT3DTEXTURE9      g_maxbt ;  
	//��������Ť
	LPDIRECT3DTEXTURE9      g_sliderbuttonTex ;  
	void  LMouseDown(int ptx,int pty);
	void  LMouseUp(int ptx,int pty);
	void  LMouseMove(int ptx,int pty);
	float   m_value1;
	float   m_value2;
	int     m_ptx1;
	int     m_ptx2;
	int   m_oldptx;
	bool  m_s1;
	bool  m_s2;
	bool  m_ismove;
	int   m_bt1;
	int   m_bt2;
	static bool unzipFile(std::string filename);
	void LoadNorImg1(LPDIRECT3DTEXTURE9* ppTexture, LPCWSTR filename);

};
