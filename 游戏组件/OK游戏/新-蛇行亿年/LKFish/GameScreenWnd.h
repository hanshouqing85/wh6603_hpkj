/*
����Ļ������,����ฺ������ͼƬ����ʾ
*/
#pragma once
#include "afxwin.h"
#include "gamestruct.h"
#include "resource.h"
#include "configwnd.h"
#include "BassMusicManager.h"
#include "factory.h"
#define WM_DX_RENDER					WM_APP+100			//��Ⱦ��Ϣ
#include "magic/platform_win_posix.h"
#include "magic/mp_wrap.h"
#include "Zlib/unzip.h"
//����DX�ļ�

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"d3d9.lib")
#include <vector>

using namespace std;
//�������
class GameScreenWnd :public CWnd
{
public:
	//��������
	GameScreenWnd(void);
	//��������
	~GameScreenWnd(void);
private:
	;
	//��Ա����
	//����ζ�ͼ����
	static const  int              m_FishmoveCount[FISHCOUNT];
	///�������ͼ����
	static const  int              m_FishdeadCount[FISHCOUNT];
	//��ͼ��С
	static const  int              m_FishmoveRec[FISHCOUNT][2];
	//��ͼ��С
    static const  int              m_FishDeadRec[FISHCOUNT][2];
	//�������
	static const  int              m_fishscore[FISHCOUNT];
	//
	static const  int              PathIndex[45][5][5];
	//
	//
	bool                    m_IsIntro;
	//������
    MP_Device_WRAP *        device;
	
	//���������
	POINT                   m_UserPT[USERCOUNT]; 
	//�ڹ�λ��
	POINT                   m_UserPaoPT[USERCOUNT]; 
	//�Լ���λ���к�
	int                     m_MeChairID;
	//�Լ���λ��ʵ��
	int                     m_MeRellayChairID;
	//
	int                     m_CellScore;
	//�������
	bool                    m_HaveUser[USERCOUNT];
	//�������
	float                     m_PowerCount[USERCOUNT];
	//��ʵ������Ӻ�
	int                     m_RealChairID[USERCOUNT];
	//�ڹܽǶ�
	float                   m_UserPaoJiaodu[USERCOUNT];
	//����״̬
	int                     m_UserSendstate[USERCOUNT];
	//�Լ��ӵ�
	Bullet                  m_MeBullet[200];
	//�����ӵ�
    Bullet                  m_OterBullet[100];
	//�Ϸ�����
	int                     m_UserScore[USERCOUNT];
	//�������
	Fish                    m_FishArr[250];
	//Ʈ��������
	Gold                    m_GoldArr[100];
	//��������
	FishTrace               m_FishTrace[250][5];
	//��ֵ��䶯������
	MuchMoneyaction         m_MuchmoneyAct[USERCOUNT];
	//
	//��ֵ��䶯������
	Zhaaction               m_zhaaction[USERCOUNT];
	//
	Numstruct               m_NumArr[50];
	//����Ƿ��ƶ�����Ť��
	bool                    m_IsMouseOver[9];
	//��Ť����
	CRect                   m_BtRect[9];
	//��ұ���
	int                     m_BeiLv[USERCOUNT];
	//���Ǯ��
	MoneyDui                m_UserStruct[USERCOUNT][3];

	
	//�Ƿ�ס
	bool                    m_IsDingzhu;
	//�Ƿ񳬼���
	bool                    m_UserSuperPao[USERCOUNT];
	//�Ѿ���������
	int                     m_HaveLoadNum;
	//�Ƿ������Ϸ
	bool                    m_InitScreen;
	//��������
	int                     m_Bgindex;
	//�Ƿ����л�����״̬
	bool                    m_IsChangeScreen;
	//
	Soundmstruct            m_SoundArr[100]; 
	//�˳����� 
	WaterWavestruct         m_ChanwaveSt;
	//��������,����������������������Ը�����������������ӵ���ɫ�л���
	int                     m_ActionIndex;
	//
	int                     m_ActionIndex1;
	int                     bgmusicindex;
	//����ʱ���������÷����ӵ����ʱ��
	int                     m_NowTime;
	//�Ƿ���갴��
	bool                    m_MouseDown;
	//�û���
	LPCTSTR                 m_Username;
	//������Ϣ
	DOUBLE                m_MyScore;
	//
	int                     m_SoundIndex;
	//
	CFactoryTemplate<Fish>	m_FishFactory;							//��Ⱥ����
	//���ô���
	CConfigWnd              m_configDlg;
	//�Ƿ���ʾʱ��
	bool                    m_IsMustOut;
	//��ǰ����ʣ���˳�ʱ��
	int                     m_OutTime;
	//
	CursorStruct            m_cursor;
	//
	int                     m_CursorInt;
	//
	Dingaction              m_DingStruc;
	//
	int                     m_SendTimeN;
	//
	DoudongStruct           m_doudongstu;
	//
	int                     m_SuoDingFishid;
	//
	int                     m_SuoDingFishIndex;
	//
	bool                    m_ScienceZhuan;
	//��Ⱥ����
	CFactoryTemplate<FishPack>	m_FishStorg;	
	//
	ChoujiangStruct         m_ChouJiang[6];
	//
	int                     m_scienceindex;
	//
	bool                    m_IsAndroid[USERCOUNT];
	//
	bool                    m_isxiuyuqi;
	//
	double Definex_bli;  
	double Definey_bli; 
	//
	//�Ƿ�ȫ��
	bool                     m_IsFullScreen;
	//�Ƿ������������
	bool							m_bLockShoot;							//������ģʽ��
	bool							m_bLockFish;							//��������
private:

