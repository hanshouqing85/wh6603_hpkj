#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void __cdecl OnAllowControl(bool bEnable) = NULL;
	//������
	virtual bool __cdecl ReqResult(const void * pBuffer) = NULL;
	virtual void __cdecl ResetUserBet() = NULL;

	virtual void __cdecl SetUserBetScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserGameScore(BYTE cbArea,LONGLONG lScore) = NULL;
	virtual void __cdecl SetUserNickName(CString strNickName) = NULL;
	virtual void __cdecl ResetUserNickName() = NULL;
};



//
//�¼�����Ϸ����6602ת6603��ʹԭ��6602����Ϸ������6603��ʹ��
//���ߣ����ٽ�
//ʱ�䣺2012.03.04 
//Q Q ��349126394
//QQȺ��101494119
//���䣺shaojiang216@163.com
//���ͣ�http://shaojiang216.blog.163.com/ 