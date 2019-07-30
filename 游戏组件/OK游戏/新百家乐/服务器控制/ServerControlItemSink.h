#pragma once
#include "../��Ϸ������/ServerControl.h"


class CServerControlItemSink : public IServerControl
{
	//���Ʊ���
protected:
	BYTE							m_cbWinSideControl;					//������Ӯ
	BYTE							m_cbExcuteTimes;					//ִ�д���
	int								m_nSendCardCount;					//���ʹ���

public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//����������
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//��Ҫ����
	virtual bool  NeedControl();

	//���ؿ�������
	virtual bool  ControlResult(BYTE	cbTableCardArray[], BYTE cbCardCount[]);

};
