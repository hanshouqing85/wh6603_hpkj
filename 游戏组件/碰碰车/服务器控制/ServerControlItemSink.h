#pragma once
#include "../��Ϸ������/ServerControl.h"


class CServerControlItemSink : public IServerControl
{
	//���Ʋ���
protected:
	BYTE							m_cbControlArea;						//��������
	BYTE							m_cbControlTimes;						//���ƴ���


public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);


public:
	//����������
	virtual bool __cdecl ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame);

	//��Ҫ����
	virtual bool __cdecl NeedControl();

	//�������
	virtual bool __cdecl MeetControl(tagControlInfo ControlInfo);

	//��ɿ���
	virtual bool __cdecl CompleteControl();

	//���ؿ�������
	virtual bool __cdecl ReturnControlArea(tagControlInfo& ControlInfo);


};
