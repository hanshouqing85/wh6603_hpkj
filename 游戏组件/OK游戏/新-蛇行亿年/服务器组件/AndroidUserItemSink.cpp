#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//����ʱ��
#define TIME_LESS					1									//����ʱ��
#define TIME_DISPATCH				5									//����ʱ��

//��Ϸʱ��
#define TIME_OUT_CARD				3									//����ʱ��
#define TIME_START_GAME				8									//��ʼʱ��
#define TIME_CALL_SCORE				5									//�з�ʱ��

//��Ϸʱ��
#define IDI_SHOOT			     	(0)			//�����ӵ�
#define IDI_ADDMONEY				(1)			//�Ϸ�
#define IDI_CALL_SCORE				(2)			//�з�ʱ��
#define IDI_CHANGE_SCORE				(3)			//�л�����


//////////////////////////////////////////////////////////////////////////

//���캯��
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//�Լ�����
	m_BeiLv = 0;
	//�Լ�����
	m_AllScore = 0;
	//
	m_IsShoot = false;

	//
	m_HaveBuy = false;
	//�ӿڱ���
	m_pIAndroidUserItem=NULL;;

	return;
}

//��������
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//�ӿڲ�ѯ
VOID * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ�ӿ�
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//���ýӿ�
bool CAndroidUserItemSink::RepositionSink()
{
	//��Ϸ����

	return true;
}

//ʱ����Ϣ
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	 case IDI_SHOOT:	//�����ӵ�
		{
			//m_HaveBuy = false;
			SendButtle();
			return true;
		}
		//�Ϸ�
	 case  IDI_ADDMONEY:
		 {
		     m_HaveBuy = true;
 	         CMD_C_BuyBullet  AddBuyscore;
	         AddBuyscore.addormove = true;
	         AddBuyscore.score = m_BeiLv;
			 CString strLog;
			 strLog.Format(L"SXYN ADD m_BeiLv:%d",m_BeiLv);
			 OutputDebugString(strLog);

	         m_pIAndroidUserItem->SendSocketData(SUB_C_BUY_BULLET,&AddBuyscore,sizeof(AddBuyscore));
			 return true;
		 }
	 case IDI_CHANGE_SCORE:
		 {
		    m_HaveBuy = false; 
		    CMD_C_SetProbability UserBeilv;
	        UserBeilv.byCptrProbability = false;
			m_pIAndroidUserItem->SendSocketData(SUB_C_SET_PROPABILITY,&UserBeilv,sizeof(UserBeilv));
			if(UserBeilv.byCptrProbability == false)
			{
			    m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,rand()%3+2);
				return true;
			}
			if(rand()%5==0)
			{
			     //��ʼ���ӵ�
	             m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,rand()%3+2);
			}
			else 
			{
			    m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,2);
			}


		 }
	}
	return false;
}

