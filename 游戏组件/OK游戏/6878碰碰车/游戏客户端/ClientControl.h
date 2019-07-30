#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//重置界面
	virtual void __cdecl ResetUserBet() = NULL;
	//玩家昵称
	virtual void __cdecl ResetUserNickName()=NULL;
	//获取昵称
	virtual void __cdecl GetCheckNickName(CString &strNickName)=NULL;
	//玩家昵称
	virtual void __cdecl SetUserNickName(LPCTSTR lpszNickName)=NULL;
	//下注信息
	virtual void __cdecl SetUserBetScore(BYTE cbArea,SCORE lScore) = NULL;
	//下注信息
	virtual void __cdecl SetAllUserBetScore(BYTE cbArea,SCORE lScore) = NULL;
	//更新控制
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
};
