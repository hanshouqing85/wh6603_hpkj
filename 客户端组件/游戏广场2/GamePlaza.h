#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "PlatformEvent.h"
#include "ServerListData.h"
#include "comm.h"
//�����ļ�
#include "Other\IDispimp.h"
#include "Other\Custsite.h"
#include "..\..\�������\�������\ServiceCoreHead.h"
//////////////////////////////////////////////////////////////////////////////////
#define  WM_BN_CLICK  WM_USER + 100
//Ӧ�ó���
class CGamePlazaApp : public CWinApp
{
	//��������
public:
	CImpIDispatch *					m_pIDispatch;						//�Զ��ӿ�
	CCustomOccManager *				m_pCustomOccManager;				//�������

	//ƽ̨����
protected:
	CGlobalUnits					m_GlobalUnits;						//ȫ�ֵ�Ԫ
	CGlobalUserInfo					m_GlobalUserInfo;					//�û���Ϣ
		CString					m_strCurrentDir;		//��ǰĿ¼
	//ȫ�����
protected:
	CPlatformEvent					m_PlatformEvent;					//ƽ̨�¼�
	CParameterGlobal				m_ParameterGlobal;					//ȫ������
	CUserItemElement				m_UserItemElement;					//�û�Ԫ��
	CUserInformation				m_UserInformation;					//�û���Ϣ

	//��Դ���
public:
	CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	CSkinResourceManager			m_SkinResourceManager;				//��Դ����

	//�������
public:
	CFaceItemControlHelper			m_FaceItemControlModule;			//ͷ�����
	CUserOrderParserHelper			m_UserOrderParserModule;			//�ȼ����

	//��������
public:
	//���캯��
	CGamePlazaApp();
	BOOL CheckVersionUpdate(CString& strUpdateURL);

	//��ʼ��������Ϣ
	void InitCaiZhong();
	bool GetSystemT();
	void GetTime(CTime& time);
	bool	ChangeStringToA(CString strInput, string&  strOutput);
	//���غ���
public:
	//�˳�����
	virtual INT ExitInstance();
	//���뺯��
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�������
extern CGamePlazaApp theApp;

//extern CDBCmd	theDBCmd;
extern CMap<unsigned int, unsigned int, double, double> theBonus;
extern CMap<int, int, CString, CString&> theGameType;
extern CMap<int, int, CString, CString&> theGameKind;
//extern long theTimeDiff;
extern CTime theTime;
extern DWORD theBeginCount;
extern CCriticalSection theTimeCS;
extern BaseUser	theAccount;
extern	 int theCanadaQihao;
extern CTime theCanadaStartTime;
//extern CString theKeFuUrl;
//////////////////////////////////////////////////////////////////////////////////
