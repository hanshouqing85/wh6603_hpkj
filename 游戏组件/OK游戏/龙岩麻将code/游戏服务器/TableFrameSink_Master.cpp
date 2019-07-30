#include "StdAfx.h"
#include "TableFrameSink.h"


void CTableFrameSink::sendMasterHandCard()
{
	MasterHandCard kMasterHandCard;

	for (int i=0;i<GAME_PLAYER;i++)
	{
		MasterHandCardInfo kCardInfo;
		kCardInfo.nChairId = i;
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],kCardInfo.kMasterHandCard);

		kMasterHandCard.kMasterHandCardList.push_back(kCardInfo);
	}
	datastream kDataStream;
	kMasterHandCard.StreamValue(kDataStream,true);
	m_pITableFrame->SendTableData( INVALID_CHAIR,SUB_S_MASTER_HANDCARD,&kDataStream[0],kDataStream.size());
}


void CTableFrameSink::sendMasterLeftCard(int nChairdID)
{
	std::vector<BYTE> LeftCardData;
	
	for (int i=0;i<m_cbLeftCardCount;i++)
	{
		LeftCardData.push_back(m_cbRepertoryCard[i]);
	}

	MasterLeftCard kMasterLeftCard;
	ZeroMemory(&kMasterLeftCard,sizeof(kMasterLeftCard));
	m_GameLogic.SwitchToCardIndex(LeftCardData,kMasterLeftCard.kMasterLeftIndex);
	kMasterLeftCard.kMasterCheakCard = m_cbMasterCheckCard[nChairdID];

	m_pITableFrame->SendTableData( nChairdID,SUB_S_MASTER_LEFTCARD,&kMasterLeftCard,sizeof(kMasterLeftCard));
}
BYTE CTableFrameSink::getSendCardData(WORD dwSendUser)
{
#define  swapCard(a,b)\
	BYTE nTemp = a;\
	a = b;\
	b = nTemp;\

	BYTE MasterCard = m_cbMasterCheckCard[dwSendUser];
	if (MasterCard == 0)
	{
		return m_cbRepertoryCard[--m_cbLeftCardCount];
	}
	
	bool bValue = false;
	for (int i=m_cbLeftCardCount-1;i>=0;i--)
	{
		if (m_cbRepertoryCard[i] == MasterCard)
		{
			bValue = true;
			swapCard(m_cbRepertoryCard[i],m_cbRepertoryCard[m_cbLeftCardCount-1]);
			break;
		}
	}
	m_cbMasterCheckCard[dwSendUser]= 0;

	BYTE nCard = m_cbRepertoryCard[--m_cbLeftCardCount];

	if (!bValue)
	{
		m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(dwSendUser),_T("��ѡ������Ѿ������ڣ�"),SMT_EJECT|SMT_CHAT);
	}

	return nCard;
}