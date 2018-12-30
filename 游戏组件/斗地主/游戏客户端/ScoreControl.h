#ifndef SCORE_CONTROL_HEAD_FILE
#define SCORE_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagScoreInfo
{
	////ը����Ϣ
	//BYTE							cbBombCount;							//ը������
	//BYTE							cbEachBombCount[GAME_PLAYER];			//ը������

	////��Ϸ�ɼ�
	//SCORE							lCellScore;								//��Ԫ����
	//SCORE							lGameScore[GAME_PLAYER];				//��Ϸ�ɼ�
	//SCORE							lCollectScore[GAME_PLAYER];				//���ܳɼ�

	////�����Ϣ
	//WORD							wMeChairID;								//�Լ�λ��
	//WORD							wBankerUser;							//ׯ���û�
	//BYTE							cbBankerScore;							//�з���Ŀ

	////�����־
	//BYTE							bChunTian;							//�����־
	//BYTE							bFanChunTian;						//�����־

	////�û���Ϣ
	//WORD							wFaceID[GAME_PLAYER];					//ͷ���ʶ
	//DWORD							dwUserID[GAME_PLAYER];					//�û���ʶ
	//TCHAR							szNickName[GAME_PLAYER][LEN_NICKNAME];	//�û��ǳ�

	//ը����Ϣ
	BYTE							cbBombCount;							//ը������
	BYTE							cbEachBombCount[GAME_PLAYER];			//ը������

	//������Ϣ
	WORD							wStartTime;								//��ʼ����
	//WORD							wValidCardTime;							//���Ʊ���
	WORD							wRodBankerTime;							//����������
	//WORD							wBackCardTime;							//���Ʊ���
	WORD							wBombTime;								//ը������
	WORD							wChunTianbTime;							//���챶��
	WORD							wCardTypeTime;							//���ͱ���

	WORD							wTotalTime;								//�ܱ���

	//��Ϸ�ɼ�
	SCORE							lCellScore;								//��Ԫ����
	SCORE							lGameScore[GAME_PLAYER];				//��Ϸ�ɼ�
	SCORE							lCollectScore[GAME_PLAYER];				//���ܳɼ�

	//�����Ϣ
	WORD							wMeChairID;								//�Լ�λ��
	WORD							wBankerUser;							//ׯ���û�

	//�����־
	BYTE							bChunTian;								//�����־
	BYTE							bFanChunTian;							//�����־

	//�û���Ϣ
	WORD							wFaceID[GAME_PLAYER];					//ͷ���ʶ
	DWORD							dwUserID[GAME_PLAYER];					//�û���ʶ
	TCHAR							szNickName[GAME_PLAYER][LEN_NICKNAME];	//�û��ǳ�

	//CGameClientEngine*				pGameClientEngine;

};

//////////////////////////////////////////////////////////////////////////////////

//���ֿؼ�
class CScoreControl : public CVirtualWindow
{
	//��������
protected:
	tagScoreInfo					m_ScoreInfo;						//������Ϣ

	//CD3DDevice * m_pD3DDevice;  //����

	//�Ҷȶ���
protected:
	BYTE							m_cbAlphaIndex;						//͸������
	CLapseCount						m_AlphaLapseCount;					//���ż���

	//�ɼ�����
protected:
	BYTE							m_cbCartoonIndex;					//��������
	CLapseCount						m_CartoonLapseCount;				//���ż���

	//�ؼ�����
protected:
	CVirtualButton					m_btClose;							//�رհ�ť

	//��Դ����
protected:
	CD3DFont						m_FontScore;						//��������
	CD3DTexture						m_TextureGameScore;					//������ͼ
	CD3DTexture						m_TextureBombNumber;				//ը������
	CD3DTexture						m_TextureWinLoseFlag;				//��Ӯ��־
	CD3DTexture						m_TextureScoreCartoon;				//�ɼ�����
	CD3DTexture						m_TextureChunTian;					//����


	//��������
public:
	//���캯��
	CScoreControl();
	//��������
	virtual ~CScoreControl();

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
	virtual BOOL OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);
	//��ť�¼�
	virtual BOOL OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos);
	//�����¼�
	virtual BOOL OnEventKeyboard(UINT uMessage, WPARAM wParam, LPARAM lParam);
	//�滭����
	virtual VOID OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos);

	//���ܺ���
public:
	//���ش���
	VOID CloseControl();
	//��ȡdraw
	//CD3DDevice* getDevice(){return m_pD3DDevice;}
	//���û���
	VOID SetScoreInfo(tagScoreInfo & ScoreInfo);
	// �滭����
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);
	
};

//////////////////////////////////////////////////////////////////////////////////

#endif