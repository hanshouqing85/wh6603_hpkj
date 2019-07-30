#include "StdAfx.h"
#include "Resource.h"
#include "PlatformEvent.h"
#include "PlatformFrame.h"
#include "DlgServiceIndividual.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgServiceIndividual, CDlgServiceItem)
	ON_BN_CLICKED(IDC_GENDER_MANKIND, OnBnClickedGenderMankind)
	ON_BN_CLICKED(IDC_GENDER_FEMALE, OnBnClickedGenderFemale)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgServiceIndividual::CDlgServiceIndividual() : CDlgServiceItem(IDD_DLG_SERVICE_INDIVIDUAL)
{
	//����ͼ��
	m_cbImageIndex=0;

	//�������
	m_bQueryMission=false;
	m_bModifyMission=false;

	//��������
	m_MissionManager.InsertMissionItem(this);

	//�ʺ�����
	m_cbGender=0;
	m_wFaceID=0;
	ZeroMemory(m_szPassword,sizeof(m_szPassword));
	
	//��ϸ����
	ZeroMemory(m_szQQ,sizeof(m_szQQ));
	ZeroMemory(m_szEMail,sizeof(m_szEMail));
	ZeroMemory(m_szUserNote,sizeof(m_szUserNote));
	ZeroMemory(m_szSeatPhone,sizeof(m_szSeatPhone));
	ZeroMemory(m_szMobilePhone,sizeof(m_szMobilePhone));
	ZeroMemory(m_szCompellation,sizeof(m_szCompellation));
	ZeroMemory(m_szDwellingPlace,sizeof(m_szDwellingPlace));

	return;
}

//��������
CDlgServiceIndividual::~CDlgServiceIndividual()
{
}

//�ؼ���
VOID CDlgServiceIndividual::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//�����ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDC_FACE_ITEM, m_FaceItemView);

	//�༭�ؼ�
	DDX_Control(pDX, IDC_ACCOUNTS, m_edAccounts);
	DDX_Control(pDX, IDC_PASSWORD, m_edPassword);

	//�༭�ؼ�
	DDX_Control(pDX, IDC_QQ, m_edQQ);
	DDX_Control(pDX, IDC_EMAIL, m_edEMail);
	DDX_Control(pDX, IDC_USER_NOTE, m_edUserNote);
	DDX_Control(pDX, IDC_SEAT_PHONE, m_edSeatPhone);
	DDX_Control(pDX, IDC_MOBILE_PHONE, m_edMobilePhone);
	DDX_Control(pDX, IDC_COMPELLATION, m_edCompellation);
	DDX_Control(pDX, IDC_DWELLING_PLACE, m_edDwellingPlace);
}

//��������
BOOL CDlgServiceIndividual::OnInitDialog()
{
	__super::OnInitDialog();

	//�ʺ���Ϣ
	m_edAccounts.LimitText(LEN_ACCOUNTS-1);
	m_edPassword.LimitText(LEN_PASSWORD-1);

	//��������
	m_edQQ.LimitText(LEN_QQ-1);
	m_edEMail.LimitText(LEN_EMAIL-1);
	m_edUserNote.LimitText(LEN_USER_NOTE-1);
	m_edSeatPhone.LimitText(LEN_SEAT_PHONE-1);
	m_edMobilePhone.LimitText(LEN_MOBILE_PHONE-1);
	m_edCompellation.LimitText(LEN_COMPELLATION-1);
	m_edDwellingPlace.LimitText(LEN_DWELLING_PLACE-1);

	//������Ϣ
	LoadAccountsInfo();

	return TRUE;
}

