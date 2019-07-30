/*****************************************************/
//����Ϸ������:��Ƥ�Ƽ�,��ϵQQ:56248
/*****************************************************/

#pragma once
#include "FishPath.h"
#include "gamescreenwnd.h"
#include "ClientKernel.h"

class CDsfishDlg : public CDialog,public CClientKernelSink,public BaseWnd
{

public:
	CDsfishDlg(CWnd* pParent = NULL);	
    // �Ի�������
	enum { IDD = IDD_Dsfish_DIALOG };
protected:

	virtual void DoDataExchange(CDataExchange* pDX);	
	//��Ϸ��ͼ
	GameScreenWnd   m_GameScreen;
	//�Ƿ�����
	bool                    m_bIsConnect;
	//�ں˵���
    CClientKernel           *m_pClientKernel;
	//�Ƿ����
	bool                            m_havePlayer[6];
	//��������
	int                             m_CurrBeilv;
	//�Ƿ��������
	bool                            m_HaveInitSceen;
	//��Ϸ״̬
	CMD_S_GameScene                 pGameScene;
	//�Ƿ�ȫ��Ļ
	bool                         m_isfullscreen;

	

//���غ���
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

	//�ص��ӿ���д
	//��������
	virtual bool  InitClientMessage();
		//�����ӵ���Ϣ
	virtual bool  OnShoot(bool isSuper,float roalation,DWORD m_bulletID);
		//�˳���Ϸ
	virtual bool CloseExit();
		//�������
	virtual bool HitFirsh(int dwFishID,int dwBulletID,int ShootUSER,bool IsAndroid,int zidanID);
		//���·�
	virtual bool AddOrRemoveScore(bool addorremove,int addscore,bool allscore = false);
		//�л��ڱ���
	virtual bool ChangeFireBei(bool upordown);
	//�л��ڵ��ٶ�
	virtual bool  ChangeFireSpeed(int index);
	//
	virtual bool  SendLockFishID(int FishID,int PlayerID);
	//
	virtual BOOL  PreTranslateMessage(MSG* pMsg);
	//

//
private:
	//�л���λ
	int SwitchChairID(int m_Currchairid);
// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
