#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//����������
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;

	//��Ҫ����
	virtual bool  NeedControl() = NULL;

	//���ؿ�������
	virtual bool  ControlResult(BYTE	cbTableCardArray[], BYTE cbCardCount[]) = NULL;
};
