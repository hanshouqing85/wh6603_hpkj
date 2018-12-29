#include "StdAfx.h"
#include ".\bassmusicmanager.h"

//����ģʽ��ָ��
CBassMusicManager* CBassMusicManager::_instance = NULL;

std::map<int, CBassMusicManager::tagMusicStream> CBassMusicManager::_mapPlayingStream;
std::map<int, CBassMusicManager::tagMusicStream> CBassMusicManager::_mapNeedDeleteStream;

CBassMusicManager::CBassMusicManager(void)
{
}

CBassMusicManager::~CBassMusicManager(void)
{
}

//��ȡ�����������ָ��
CBassMusicManager* CBassMusicManager::GetInstance()
{
	if(_instance == NULL)
		_instance = new CBassMusicManager();

	return _instance;		
}

//�ͷ���Դ
void CBassMusicManager::ReleaseAll()
{
	if(_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
}

//���ô��ھ��
bool CBassMusicManager::Init(HWND hWnd)
{
	if(_instance == NULL)
		return false;

	this->_hWnd=hWnd;
	return (BASS_Init(-1, 44100, 0, _hWnd, NULL) == TRUE); 
}

//���ļ��м�����Դ
bool CBassMusicManager::LoadFromFile(const int nUid, const LPCSTR lpszFileName, const bool bLoop)
{
	if(_instance == NULL)
		return false;

	HSTREAM hStream;

	if(bLoop)
		hStream = BASS_StreamCreateFile(FALSE, (char*)lpszFileName, 0, 0, BASS_SAMPLE_LOOP|BASS_SAMPLE_FX);
	else
		hStream = BASS_StreamCreateFile(FALSE, (char*)lpszFileName, 0, 0, 0);

	if(hStream == 0)
		return false;
 	assert(hStream != 0);

	tagMusicStream tagMs;
	tagMs.hStream = hStream;
	tagMs.bLoop = bLoop;

	_mapPlayingStream.insert(std::pair<int, tagMusicStream>(nUid, tagMs));
	
	return true;
}

//�����ƶ���־����
bool CBassMusicManager::Play(int nUid, bool bDeleteWhenStop)
{
	if(IsPlaying(nUid))RePlay(nUid, bDeleteWhenStop);
	return _Play(nUid, false, bDeleteWhenStop);
}

//��ͷ���²�������
bool CBassMusicManager::RePlay(int nUid, bool bDeleteWhenStop)
{
	return _Play(nUid, true, bDeleteWhenStop);
}

//��������
bool CBassMusicManager::_Play(int nUid, bool bReplay, bool bDeleteWhenStop)
{
	if(_instance == NULL)
		return false;

	std::map<int, tagMusicStream>::iterator iter = _mapPlayingStream.find(nUid);

	if(iter == _mapPlayingStream.end())
		return false;

	HSTREAM hStream = iter->second.hStream;
	bool bLoop = iter->second.bLoop;

	tagMusicStream tagMs;
	tagMs.bLoop = bLoop;
	tagMs.hStream = hStream;
	
	//��ʼ����
	BASS_ChannelPlay(hStream, bReplay==TRUE);

	//���ûص�����
	//BASS_ChannelSetSync(hStream, BASS_SYNC_END, (QWORD)MAKELONG(10,0), &CBassMusicManager::MySyncProc, 0);

	//���Ҫ������ɾ����Դ������ӵ���Ҫɾ����������,ͬʱ�ӵ�ǰ����������ɾ��
	if(bDeleteWhenStop)
	{
		_mapNeedDeleteStream.insert(std::pair<int, tagMusicStream>(nUid, tagMs));
		_mapPlayingStream.erase(iter);
	}

	return true;
}

//��ͣ
bool CBassMusicManager::Pause(int nUid)
{
	if(_instance == NULL)
		return false;

	std::map<int, tagMusicStream>::iterator iter = _mapNeedDeleteStream.find(nUid);
	if(iter != _mapNeedDeleteStream.end())
		return ( BASS_ChannelPause(iter->second.hStream) == TRUE);
	else
	{
		iter = _mapPlayingStream.find(nUid);
		if(iter != _mapPlayingStream.end())
			return ( BASS_ChannelPause(iter->second.hStream) == TRUE);
	}

	return false;
	
}

//ֹͣ
bool CBassMusicManager::Stop(int nUid)
{
	if(_instance == NULL)
		return false;

	std::map<int, tagMusicStream>::iterator iter = _mapNeedDeleteStream.find(nUid);
	if(iter != _mapNeedDeleteStream.end())
		return ( BASS_ChannelStop(iter->second.hStream) == TRUE);
	else
	{
		iter = _mapPlayingStream.find(nUid);
		if(iter != _mapPlayingStream.end())
			return ( BASS_ChannelStop(iter->second.hStream) == TRUE);
	}

	return false;
}

//�Ƿ��ڲ���
bool CBassMusicManager::IsPlaying(int nUid)
{
	if(_instance == NULL)
		return false;

	std::map<int, tagMusicStream>::iterator iter;
	iter = _mapPlayingStream.find(nUid);
	if(iter != _mapPlayingStream.end())
	{
		return (BASS_ChannelIsActive(iter->second.hStream)==BASS_ACTIVE_PLAYING);
	}
	else
	{
		iter = _mapNeedDeleteStream.find(nUid);
		if(iter != _mapNeedDeleteStream.end())
			return (BASS_ChannelIsActive(iter->second.hStream)==BASS_ACTIVE_PLAYING);
		else
			return false;
	}

	return false;
}

//����������С
bool CBassMusicManager::SetVolumn(int nVolumn)
{
	if(nVolumn<0 || nVolumn>100) return false;

	return (BASS_SetConfig(BASS_CONFIG_GVOL_STREAM,nVolumn)==TRUE);
}

//�ص�����
void CALLBACK CBassMusicManager::MySyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	std::map<int, tagMusicStream>::iterator iter = _mapNeedDeleteStream.begin();
	for(; iter!=_mapNeedDeleteStream.end();)
	{
		if( BASS_ChannelIsActive(iter->second.hStream) == BASS_ACTIVE_STOPPED )
		{	
			if(BASS_StreamFree(iter->second.hStream)==TRUE)
				_mapNeedDeleteStream.erase(iter++);
			else
				++iter;
		}
		else
			++iter;
	}
}