//�����ӵ�
void CAndroidUserItemSink::SendButtle()
{
            CMD_C_UserShoot UserShoot;
			UserShoot.fAngle=rand()%80;
            if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==0)
               UserShoot.fAngle=rand()%80;
			else if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==1)
			{ 
				int a=rand()%2;
				if(a==0)UserShoot.fAngle=-rand()%80;
				else UserShoot.fAngle=rand()%80;
			}
			else if(SwitchChairID(m_pIAndroidUserItem->GetChairID())==2)
               UserShoot.fAngle=-rand()%80;

			UserShoot.dwBulletID=GetTickCount();
			m_pIAndroidUserItem->SendSocketData(SUB_C_USER_SHOOT,&UserShoot,sizeof(UserShoot));
}
//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{

	switch (wSubCmdID)
	{
	  case SUB_S_BULLET_COUNT:	//�����ӵ�
		{

			srand((unsigned)time(NULL));
			CMD_S_BulletCount * addfen = (CMD_S_BulletCount *)pData;
			if(addfen->isaddorremove)
			{
				if(addfen->wChairID == m_pIAndroidUserItem->GetChairID())
				{
					//�Լ�����
					m_AllScore = addfen->score;
					//�����ӵ�
				    
					if(rand()%3==0)
					{
					  if(!m_IsShoot)
					   {

						  m_IsShoot = true;
						  m_pIAndroidUserItem->SetGameTimer(IDI_SHOOT,rand()%4+2);
					   }
					}
					else
					{
						m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,2);
						
						
					   
					}
				}
			    
			}
			else
			{
				
			}
			return true;
		}
	  case SUB_S_ANDROID_CELL:
		  {
			//�Ϸ��ж�
	        ASSERT(wDataSize==sizeof(CMD_S_AndroidCellScore));
	        if (!(wDataSize==sizeof(CMD_S_AndroidCellScore))) return false;
			CMD_S_AndroidCellScore *pCellScore=(CMD_S_AndroidCellScore *)pData;
			m_BeiLv = pCellScore->score;

			//��������ñ���
		//	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,rand()%8+6);

			return true;
	
		  }
	case SUB_S_USER_SHOOT:				//�����ڵ�
		{
			//�Ϸ��ж�
	        ASSERT(wDataSize==sizeof(CMD_S_UserShoot));
	        if (!(wDataSize==sizeof(CMD_S_UserShoot))) return false;
	        //����ת��
	        CMD_S_UserShoot *pUserShoot=(CMD_S_UserShoot *)pData;
			if(pUserShoot->dwUserScore<m_BeiLv&&m_HaveBuy)
			{
				
				m_HaveBuy= false;
			    m_pIAndroidUserItem->SetGameTimer(IDI_ADDMONEY,2);
			}
			else if(pUserShoot->dwUserScore>=m_BeiLv)
			{
 	       //�����ڵ�
			if(pUserShoot->wChairID==m_pIAndroidUserItem->GetChairID())
			{

				m_pIAndroidUserItem->KillGameTimer(IDI_SHOOT);
				if(m_AllScore>=m_BeiLv)
				{
					SendButtle();
				}
				else
				{   

					//�˳���Ϸ
					m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_FREE);
				    m_pIAndroidUserItem->SendSocketData(216);
					
				}
			}
			}
			   
			return true;
		}
	}


	return true;
}

//��Ϸ��Ϣ
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//������Ϣ
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	return true;
	//����״̬
	//m_pIAndroidUserItem->SendUserReady(NULL,0);
	m_pIAndroidUserItem->SetGameStatus(GAME_SCENE_PLAY);
	CMD_S_GameScene * pGameScene1=(CMD_S_GameScene *)pData;
	//���ñ���
	//m_BeiLv = pGameScene1->lBulletCharge;

	CString strLog;
	strLog.Format(L"SXYN m_BeiLv:%d",m_BeiLv);
	OutputDebugString(strLog);
	//��������ñ���
//	m_pIAndroidUserItem->SetGameTimer(IDI_CHANGE_SCORE,rand()%8+6);

	return true;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û��뿪
VOID CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�����
VOID CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û�״̬
VOID CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//�û���λ
VOID CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//��Ϸ��ʼ
bool CAndroidUserItemSink::OnSubGameStart(VOID * pData, WORD wDataSize)
{
  //  AfxMessageBox(L"a");	

	return true;
}

//�û��з�
bool CAndroidUserItemSink::OnSubCallScore(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//ׯ����Ϣ
bool CAndroidUserItemSink::OnSubBankerInfo(VOID * pData, WORD wDataSize)
{
	
	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubOutCard(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//�û�����
bool CAndroidUserItemSink::OnSubPassCard(VOID * pData, WORD wDataSize)
{
	

	return true;
}

//��Ϸ����
bool CAndroidUserItemSink::OnSubGameEnd(VOID * pData, WORD wDataSize)
{
	
	return true;
}
int CAndroidUserItemSink::SwitchChairID(int m_Currchairid)
{
    int changChairID = m_Currchairid;
	switch(m_pIAndroidUserItem->GetChairID())
	{
	   case 0:
	   case 1:
	   case 2:
	   {
	         switch(m_Currchairid)
			 {
			    case 0:return 0;
				case 1:return 1;
				case 2:return 2;
				case 3:return 3;
				case 4:return 4;
				case 5:return 5;
			 }
	   
	   }
	   case 3:
	   case 4:
	   case 5:
	   {
	         switch(m_Currchairid)
			 {
			    case 0:return 3;
				case 1:return 4;
				case 2:return 5;
				case 3:return 0;
				case 4:return 1;
				case 5:return 2;
			 }
	   
	   }
	}
	return changChairID;
}



//////////////////////////////////////////////////////////////////////////
