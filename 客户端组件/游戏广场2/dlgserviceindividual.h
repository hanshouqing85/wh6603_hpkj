#ifndef SERVICE_INDIVIDUAL_HEAD_FILE
#define SERVICE_INDIVIDUAL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgServiceItem.h"

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
class CDlgServiceIndividual : public CDlgServiceItem, public CMissionItem
{
	//��Ԫ����
	friend class CDlgService;

	//��������
protected:
	bool							m_bQueryMission;						//��ѯ����
	bool							m_bModifyMission;						//�޸�����
	CMissionManager					m_MissionManager;						//�������

	//�ʺ�����
protected:
	BYTE							m_cbGender;								//�û��Ա�
	WORD                            m_wFaceID;                              //ͷ��ID 
	TCHAR							m_szPassword[LEN_PASSWORD];				//�û�����

	//��ϸ����
protected:
	TCHAR							m_szQQ[LEN_QQ];							//Q Q ����
	TCHAR							m_szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							m_szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							m_szSeatPhone[LEN_SEAT_PHONE];			//�̶��绰
	TCHAR							m_szMobilePhone[LEN_MOBILE_PHONE];		//�ƶ��绰
	TCHAR							m_szCompellation[LEN_COMPELLATION];		//��ʵ����
	TCHAR							m_szDwellingPlace[LEN_DWELLING_PLACE];	//��ϵ��ַ

	//�ؼ�����
protected:
	CSkinEdit						m_edAccounts;							//�û��ʺ�
	CSkinEdit						m_edPassword;							//�޸�����

	//�ؼ�����
protected:
	CSkinEdit						m_edQQ;									//Q Q ����
	CSkinEdit						m_edEMail;								//�����ʼ�
	CSkinEdit						m_edUserNote;							//�û�˵��
	CSkinEdit						m_edSeatPhone;							//�̶��绰
	CSkinEdit						m_edMobilePhone;						//�ƶ��绰
	CSkinEdit						m_edCompellation;						//��ʵ����
	CSkinEdit						m_edDwellingPlace;						//��ϵ��ַ

	//�ؼ�����
protected:
	CSkinButton						m_btOk;									//�޸İ�ť

	//�������
protected:
	CFaceItemView					m_FaceItemView;							//ͷ����ͼ

	//��������
public:
	//���캯��
	CDlgServiceIndividual();
	//��������
	virtual ~CDlgServiceIndividual();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���غ���
public:
	//��ȡ����
	virtual LPCTSTR GetServiceItemName() { return TEXT("�û�����"); }

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode,INT nSocketID);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
protected:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

	//���غ���
protected:
	//�ʺ���Ϣ
	VOID LoadAccountsInfo();
	//��ѯ����
	VOID QueryIndividualInfo();
	//��������
	VOID UpdateIndividualInfo();


	//��Ϣӳ��
protected:
	//�������
	VOID  OnBnClickedGenderMankind();
	//���Ů��
	VOID  OnBnClickedGenderFemale();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif