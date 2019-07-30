#ifndef DOUBLE_CONTROL_HEAD_FILE
#define DOUBLE_CONTROL_HEAD_FILE

#pragma once
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagDoubleInfo
{
	//����λ��
	WORD							wStartTime;								//��ʼ����
	WORD							wValidCardTime;							//���Ʊ���
	WORD							wRodBankerTime;							//����������
	WORD							wBackCardTime;							//���Ʊ���
	WORD							wBombTime;								//ը������
	WORD							wChunTianbTime;							//���챶��
	WORD							wCardTypeTime;							//���ͱ���
	
	//�ӱ�����
	WORD							wUserDouble[GAME_PLAYER];				//�û��ӱ�

	//�û���Ϣ
	WORD							wBankerUser;							//ׯ���û�
	WORD							wMeChairID;								//����Լ�
};

//////////////////////////////////////////////////////////////////////////////////

//�����ؼ�
class CDoubleControl : public CVirtualWindow
{
	//��������
protected:
	tagDoubleInfo					m_DoubleInfo;							//������Ϣ
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];//�û��ǳ�
	WORD							m_wTotalTime;							//��������
	WORD							m_wAddDoubleTime;						//�ӱ�����

	//�Ҷȶ���
protected:
	BYTE							m_cbAlphaIndex;						//͸������
	CLapseCount						m_AlphaLapseCount;					//���ż���

	//��Դ����
protected:
	CD3DFont						m_Font;								//��������
	CD3DTexture						m_TextureGameDouble;				//������ͼ
	CD3DTexture						m_TextureBaseNumber;				//��������
	CD3DTexture						m_TextureProcessNumber;				//�ӱ�����
	CD3DTexture						m_TexturePlayersNumber;				//�������

	//��������
public:
	//���캯��
	CDoubleControl();
	//��������
	virtual ~CDoubleControl();

	//ϵͳ�¼�
protected:
	//������Ϣ
	virtual VOID OnWindowMovie();
	//������Ϣ
	virtual VOID OnWindowCreate(CD3DDevice * pD3DDevice);
	//������Ϣ
	virtual VOID OnWindowDestory(CD3DDevice * pD3DDevice);

	//���غ���
protected:
	//����¼�
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);
	//��ť�¼�
	virtual VOID OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos);
	//�滭����
	virtual VOID OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);

	//���ܺ���
public:
	//���ش���
	VOID CloseControl();
	//����λ��
	VOID SetDoubleInfo();
	//����λ��
	VOID SetDoubleInfo(tagDoubleInfo &DoubleInfo);

};

//////////////////////////////////////////////////////////////////////////////////

#endif