	SuoStruct               m_SuoPao[6][10];
	//
    //������ID
	int                     m_SuoFishId[6];
	//
	TCHAR                   m_Infomation[500];
	//
	FontStruct              m_FontCurr[20];
	//
	FireStructAll            m_FireStruc[10];
	
	TCHAR					m_szTitle[128];
private:
	//���ھ��
	HWND					m_hWnd;
	//���ƽ����߳̾��
	HANDLE 					m_hDxThread;	
	//�ж������߳̾��
	HANDLE 					m_hWorkThread;	
	//
	HANDLE 					m_hWorkThread1;	
	//DX�豸
    LPDIRECT3D9             m_pD3D;
	//��
	ID3DXFont*              m_Dxfont;
	//
    ID3DXFont*              m_DxfontA;
	//
	ID3DXFont*				m_DxfontUserName;

	LPDIRECT3DDEVICE9       m_pd3dDevice;
	//�������ƾ���
	LPD3DXSPRITE            m_pSprite;
	//
	D3DPRESENT_PARAMETERS	m_d3dpp;				//��ʾ����
	//
	D3DDISPLAYMODE			m_d3ddm;				//��ʾģʽ
	//��������
	//

	//
//��ԴTEXTURE
private:
	//����ͼƬTEXT
	LPDIRECT3DTEXTURE9      m_bgImgTexture[3];  
	//����
	LPDIRECT3DTEXTURE9      m_IntroImgTexture; 
	//��Ϸ�߿�TEXT
	LPDIRECT3DTEXTURE9      m_BorderImgTexture;   
	//�ڵ���
    LPDIRECT3DTEXTURE9      m_PaoConImgTexture;   
	//�ڱ���
	LPDIRECT3DTEXTURE9      m_PaoBgImgTexture;   
	//��ͨ��
	LPDIRECT3DTEXTURE9      m_PaoTwoImgTexture;   
	//������
	LPDIRECT3DTEXTURE9      m_SipperPaoTexture[3];   
 
