#ifndef NOTE_CARD_HEAD_FILE
#define NOTE_CARD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CNoteCard : public CVirtualWindow
{
	//��������
protected:
	BYTE							m_cbCardData[54];					//�˿�����	

	//�Ҷȶ���
protected:
	BYTE							m_cbAlphaIndex;						//͸������
	CLapseCount						m_AlphaLapseCount;					//���ż���

	//��Դ����
protected:
	CD3DTexture						m_TextureNoteCard;					//������ͼ
	CD3DTexture						m_TextureMiniCard;					//�˿���ͼ

	//��Դ����
protected:
	CSize							m_SizeMiniCard;						//�˿˴�С

	//��������
public:
	//���캯��
	CNoteCard();
	//��������
	virtual ~CNoteCard();

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

	//���ܺ���
public:
	//���ش���
	VOID CloseControl();
	//�����˿�
	VOID SetCardInfo();
	//�����˿�
	VOID SetCardInfo(BYTE cbCardData[], BYTE cbCardCount);
	//ɾ���˿�
	VOID RemoveCard(BYTE cbCardData[], BYTE cbCardCount);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);

	//�ڲ�����
private:
	//ת������
	int SwitchToIndex(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////////////

#endif