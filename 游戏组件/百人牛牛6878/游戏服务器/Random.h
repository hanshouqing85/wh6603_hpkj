#ifndef _HP_RANDOM_H_
#define _HP_RANDOM_H_


#include "WinControl.h"

#define _IN_
#define _OUT_


//�Զ����������
//@���ܣ���ȡ�����
//@���ܣ�ϴ���㷨
class WINCONTROL_API	IRandom
{
public:
	/// ȡ[iMin, iMax]�е��������iMin��iMaxһ��Ҫ���㾫ȷ�����û��������Ҫ��
	///	��Ҫ�ٶ�ȡ������ֵ����ȡģ֮��ļ���
	virtual unsigned int Rand(unsigned int iMin=0, unsigned int iMax=32767) = 0;

	/// ϴ���㷨
	virtual void	Shuffle(_IN_ _OUT_ unsigned char* pData, unsigned int nDataLen) = 0;
	virtual void	Shuffle(_IN_ _OUT_ unsigned short* pData, unsigned int nDataLen) = 0;
	virtual void	Shuffle(_IN_ _OUT_ unsigned int* pData, unsigned int nDataLen) = 0;
	static IRandom*		GetSingleton();
};

//ʹ�ð���
/*
	//������������Ϸ������ȥ�� srand()������Ȼ��ԭ��������� rand()����Ϊ IRandom::GetSingleton()->Rand();
	//���Ҳ���Ҫ�ٽ���ȡģ�㷨

	//��ȡ�����
	int nRnd = IRandom::GetSingleton()->Rand(0, 200);

	//ϴ���㷨
	unsigned char szData[54];
	for(BYTE n=0; n<54; n++)
		szData[n] = '0' + n%13;
	IRandom::GetSingleton()->Shuffle(szData, sizeof(szData));

*/


#undef _IN_
#undef _OUT_

#endif // _RANDOM_H_