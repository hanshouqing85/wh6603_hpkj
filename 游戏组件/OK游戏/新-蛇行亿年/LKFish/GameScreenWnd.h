/*
主屏幕场景类,这个类负责所有图片的显示
*/
#pragma once
#include "afxwin.h"
#include "gamestruct.h"
#include "resource.h"
#include "configwnd.h"
#include "BassMusicManager.h"
#include "factory.h"
#define WM_DX_RENDER					WM_APP+100			//渲染消息
#include "magic/platform_win_posix.h"
#include "magic/mp_wrap.h"
#include "Zlib/unzip.h"
//引入DX文件

#include <d3dx9.h>
#include <d3d9.h>
#pragma comment (lib,"d3dx9.lib")
#pragma comment (lib,"d3d9.lib")
#include <vector>

using namespace std;
//引入完毕
class GameScreenWnd :public CWnd
{
public:
	//创建函数
	GameScreenWnd(void);
	//解析函数
	~GameScreenWnd(void);
private:
	;
	//成员变量
	//鱼儿游动图数量
	static const  int              m_FishmoveCount[FISHCOUNT];
	///鱼儿死亡图数量
	static const  int              m_FishdeadCount[FISHCOUNT];
	//活图大小
	static const  int              m_FishmoveRec[FISHCOUNT][2];
	//死图大小
    static const  int              m_FishDeadRec[FISHCOUNT][2];
	//鱼儿分数
	static const  int              m_fishscore[FISHCOUNT];
	//
	static const  int              PathIndex[45][5][5];
	//
	//
	bool                    m_IsIntro;
	//粒子类
    MP_Device_WRAP *        device;
	
	//玩家炮坐标
	POINT                   m_UserPT[USERCOUNT]; 
	//炮管位置
	POINT                   m_UserPaoPT[USERCOUNT]; 
	//自己座位序列号
	int                     m_MeChairID;
	//自己座位真实号
	int                     m_MeRellayChairID;
	//
	int                     m_CellScore;
	//玩家数组
	bool                    m_HaveUser[USERCOUNT];
	//玩加能量
	float                     m_PowerCount[USERCOUNT];
	//真实玩家椅子号
	int                     m_RealChairID[USERCOUNT];
	//炮管角度
	float                   m_UserPaoJiaodu[USERCOUNT];
	//发射状态
	int                     m_UserSendstate[USERCOUNT];
	//自己子弹
	Bullet                  m_MeBullet[200];
	//别人子弹
    Bullet                  m_OterBullet[100];
	//上分数据
	int                     m_UserScore[USERCOUNT];
	//鱼儿数组
	Fish                    m_FishArr[250];
	//飘落金币数组
	Gold                    m_GoldArr[100];
	//坐标数组
	FishTrace               m_FishTrace[250][5];
	//大怪凋落动画数组
	MuchMoneyaction         m_MuchmoneyAct[USERCOUNT];
	//
	//大怪凋落动画数组
	Zhaaction               m_zhaaction[USERCOUNT];
	//
	Numstruct               m_NumArr[50];
	//鼠标是否移动到按扭上
	bool                    m_IsMouseOver[9];
	//按扭区域
	CRect                   m_BtRect[9];
	//玩家倍率
	int                     m_BeiLv[USERCOUNT];
	//玩家钱堆
	MoneyDui                m_UserStruct[USERCOUNT][3];

	
	//是否定住
	bool                    m_IsDingzhu;
	//是否超级泡
	bool                    m_UserSuperPao[USERCOUNT];
	//已经加载数量
	int                     m_HaveLoadNum;
	//是否进入游戏
	bool                    m_InitScreen;
	//背景序列
	int                     m_Bgindex;
	//是否处于切换场景状态
	bool                    m_IsChangeScreen;
	//
	Soundmstruct            m_SoundArr[100]; 
	//浪潮数据 
	WaterWavestruct         m_ChanwaveSt;
	//备用数据,在整个程序里面递增，可以根据这个数据来设置子弹颜色切换等
	int                     m_ActionIndex;
	//
	int                     m_ActionIndex1;
	int                     bgmusicindex;
	//按下时间用来设置发射子弹间隔时间
	int                     m_NowTime;
	//是否鼠标按下
	bool                    m_MouseDown;
	//用户名
	LPCTSTR                 m_Username;
	//积分信息
	DOUBLE                m_MyScore;
	//
	int                     m_SoundIndex;
	//
	CFactoryTemplate<Fish>	m_FishFactory;							//鱼群工厂
	//设置窗口
	CConfigWnd              m_configDlg;
	//是否显示时间
	bool                    m_IsMustOut;
	//当前发泡剩余退出时间
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
	//鱼群工厂
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
	//是否全屏
	bool                     m_IsFullScreen;
	//是否正在锁定设计
	bool							m_bLockShoot;							//【连发模式】
	bool							m_bLockFish;							//锁定大鱼
private:

