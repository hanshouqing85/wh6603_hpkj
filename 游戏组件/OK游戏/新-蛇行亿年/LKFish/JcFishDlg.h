/*****************************************************/
//本游戏开发者:哈皮科技,联系QQ:56248
/*****************************************************/

#pragma once
#include "FishPath.h"
#include "gamescreenwnd.h"
#include "ClientKernel.h"

class CDsfishDlg : public CDialog,public CClientKernelSink,public BaseWnd
{

public:
	CDsfishDlg(CWnd* pParent = NULL);	
    // 对话框数据
	enum { IDD = IDD_Dsfish_DIALOG };
protected:

	virtual void DoDataExchange(CDataExchange* pDX);	
	//游戏视图
	GameScreenWnd   m_GameScreen;
	//是否连接
	bool                    m_bIsConnect;
	//内核调度
    CClientKernel           *m_pClientKernel;
	//是否玩家
	bool                            m_havePlayer[6];
	//基础倍率
	int                             m_CurrBeilv;
	//是否载入完毕
	bool                            m_HaveInitSceen;
	//游戏状态
	CMD_S_GameScene                 pGameScene;
	//是否全屏幕
	bool                         m_isfullscreen;

	

//重载函数
public:
    virtual void ResetGameFrame() {}
    virtual void CloseGameFrame() {}
	virtual bool OnEventSocket(const TCP_Command & Command, const void * pBuffer, WORD wDataSize);
    virtual bool OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
	virtual void OnEventUserMemberOrder(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}
    virtual void OnEventUserStatus(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser){return ;};
    virtual bool OnEventUserLeave(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser);
    virtual bool OnEventUserEnter(tagUserInfo * pUserData, WORD wChairID, bool bLookonUserr);
    virtual void OnEventUserScore(tagUserInfo * pUserData, WORD wChairID, bool bLookonUser) {}

	//回调接口重写
	//创建调用
	virtual bool  InitClientMessage();
		//发送子弹消息
	virtual bool  OnShoot(bool isSuper,float roalation,DWORD m_bulletID);
		//退出游戏
	virtual bool CloseExit();
		//击中鱼儿
	virtual bool HitFirsh(int dwFishID,int dwBulletID,int ShootUSER,bool IsAndroid,int zidanID);
		//上下分
	virtual bool AddOrRemoveScore(bool addorremove,int addscore,bool allscore = false);
		//切换炮倍率
	virtual bool ChangeFireBei(bool upordown);
	//切换炮的速度
	virtual bool  ChangeFireSpeed(int index);
	//
	virtual bool  SendLockFishID(int FishID,int PlayerID);
	//
	virtual BOOL  PreTranslateMessage(MSG* pMsg);
	//

//
private:
	//切换座位
	int SwitchChairID(int m_Currchairid);
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