//ȷ������
VOID CDlgServiceIndividual::OnOK()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��ȡ����
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	GetDlgItemText(IDC_PASSWORD,szPassword,CountArray(szPassword));

	//�û�����
	GetDlgItemText(IDC_USER_NOTE,m_szUserNote,CountArray(m_szUserNote));
	GetDlgItemText(IDC_COMPELLATION,m_szCompellation,CountArray(m_szCompellation));

	//�绰����
	GetDlgItemText(IDC_SEAT_PHONE,m_szSeatPhone,CountArray(m_szSeatPhone));
	GetDlgItemText(IDC_MOBILE_PHONE,m_szMobilePhone,CountArray(m_szMobilePhone));

	//��ϵ����
	GetDlgItemText(IDC_QQ,m_szQQ,CountArray(m_szQQ));
	GetDlgItemText(IDC_EMAIL,m_szEMail,CountArray(m_szEMail));
	GetDlgItemText(IDC_DWELLING_PLACE,m_szDwellingPlace,CountArray(m_szDwellingPlace));

	//�û��Ա�
	if (((CButton *)GetDlgItem(IDC_GENDER_FEMALE))->GetCheck()==BST_CHECKED) m_cbGender=GENDER_FEMALE;
	if (((CButton *)GetDlgItem(IDC_GENDER_MANKIND))->GetCheck()==BST_CHECKED) m_cbGender=GENDER_MANKIND;

	//��������
	CWHEncrypt::MD5Encrypt(szPassword,m_szPassword);

	//�����ж�
	if (lstrcmp(m_szPassword,pGlobalUserData->szPassword)!=0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�û��ʺ�����������������룡"),MB_ICONERROR,0);

		//���ý���
		m_edPassword.SetFocus();

		return;
	}

	//���ñ���
	m_bQueryMission=false;
	m_bModifyMission=true;

	//�¼�����
	OnMissionStart();

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false) 
	{
		OnMissionConclude();
		return;
	}

	return;
}

//�����¼�
bool CDlgServiceIndividual::OnEventMissionLink(INT nErrorCode,INT nSocketID)
{
	//���ӽ��
	if (nErrorCode!=0)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//�¼�����
		OnMissionConclude();

		//������ʾ
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û����ϻ�ȡʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		//������ʾ
		if (m_bModifyMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("���ӷ�������ʱ���û������޸�ʧ�ܣ�"),MB_OK|MB_ICONSTOP,30);
		}

		return true;
	}

	//�޸�����
	if (m_bModifyMission==true)
	{
		//��������
		BYTE cbBuffer[SOCKET_TCP_BUFFER];
		ZeroMemory(cbBuffer,sizeof(cbBuffer));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//�޸�ͷ��
		if(m_wFaceID!=pGlobalUserData->wFaceID)
		{
			//��������
			CMD_GP_SystemFaceInfo SystemFaceInfo;
			ZeroMemory(&SystemFaceInfo,sizeof(SystemFaceInfo));

			//���ñ���
			SystemFaceInfo.wFaceID=m_wFaceID;
			SystemFaceInfo.dwUserID=pGlobalUserData->dwUserID;
			CWHService::GetMachineID(SystemFaceInfo.szMachineID);
			CopyMemory(SystemFaceInfo.szPassword,pGlobalUserData->szPassword,sizeof(SystemFaceInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_SYSTEM_FACE_INFO,&SystemFaceInfo,sizeof(SystemFaceInfo));
		}


		//��������
		CMD_GP_ModifyIndividual * pModifyIndividual=(CMD_GP_ModifyIndividual *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_ModifyIndividual),sizeof(cbBuffer)-sizeof(CMD_GP_ModifyIndividual));

		//���ñ���
		pModifyIndividual->cbGender=m_cbGender;
		pModifyIndividual->dwUserID=pGlobalUserData->dwUserID;
		lstrcpyn(pModifyIndividual->szPassword,m_szPassword,CountArray(pModifyIndividual->szPassword));

		//�û��ǳ�
		if (pGlobalUserData->szAccounts[0]!=0)
		{
			SendPacket.AddPacket(pGlobalUserData->szAccounts,DTP_GP_UI_NICKNAME);
		}

		//�û���ע
		if (m_szUserNote[0]!=0)
		{
			SendPacket.AddPacket(m_szUserNote,DTP_GP_UI_USER_NOTE);
		}

		//��ʵ����
		if (m_szCompellation[0]!=0) 
		{
			SendPacket.AddPacket(m_szCompellation,DTP_GP_UI_COMPELLATION);
		}

		//�̶�����
		if (m_szSeatPhone[0]!=0) 
		{
			SendPacket.AddPacket(m_szSeatPhone,DTP_GP_UI_SEAT_PHONE);
		}

		//�ֻ�����
		if (m_szMobilePhone[0]!=0)
		{
			SendPacket.AddPacket(m_szMobilePhone,DTP_GP_UI_MOBILE_PHONE);
		}

		//Q Q ����
		if (m_szQQ[0]!=0) 
		{
			SendPacket.AddPacket(m_szQQ,DTP_GP_UI_QQ);
		}

		//�����ʼ�
		if (m_szEMail[0]!=0) 
		{
			SendPacket.AddPacket(m_szEMail,DTP_GP_UI_EMAIL);
		}

		//��ϸ��ַ
		if (m_szDwellingPlace[0]!=0) 
		{
			SendPacket.AddPacket(m_szDwellingPlace,DTP_GP_UI_DWELLING_PLACE);
		}

		//��������
		WORD wSendSize=sizeof(CMD_GP_ModifyIndividual)+SendPacket.GetDataSize();
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_MODIFY_INDIVIDUAL,cbBuffer,wSendSize);
	}

	//��ѯ����
	if (m_bQueryMission==true)
	{
		//��������
		CMD_GP_QueryIndividual QueryIndividual;
		ZeroMemory(&QueryIndividual,sizeof(QueryIndividual));

		//��������
		CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
		tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

		//���ñ���
		QueryIndividual.dwUserID=pGlobalUserData->dwUserID;

		//��������
		m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_QUERY_INDIVIDUAL,&QueryIndividual,sizeof(QueryIndividual));
	}

	return true;
}

