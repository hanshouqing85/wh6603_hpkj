#ifndef CONFIG_H
#define CONFIG_H
#include "..\��Ϣ����\CMD_OxBattle.h"

/*
	�������ļ����Ʒ��̶��ƣ����ڲ���
*/
class BRNNConfig
{
public:
	BRNNConfig(const char *xmlFileName=0);
	static BRNNConfig & Instance();
	int LoadData(const char *xmlFileName);

public:
	bool m_HasLoadOK;//�Ƿ���سɹ���

	/*
		<!--���̶��ƣ����ڲ���-->
		<FixCard Enable="1" CenterCard="0x31,0x22,0x01,0x11,0x21" HandCard0="0x03,0x13" HandCard1="0x04,0x14" HandCard2="0x05,0x15" HandCard3="0x06,0x16" HandCard4="0x07,0x17" HandCard5="0x08,0x18" HandCard6="0x09,0x19" HandCard7="0x0A,0x1A"></FixCard>
	*/
	int m_Enable;
	int m_Refresh;
	BYTE m_cbCardData[5][5];
};

#endif

