#pragma once

#pragma comment(lib, "bass/bass.lib")

#include "bass/bass.h"
#include <map>
#include <assert.h>

///������Դ�ṹ��
typedef struct{
	///HSTREAM
	HSTREAM hStream;
	///�Ƿ�ѭ������, ע�⣺�˲���������bDeleteWhenStopͬʱΪtrue
	bool bLoop;
} tagMusicStream;


/**
* @brief ������Դ������
* @detail �����Ե���ģʽʵ�ֵ�,���Թ������������������Դ�Ĳ��š���ͣ�Ȳ���
* @author Ettan
*/
class CBassMusicManager
{
public:
	///��������
	~CBassMusicManager(void);
	//
    CBassMusicManager(void);
	/**
	* @brief ��ȡ�����������ָ��
	*/
	static CBassMusicManager* GetInstance();

	///���ô��ھ��
	bool Init(HWND hWnd);

	///�ͷ���Դ
	void ReleaseAll();
	
	/**
	* @brief ���ļ��м�����Դ
	* @param nUid	��Դ��־ID�����ݴ˱�־�����Ժ���Ӧ�Ĳ������粥�ţ���ͣ��
	* @param lpszFileName	�ļ�·��
	* @param bLoop	�Ƿ�ѭ����Ĭ�ϣ�false
	* @param bDeleteWhenStop	���Ž������Ƿ�����ɾ����Դ��Ĭ��true
	*/
	bool LoadFromFile(const int nUid, const LPCSTR lpszFileName, const bool bLoop=false);

	/**
	* @brief ����ָ����־������
	* @param nUid ������־ID
	* @param bDeleteWhenStop ������ɺ��Ƿ�ɾ����Դ��Ĭ��false
	*/
	bool Play(int nUid, bool bDeleteWhenStop=true);

	/**
	* @brief ��ͷ���²���ָ����־������
	* @param nUid ������־ID
	* @param bDeleteWhenStop ������ɺ��Ƿ�ɾ����Դ��Ĭ��false
	*/
	bool RePlay(int nUid, bool bDeleteWhenStop=true);

	///��ͣ
	bool Pause(int nUid);

	///ֹͣ
	bool Stop(int nUid);

	///�Ƿ��ڲ���
	bool IsPlaying(int nUid);

	/**
	* @brief ����������С
	* @param nVolumn ������С��ȡֵ��Χ0~100
	*/
	bool SetVolumn(int nVolumn);

protected:
	///�ص�����
	static void CALLBACK MySyncProc(HSYNC handle, DWORD channel, DWORD data, void *user);

	bool _Play(int nUid, bool bReplay=false, bool bDeleteWhenStop=true);

protected:
	///���캯��
	

private:
	///HSTREAM����
	static std::map<int, tagMusicStream> _mapPlayingStream;	
	///��Ҫ�ͷ�HSTREAM��Դ������
	static std::map<int, tagMusicStream> _mapNeedDeleteStream;
	///��ָ��
	static CBassMusicManager* _instance;
	///����ָ��
	HWND _hWnd;
};