	SuoStruct               m_SuoPao[6][10];
	//
    //锁定鱼ID
	int                     m_SuoFishId[6];
	//
	TCHAR                   m_Infomation[500];
	//
	FontStruct              m_FontCurr[20];
	//
	FireStructAll            m_FireStruc[10];
	
	TCHAR					m_szTitle[128];
private:
	//窗口句柄
	HWND					m_hWnd;
	//绘制界面线程句柄
	HANDLE 					m_hDxThread;	
	//判定捕获线程句柄
	HANDLE 					m_hWorkThread;	
	//
	HANDLE 					m_hWorkThread1;	
	//DX设备
    LPDIRECT3D9             m_pD3D;
	//字
	ID3DXFont*              m_Dxfont;
	//
    ID3DXFont*              m_DxfontA;
	//
	ID3DXFont*				m_DxfontUserName;

	LPDIRECT3DDEVICE9       m_pd3dDevice;
	//创建绘制精灵
	LPD3DXSPRITE            m_pSprite;
	//
	D3DPRESENT_PARAMETERS	m_d3dpp;				//显示参数
	//
	D3DDISPLAYMODE			m_d3ddm;				//显示模式
	//背景声音
	//

	//
//资源TEXTURE
private:
	//背景图片TEXT
	LPDIRECT3DTEXTURE9      m_bgImgTexture[3];  
	//介绍
	LPDIRECT3DTEXTURE9      m_IntroImgTexture; 
	//游戏边框TEXT
	LPDIRECT3DTEXTURE9      m_BorderImgTexture;   
	//炮底座
    LPDIRECT3DTEXTURE9      m_PaoConImgTexture;   
	//炮背景
	LPDIRECT3DTEXTURE9      m_PaoBgImgTexture;   
	//普通炮
	LPDIRECT3DTEXTURE9      m_PaoTwoImgTexture;   
	//离子炮
	LPDIRECT3DTEXTURE9      m_SipperPaoTexture[3];   
 
	//上下分的背景图
    LPDIRECT3DTEXTURE9      m_allscoreBgImgTexture[USERCOUNT];   
	//数字图片
	LPDIRECT3DTEXTURE9      m_NumImgTexture[5];
	//金币图片
	LPDIRECT3DTEXTURE9      m_GoldImgTexture[2];
	//2根子弹图
    LPDIRECT3DTEXTURE9      m_BulltTwoTexture[3];
	//2根炮子弹网图
	LPDIRECT3DTEXTURE9      m_PaoNetImgTexture[3];   
	//鱼游动图
	LPDIRECT3DTEXTURE9      m_FishMoveTexture[FISHCOUNT];
	//鱼死亡图
	LPDIRECT3DTEXTURE9      m_FishDeadTexture[FISHCOUNT][10];
	//大把钞票图
	LPDIRECT3DTEXTURE9      m_MuchMoneyTexture[3];
	///专圈动画
	LPDIRECT3DTEXTURE9      m_QuanTexture[4];
	///按扭图
	LPDIRECT3DTEXTURE9      m_BtTexture[9];
	//小钱币
	LPDIRECT3DTEXTURE9      m_BiTexture;
	//载入背景
	LPDIRECT3DTEXTURE9      m_LoadBgTexture;
	//浪潮背景
	LPDIRECT3DTEXTURE9      m_WaterWaveTexture[2];
	//波浪背景
	LPDIRECT3DTEXTURE9      m_WaterTexture[32];
	//载入进度
	LPDIRECT3DTEXTURE9      m_LoadTexture;
	//超级炮子弹
	LPDIRECT3DTEXTURE9      m_SupButTexture[3];
	//超级状态图片
	LPDIRECT3DTEXTURE9      m_SupStatTexture;
	//超级状态图片
	LPDIRECT3DTEXTURE9      m_OutTexture;
	//鼠标样式
	LPDIRECT3DTEXTURE9      m_CursorTexture;
	//定动画图
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
	//蛇
	LPDIRECT3DTEXTURE9      m_SankeTure[3];
	//脏鱼死亡
	LPDIRECT3DTEXTURE9      m_BlackWateTure;
	//能量背景
	LPDIRECT3DTEXTURE9      PowerFrameTure;
	//
	LPDIRECT3DTEXTURE9      PowerBarTure;
	//
	LPDIRECT3DTEXTURE9      m_GunFireTextTure;