//�ر��¼�
bool CDlgServiceIndividual::OnEventMissionShut(BYTE cbShutReason)
{
	//��ʾ��Ϣ
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//��������
		if (m_MissionManager.AvtiveMissionItem(this,true)==true)
		{
			return true;
		}

		//��ѯ����
		if (m_bQueryMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ��û����ϻ�ȡʧ�ܣ�"),MB_ICONERROR,30);
		}

		//�޸�����
		if (m_bModifyMission==true)
		{
			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("��������������쳣�Ͽ��ˣ��û������޸�ʧ�ܣ�"),MB_ICONERROR,30);
		}
	}

	//�¼�����
	OnMissionConclude();

	return true;
}

//��ȡ�¼�
bool CDlgServiceIndividual::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�����
	if (Command.wMainCmdID==MDM_GP_USER_SERVICE)
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GP_OPERATE_FAILURE:		//����ʧ��
			{
				//Ч�����
				CMD_GP_OperateFailure * pOperateFailure=(CMD_GP_OperateFailure *)pData;
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateFailure)-sizeof(pOperateFailure->szDescribeString))) return false;

				//ʧ�ܴ���
				OnMissionConclude();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

// 				//��ʾ��Ϣ
// 				if (pOperateFailure->szDescribeString[0]!=0)
// 				{
// 					CInformation Information(this);
// 					Information.ShowMessageBox(pOperateFailure->szDescribeString,MB_ICONERROR,60);
// 				}

				return true;
			}
		case SUB_GP_OPERATE_SUCCESS:	//�����ɹ�
			{
				//��������
				CMD_GP_OperateSuccess * pOperateSuccess=(CMD_GP_OperateSuccess *)pData;

				//Ч������
				ASSERT(wDataSize>=(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString)));
				if (wDataSize<(sizeof(CMD_GP_OperateSuccess)-sizeof(pOperateSuccess->szDescribeString))) return false;

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);

				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
				tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

				//�ʺ�����
				pGlobalUserData->cbGender=m_cbGender;

				//��ϸ����
				lstrcpyn(pIndividualUserData->szQQ,m_szQQ,CountArray(pIndividualUserData->szQQ));
				lstrcpyn(pIndividualUserData->szEMail,m_szEMail,CountArray(pIndividualUserData->szEMail));
				lstrcpyn(pIndividualUserData->szUserNote,m_szUserNote,CountArray(pIndividualUserData->szUserNote));
				lstrcpyn(pIndividualUserData->szSeatPhone,m_szSeatPhone,CountArray(pIndividualUserData->szSeatPhone));
				lstrcpyn(pIndividualUserData->szMobilePhone,m_szMobilePhone,CountArray(pIndividualUserData->szMobilePhone));
				lstrcpyn(pIndividualUserData->szCompellation,m_szCompellation,CountArray(pIndividualUserData->szCompellation));
				lstrcpyn(pIndividualUserData->szDwellingPlace,m_szDwellingPlace,CountArray(pIndividualUserData->szDwellingPlace));

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);

				//��ʾ��Ϣ
				if (pOperateSuccess->szDescribeString[0]!=0)
				{
					CInformation Information(this);
					Information.ShowMessageBox(pOperateSuccess->szDescribeString,MB_ICONINFORMATION,60);
				}

				//�ɹ�����
				OnMissionConclude();

				return true;
			}
		case SUB_GP_USER_INDIVIDUAL:	//������Ϣ
			{
				//Ч�����
				ASSERT(wDataSize>=sizeof(CMD_GP_UserIndividual));
				if (wDataSize<sizeof(CMD_GP_UserIndividual)) return false;

				//��������
				CMD_GP_UserIndividual * pUserIndividual=(CMD_GP_UserIndividual *)pData;

				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

				//���ñ���
				pIndividualUserData->dwUserID=pUserIndividual->dwUserID;
				
				//��������
				VOID * pDataBuffer=NULL;
				tagDataDescribe DataDescribe;
				CRecvPacketHelper RecvPacket(pUserIndividual+1,wDataSize-sizeof(CMD_GP_UserIndividual));

				//��չ��Ϣ
				while (true)
				{
					pDataBuffer=RecvPacket.GetData(DataDescribe);
					if (DataDescribe.wDataDescribe==DTP_NULL) break;
					switch (DataDescribe.wDataDescribe)
					{
					case DTP_GP_UI_USER_NOTE:		//�û���ע
						{
							lstrcpyn(pIndividualUserData->szUserNote,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szUserNote));
							break;
						}
					case DTP_GP_UI_COMPELLATION:	//��ʵ����
						{
							lstrcpyn(pIndividualUserData->szCompellation,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szCompellation));
							break;
						}
					case DTP_GP_UI_SEAT_PHONE:		//�̶��绰
						{
							lstrcpyn(pIndividualUserData->szSeatPhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szSeatPhone));
							break;
						}
					case DTP_GP_UI_MOBILE_PHONE:	//�ƶ��绰
						{
							lstrcpyn(pIndividualUserData->szMobilePhone,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szMobilePhone));
							break;
						}
					case DTP_GP_UI_QQ:				//Q Q ����
						{
							lstrcpyn(pIndividualUserData->szQQ,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szQQ));
							break;
						}
					case DTP_GP_UI_EMAIL:			//�����ʼ�
						{
							lstrcpyn(pIndividualUserData->szEMail,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szEMail));
							break;
						}
					case DTP_GP_UI_DWELLING_PLACE:	//��ϵ��ַ
						{
							lstrcpyn(pIndividualUserData->szDwellingPlace,(LPCTSTR)pDataBuffer,CountArray(pIndividualUserData->szDwellingPlace));
							break;
						}
					}
				}

				//�ɹ�����
				OnMissionConclude();

				//��������
				UpdateIndividualInfo();

				//�ر�����
				m_MissionManager.ConcludeMissionItem(this,false);


				return true;
			}
		case SUB_GP_USER_FACE_INFO:
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GP_UserFaceInfo));
				if (wDataSize!=sizeof(CMD_GP_UserFaceInfo)) return false;

				//��Ϣ����
				CMD_GP_UserFaceInfo * pUserFaceInfo=(CMD_GP_UserFaceInfo *)pData;

				//��������
				CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

				//���ñ���
				pGlobalUserData->wFaceID=pUserFaceInfo->wFaceID;

				return true;
			}
		}
	}

	return true;
}