	//���·ֵı���ͼ
    LPDIRECT3DTEXTURE9      m_allscoreBgImgTexture[USERCOUNT];   
	//����ͼƬ
	LPDIRECT3DTEXTURE9      m_NumImgTexture[5];
	//���ͼƬ
	LPDIRECT3DTEXTURE9      m_GoldImgTexture[2];
	//2���ӵ�ͼ
    LPDIRECT3DTEXTURE9      m_BulltTwoTexture[3];
	//2�����ӵ���ͼ
	LPDIRECT3DTEXTURE9      m_PaoNetImgTexture[3];   
	//���ζ�ͼ
	LPDIRECT3DTEXTURE9      m_FishMoveTexture[FISHCOUNT];
	//������ͼ
	LPDIRECT3DTEXTURE9      m_FishDeadTexture[FISHCOUNT][10];
	//��ѳ�Ʊͼ
	LPDIRECT3DTEXTURE9      m_MuchMoneyTexture[3];
	///רȦ����
	LPDIRECT3DTEXTURE9      m_QuanTexture[4];
	///��Ťͼ
	LPDIRECT3DTEXTURE9      m_BtTexture[9];
	//СǮ��
	LPDIRECT3DTEXTURE9      m_BiTexture;
	//���뱳��
	LPDIRECT3DTEXTURE9      m_LoadBgTexture;
	//�˳�����
	LPDIRECT3DTEXTURE9      m_WaterWaveTexture[2];
	//���˱���
	LPDIRECT3DTEXTURE9      m_WaterTexture[32];
	//�������
	LPDIRECT3DTEXTURE9      m_LoadTexture;
	//�������ӵ�
	LPDIRECT3DTEXTURE9      m_SupButTexture[3];
	//����״̬ͼƬ
	LPDIRECT3DTEXTURE9      m_SupStatTexture;
	//����״̬ͼƬ
	LPDIRECT3DTEXTURE9      m_OutTexture;
	//�����ʽ
	LPDIRECT3DTEXTURE9      m_CursorTexture;
	//������ͼ
	LPDIRECT3DTEXTURE9      m_DingTexture[10];
	//
	LPDIRECT3DTEXTURE9      m_ZhaTexture[10];
	//
	LPDIRECT3DTEXTURE9      m_LockFishTexture;
	//
	LPDIRECT3DTEXTURE9      m_FireAllTexture;
	//
	LPDIRECT3DTEXTURE9      m_CoinUp;
	//
	LPDIRECT3DTEXTURE9      m_SuoBall[6];
	//
	LPDIRECT3DTEXTURE9      m_ChoujiangTure[2];
	//��
	LPDIRECT3DTEXTURE9      m_SankeTure[3];
	//��������
	LPDIRECT3DTEXTURE9      m_BlackWateTure;
	//��������
	LPDIRECT3DTEXTURE9      PowerFrameTure;
	//
	LPDIRECT3DTEXTURE9      PowerBarTure;
	//
	LPDIRECT3DTEXTURE9      m_GunFireTextTure;