    LPDIRECT3DTEXTURE9      FontBgTure;



//调用函数
public:
	//委托接口
	BaseWnd *               m_baseSink;
	//
	void SetPower(int PlayerID, float p ){m_PowerCount[PlayerID]=p;};
    //发射子弹
    void UserShoot(int PlayerID,float Roation,int RealChairID,bool isAndroid,int zidanID);
	//加钱
	void UserAddMoney(int PlayerID,int Fishid,int Money,int FishKindScord,bool ishavaction );
	//切换场景
	void ChangeScreen(int bgindex);
	//玩家进入或者出去
	void UserComeInorLeave(int PlayerID,bool ComeOrLeave,int RealUserid);
	//设置自己信息
	void  SetMeInformation(int MePlayerid,int MeRellid ,LPCTSTR myname , DOUBLE myscore);
	//设置玩家倍率
	void SetBeiLv(int PlayerId,int BeiScore)
	{
		m_BeiLv[PlayerId] = BeiScore;
		if(m_configDlg.m_ptx2>0)CBassMusicManager::GetInstance()->Play(71, false);
	};
	//设置玩家分数
	void SetUserScore(int PlayerId,int Score){m_UserScore[PlayerId] = Score;};
	//单元分
	void SetCellScore(int cellscore){m_CellScore = cellscore;};
	//更新自己积分
	void SetBaseScore(DOUBLE myscore)
	{
		m_MyScore =  myscore;
// 		CString strLog;
// 		strLog.Format(L"SXFISH SetBaseScore m_MyScore:%ld myscore:%ld",m_MyScore,myscore);
// 		OutputDebugString(strLog);

	}
	//添加鱼
	void AddFish(int traceX,int traceY,float roation,float movetime,float changetime,int fishtype,int ptindex,int nowindex,int fishid,int smallfishtype,int fishspeed,int likuiscore=0);
	//设置玩家上下分
	//清理所有数据
	void CleanAllMemory();
	//设置超级炮玩家
	void SetSuperPao(int PlayerId,int Fishid){m_UserSuperPao[PlayerId]=true;}
	//取消超级炮玩家
	void CancelSuperPao(int PlayerId){m_UserSuperPao[PlayerId]=false;}
	//猴子被打中爆炸
	void SetBomAction(int PlayerId,int Fishid,int fishscore);
	//棒子定住
	void SetDingAction(Fish *tfish);
	//锁定鱼
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
	//任务文字提示
	void SetFontString(LPCWSTR WString);
	//
	void AddFire(int px,int py);
	//根据传输的鱼线路来添加李逵的分数
	void AddLiKuiScore();



private:
	//大怪爆钱
	void GiveMuchMoney(int Fishindex,int PlayerID,int Money,Fish *Deadfish,int fishscore);
	//大怪爆钱
	void GiveZhaMoney(int Fishindex,int PlayerID,int Money,Fish *Deadfish,int fishscore);
	//载入图片纹理
	void LoadImg(const char* m_path,LPDIRECT3DTEXTURE9*  ppTexture);
	//
	void LoadNorImg(LPCWSTR,LPDIRECT3DTEXTURE9* ppTexture);
    //载入声音
	void LoadSound();
	//
	LRESULT OnFrameRender(WPARAM wParam, LPARAM lParam);
	//
	void LoadNorImg1(LPDIRECT3DTEXTURE9* ppTexture, LPCWSTR filename);
	//
	static bool unzipFile(std::string filename);
	//抽奖过程
	void ChouJiangVoid(int m_index,int score,int player);
	//
	void ChouJiangFinshVoid(int player);
	//
	CPoint drawing(CPoint a,CPoint b,int c);//该函数实现将点a绕点b旋转c度角


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
	//初始化窗口
	bool  InitDxDlog();
	//创建DX窗口及载入资源
	HRESULT InitD3D( HWND hWnd );
	//
	bool                    m_RunRender;
	//
	void                   CheckHit();
	//渲染界面
	void  Render();
	//载入纹理
	void LoadImg();
	//绘制按扭
	void DrawBt();
    //重构消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//设置标题
	void	SetGameTitle(TCHAR* szTitle);

	DECLARE_MESSAGE_MAP()
    //时间事件
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