//��ʼ����
VOID CDlgServiceIndividual::OnMissionStart()
{
	//��Ϣ�ؼ�
	m_edAccounts.EnableWindow(FALSE);
	m_edPassword.EnableWindow(FALSE);

	//��ϸ����
	m_edQQ.EnableWindow(FALSE);
	m_edEMail.EnableWindow(FALSE);
	m_edUserNote.EnableWindow(FALSE);
	m_edSeatPhone.EnableWindow(FALSE);
	m_edMobilePhone.EnableWindow(FALSE);
	m_edDwellingPlace.EnableWindow(FALSE);

	//���ܰ�ť
	m_btOk.EnableWindow(FALSE);

	return;
}

//��ֹ����
VOID CDlgServiceIndividual::OnMissionConclude()
{
	//��Ϣ�ؼ�
	m_edAccounts.EnableWindow(TRUE);
	m_edPassword.EnableWindow(TRUE);

	//��ϸ����
	m_edQQ.EnableWindow(TRUE);
	m_edEMail.EnableWindow(TRUE);
	m_edUserNote.EnableWindow(TRUE);
	m_edSeatPhone.EnableWindow(TRUE);
	m_edMobilePhone.EnableWindow(TRUE);
	m_edDwellingPlace.EnableWindow(TRUE);

	//���ܰ�ť
	m_btOk.EnableWindow(TRUE);

	//���ÿؼ�
	m_edPassword.SetWindowText(TEXT(""));

	return;
}

