#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbControlArea = 0xff;
	m_cbControlTimes = 0;
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//����������
bool __cdecl CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
	//Ч������
	//if ( (pIServerUserItem->GetUserRight()&UR_GAME_CONTROL) == 0 )
	if((CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))==false)
		return false;

	ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
	if (wDataSize!=sizeof(CMD_C_ControlApplication)) 
		return false;

	CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

	switch(pControlApplication->cbControlAppType)
	{
	case C_CA_UPDATE:	//����
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//����
		{
			if (  pControlApplication->cbControlArea  > 7 )
			{
				//����ʧ��
				CMD_S_ControlReturns ControlReturns;
				ZeroMemory(&ControlReturns,sizeof(ControlReturns));
				ControlReturns.cbReturnsType = S_CR_FAILURE;
				ControlReturns.cbControlArea = 0xff;
				pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
				return true;
			}
			m_cbControlArea = pControlApplication->cbControlArea;
			m_cbControlTimes = pControlApplication->cbControlTimes;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_CANCELS:	//ȡ��
		{
			m_cbControlArea = 0xff;
			m_cbControlTimes = 0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea = 0xff;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//����ʧ��
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType = S_CR_FAILURE;
	ControlReturns.cbControlArea = 0xff;
	pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//��Ҫ����
bool __cdecl CServerControlItemSink::NeedControl()
{
	if ( m_cbControlArea != 0xff && m_cbControlArea <= 7 && m_cbControlTimes > 0 )
	{
		return true;
	}
	return false;
	
}

//�������
bool __cdecl CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
	return true;
}

//��ɿ���
bool __cdecl CServerControlItemSink::CompleteControl()
{
	m_cbControlTimes--;
	if(m_cbControlTimes == 0)
	{
		m_cbControlArea = 0xff;
	}
	return true;
}

//���ؿ�������
bool __cdecl CServerControlItemSink::ReturnControlArea(tagControlInfo& ControlInfo)
{
	BYTE cbControlArea[8][4] = { 1, 9, 17, 25,   3, 11, 19, 27,   5, 13, 21, 29,   7, 15, 23, 31,   2, 10, 18, 26,   4, 12, 20, 28,   6, 14, 22, 30,   8, 16, 24, 32 };
	ControlInfo.cbControlArea = cbControlArea[m_cbControlArea][rand()%4];
	return true;
}