    LPDIRECT3DTEXTURE9      FontBgTure;



//���ú���
public:
	//ί�нӿ�
	BaseWnd *               m_baseSink;
	//
	void SetPower(int PlayerID, float p ){m_PowerCount[PlayerID]=p;};
    //�����ӵ�
    void UserShoot(int PlayerID,float Roation,int RealChairID,bool isAndroid,int zidanID);
	//��Ǯ
	void UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction );
	//�л�����
	void ChangeScreen(int bgindex);
	//��ҽ�����߳�ȥ
	void UserComeInorLeave(int PlayerID,bool ComeOrLeave,int RealUserid);
	//�����Լ���Ϣ
	void  SetMeInformation(int MePlayerid,int MeRellid ,LPCTSTR myname , DOUBLE myscore);
	//������ұ���
	void SetBeiLv(int PlayerId,int BeiScore)
	{
		m_BeiLv[PlayerId] = BeiScore;
		if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(71, false);
	};
	//������ҷ���
	void SetUserScore(int PlayerId,int Score){m_UserScore[PlayerId] = Score;};
	//��Ԫ��
	void SetCellScore(int cellscore){m_CellScore = cellscore;};
	//�����Լ�����
	void SetBaseScore(DOUBLE myscore)
	{
		m_MyScore =  myscore;
// 		CString strLog;
// 		strLog.Format(L"SXFISH SetBaseScore m_MyScore:%ld myscore:%ld",m_MyScore,myscore);
// 		OutputDebugString(strLog);

	}
	//�����
	void AddFish(int traceX,int traceY,float roation,float movetime,float changetime,int fishtype,int ptindex,int nowindex,int fishid,int smallfishtype,int fishspeed,int likuiscore=0);
	//����������·�
	//������������
	void CleanAllMemory();
	//���ó��������
	void SetSuperPao(int PlayerId,int Fishid){m_UserSuperPao[PlayerId]=true;}
	//ȡ�����������
	void CancelSuperPao(int PlayerId){m_UserSuperPao[PlayerId]=false;}
	//���ӱ����б�ը
	void SetBomAction(int PlayerId,int Fishid,int fishscore);
	//���Ӷ�ס
	void SetDingAction(Fish *tfish);
	//������
	void LockFishID(int FishId,int PlayerId)
	{
		m_SuoFishId[PlayerId]=FishId;


	
	};
	//
	void AddStorgeFish(FishPack *m_fishpack);
	//
	void CreatRegFishOne(FishPack *m_fishpack);
	//
	void CreatRegFishTwo(FishPack *m_fishpack);
	//
	void CreatRegFishThree(FishPack *m_fishpack);
	//����������ʾ
	void SetFontString(LPCWSTR WString);
	//
	void AddFire(int px,int py);
	//���ݴ��������·��������ӵķ���
	void AddLiKuiScore();



private:
	//��ֱ�Ǯ
	void GiveMuchMoney(int Fishindex,int PlayerID,int Money,Fish *Deadfish,int fishscore);
	//��ֱ�Ǯ
	void GiveZhaMoney(int Fishindex,int PlayerID,int Money,Fish *Deadfish,int fishscore);
	//����ͼƬ����
	void LoadImg(const char* m_path,LPDIRECT3DTEXTURE9*  ppTexture);
	//
	void LoadNorImg(LPCWSTR,LPDIRECT3DTEXTURE9* ppTexture);
    //��������
	void LoadSound();
	//
	LRESULT OnFrameRender(WPARAM wParam, LPARAM lParam);
	//
	void LoadNorImg1(LPDIRECT3DTEXTURE9* ppTexture, LPCWSTR filename);
	//
	static bool unzipFile(std::string filename);
	//�齱����
	void ChouJiangVoid(int m_index,int score,int player);
	//
	void ChouJiangFinshVoid(int player);
	//
	CPoint drawing(CPoint a,CPoint b,int c);//�ú���ʵ�ֽ���a�Ƶ�b��תc�Ƚ�


public:
	//
	void SetFullScreen(bool m_isfull);
	void SetSYSFontString(LPCWSTR WString)
       {
	
		  if(m_FontCurr[19].m_Have==false)
	        {
	           lstrcpyn(m_FontCurr[19].m_Infomation,WString,500);
	           m_FontCurr[19].m_Curr=CLIENT_WIDTH;
	           m_FontCurr[19].m_Have=true;

	        }
       };
	//��ʼ������
	bool  InitDxDlog();
	//����DX���ڼ�������Դ
	HRESULT InitD3D( HWND hWnd );
	//
	bool                    m_RunRender;
	//
	void                   CheckHit();
	//��Ⱦ����
	void  Render();
	//��������
	void LoadImg();
	//���ư�Ť
	void DrawBt();
    //�ع���Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ñ���
	void	SetGameTitle(TCHAR* szTitle);

	DECLARE_MESSAGE_MAP()
    //ʱ���¼�
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
