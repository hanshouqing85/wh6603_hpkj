#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���ý���
	virtual void __cdecl ResetUserBet() = NULL;
	//����ǳ�
	virtual void __cdecl ResetUserNickName()=NULL;
	//��ȡ�ǳ�
	virtual void __cdecl GetCheckNickName(CString &strNickName)=NULL;
	//����ǳ�
	virtual void __cdecl SetUserNickName(LPCTSTR lpszNickName)=NULL;
	//��ע��Ϣ
	virtual void __cdecl SetUserBetScore(BYTE cbArea,SCORE lScore) = NULL;
	//��ע��Ϣ
	virtual void __cdecl SetAllUserBetScore(BYTE cbArea,SCORE lScore) = NULL;
	//���¿���
	virtual void __cdecl UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
};