//�ʺ���Ϣ
VOID CDlgServiceIndividual::LoadAccountsInfo()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//�û�ͷ��
	//m_wFaceID = pGlobalUserData->wFaceID;
	m_FaceItemView.SetSystemFace(pGlobalUserData->wFaceID);

	//��������
	SetDlgItemText(IDC_ACCOUNTS,pGlobalUserData->szAccounts);

	//�û��Ա�
	if (pGlobalUserData->cbGender==GENDER_FEMALE) ((CButton *)GetDlgItem(IDC_GENDER_FEMALE))->SetCheck(BST_CHECKED);
	if (pGlobalUserData->cbGender==GENDER_MANKIND) ((CButton *)GetDlgItem(IDC_GENDER_MANKIND))->SetCheck(BST_CHECKED);

	//��ϸ����
	if (pIndividualUserData->dwUserID==0L)
	{
		QueryIndividualInfo();
	}
	else
	{
		UpdateIndividualInfo();
	}

	return;
}

//��ѯ����
VOID CDlgServiceIndividual::QueryIndividualInfo()
{
	//���ñ���
	m_bQueryMission=true;
	m_bModifyMission=false;

	//�¼�����
	OnMissionStart();

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		OnMissionConclude();
		return;
	}

	return;
}

//��������
VOID CDlgServiceIndividual::UpdateIndividualInfo()
{
	//��������
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagIndividualUserData * pIndividualUserData=pGlobalUserInfo->GetIndividualUserData();

	//�û�����
	SetDlgItemText(IDC_USER_NOTE,pIndividualUserData->szUserNote);
	SetDlgItemText(IDC_COMPELLATION,pIndividualUserData->szCompellation);

	//�绰����
	SetDlgItemText(IDC_SEAT_PHONE,pIndividualUserData->szSeatPhone);
	SetDlgItemText(IDC_MOBILE_PHONE,pIndividualUserData->szMobilePhone);

	//��ϵ����
	SetDlgItemText(IDC_QQ,pIndividualUserData->szQQ);
	SetDlgItemText(IDC_EMAIL,pIndividualUserData->szEMail);
	SetDlgItemText(IDC_DWELLING_PLACE,pIndividualUserData->szDwellingPlace);

	return;
}

//�������
VOID  CDlgServiceIndividual::OnBnClickedGenderMankind()
{
	m_wFaceID=0;
	m_FaceItemView.SetSystemFace(m_wFaceID);

	return;
}

//���Ů��
VOID  CDlgServiceIndividual::OnBnClickedGenderFemale()
{
	m_wFaceID=1;
	m_FaceItemView.SetSystemFace(m_wFaceID);

	return;
}


//////////////////////////////////////////////////////////////////////////////////
