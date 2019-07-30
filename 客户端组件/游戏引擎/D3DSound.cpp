#include "Stdafx.h"
#include "D3DSound.h"

CD3DSound *				CD3DSound::m_pD3DSound=NULL;						//����ָ��

CD3DSound::CD3DSound()
{
	//���ö���
	ASSERT(m_pD3DSound==NULL);
	if (m_pD3DSound==NULL) m_pD3DSound=this;
}

CD3DSound::~CD3DSound()
{
	
}

//��������
bool CD3DSound::CreateD3DSound(HWND hWnd)
{
	m_hWnd = hWnd;
	HRESULT Result = DirectSoundCreate(NULL, &m_pDirectSound, NULL);
	if(Result != DS_OK){	return false;}

	Result = m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(Result != DS_OK){ return false;}
	DSBUFFERDESC					dsBufferDesc;             //���������ṹ��������ʼ����������
	memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));			//��սṹ����
	dsBufferDesc.dwSize = sizeof(dsBufferDesc);             //���������ṹ��С
	dsBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsBufferDesc.dwBufferBytes = 0;
	dsBufferDesc.lpwfxFormat = NULL;
	Result = m_pDirectSound->CreateSoundBuffer(&dsBufferDesc, &m_pDirectSoundBuffer, NULL);
	if(Result != DS_OK){	ASSERT(FALSE); return false;}
	return true;
}

//ֹͣ����
bool CD3DSound::StopSound()
{
	POSITION pos = m_SoundArray.GetStartPosition(); 
	while(pos != NULL)   
	{
		LPCTSTR StrChar;LPDIRECTSOUNDBUFFER SoundBuffer;
		m_SoundArray.GetNextAssoc(pos,StrChar,SoundBuffer); 
		if(SoundBuffer!=NULL)SoundBuffer->Stop();
	}
	return true;
}

bool CD3DSound::SetBackVolume(BYTE volume)
{
	return true;
}

bool CD3DSound::SetGameVolume(BYTE volume)
{
	return true;
}

//��������
bool CD3DSound::PlaySound(LPCTSTR pszFileName, bool bRepetition)
{
	CString StrFileName;
	StrFileName.Format(TEXT("%s"),pszFileName);
	//ģ��·��
	TCHAR szModulePath[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModulePath,CountArray(szModulePath));
	//�����ļ�
	for (INT i=lstrlen(szModulePath);i>=0;i--)
	{
		if (szModulePath[i]==TEXT('.'))
		{
			szModulePath[i]=0;
			break;
		}
	}
	LPDIRECTSOUNDBUFFER pSoundBuffer=NULL;
	m_SoundArray.Lookup(pszFileName,pSoundBuffer);
	if(pSoundBuffer == NULL)
	{
		ReadFileBuffer(pSoundBuffer,(LPWSTR)pszFileName);
		m_SoundArray.SetAt(pszFileName,pSoundBuffer);
	}
	if(pSoundBuffer==NULL) return false;
	pSoundBuffer->SetCurrentPosition(0);
	pSoundBuffer->Play(0, 0, bRepetition ? DSBPLAY_LOOPING : 0);

	return true;
}
//��������
bool CD3DSound::PlaySound(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, bool bRepetition)
{
	LPDIRECTSOUNDBUFFER pSoundBuffer=NULL;
	m_SoundArray.Lookup(pszResource,pSoundBuffer);
	if(pSoundBuffer == NULL)
	{
		ReadFileBuffer(pSoundBuffer,hInstance,pszResource,pszTypeName);
		m_SoundArray.SetAt(pszResource,pSoundBuffer);
	}
	if(pSoundBuffer==NULL) return false;
	pSoundBuffer->SetCurrentPosition(0);
	pSoundBuffer->Play(0, 0, bRepetition ? DSBPLAY_LOOPING : 0);

	return true;
}

bool CD3DSound::ReadFileBuffer(LPDIRECTSOUNDBUFFER& SoundBuffer, LPWSTR szFileName)
{
	WAVEFORMATEX	WaveForMatex;    //���������ṹ
	memset(&WaveForMatex,0,sizeof(WaveForMatex));
	LPVOID pAudio;
	DWORD bytesAudio;
	m_Hmmio = mmioOpen(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);  //���ļ�
	if(m_Hmmio == NULL) TRACE(TEXT("�ļ������ڣ�"));
	MMCKINFO ckRiff;   //RIFF�������Ϣ
	MMCKINFO ckInfo;   //���������Ϣ

	//��������
	ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');//�趨�ļ�����
	if(mmioDescend(m_Hmmio, &ckRiff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) TRACE(TEXT("�ļ���ʽ����"));

	//��������
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');//�趨��������
	if(mmioDescend(m_Hmmio, &ckInfo, &ckRiff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		TRACE(TEXT("�ļ���ʽ����"));
		ASSERT(FALSE);
		return false;
	}

	if(mmioRead(m_Hmmio, (HPSTR)&WaveForMatex, sizeof(WaveForMatex)) == -1)
	{
		TRACE(TEXT("��ȡ��ʽʧ�ܣ�"));
		ASSERT(FALSE);
		return false;
	}
	mmioAscend(m_Hmmio, &ckInfo, 0);   //����������
	//��������
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(m_Hmmio, &ckInfo, &ckRiff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		TRACE(TEXT("�ļ���ʽ����"));
		ASSERT(FALSE);
		return false;
	}

	DSBUFFERDESC					dsBufferDesc;             //���������ṹ��������ʼ����������
	memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));					//��սṹ����
	dsBufferDesc.dwSize = sizeof(dsBufferDesc);						//���������ṹ��С
	dsBufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS ;   //???
	dsBufferDesc.dwBufferBytes = ckInfo.cksize;
	dsBufferDesc.lpwfxFormat = &WaveForMatex;

	if(m_pDirectSound->CreateSoundBuffer(&dsBufferDesc, &SoundBuffer, NULL) != DS_OK){	ASSERT(FALSE); return false;}

	if(SoundBuffer->Lock(0, ckInfo.cksize, &pAudio, &bytesAudio, NULL, NULL, NULL) != DS_OK) {	ASSERT(FALSE); return false;}
	//��ȡ�����ļ�����ʧ��
	if(mmioRead(m_Hmmio, (HPSTR)pAudio, bytesAudio) == -1){	ASSERT(FALSE); return false;}
	//�������������ʧ��
	if(SoundBuffer->Unlock(pAudio, bytesAudio, NULL, NULL) != DS_OK){	ASSERT(FALSE); return false;}
	mmioClose(m_Hmmio, 0);
	return true;
}

bool CD3DSound::ReadFileBuffer(LPDIRECTSOUNDBUFFER& SoundBuffer,HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName)
{
	WAVEFORMATEX	*WaveForMatex = NULL;    //���������ṹ

	LPVOID pbWaveData =0;
	DWORD  cbWaveSize =0;
	//������Դ
	HRSRC hResource=FindResource(hInstance,pszResource,pszTypeName);
	if (hResource==NULL) return false;

	//��ȡ��Դ
	DWORD dwImageSize=SizeofResource(hInstance,hResource);
	LPVOID pSoundData=LoadResource(hInstance,hResource);
	if(SoundBuffer==NULL)
	{
		DWORD * pdw = (DWORD *)pSoundData;
		DWORD dwRiff = *pdw++;
		DWORD dwLength = *pdw++;
		DWORD dwType = *pdw++;
		if( dwRiff != mmioFOURCC('R', 'I', 'F', 'F') )	return FALSE;      // not even RIFF

		if( dwType != mmioFOURCC('W', 'A', 'V', 'E') )	return FALSE;      // not a WAV

		DWORD * pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);
		while( pdw < pdwEnd )
		{
			dwType = *pdw++;
			dwLength = *pdw++;
			if(dwType == mmioFOURCC('f', 'm', 't', ' ') && !WaveForMatex)
			{
				if( dwLength < sizeof(WAVEFORMAT) )	return false;      // not a WAV
				WaveForMatex = (WAVEFORMATEX *)pdw;
				if( pbWaveData && cbWaveSize )	break;
			}
			else if(dwType == mmioFOURCC('d', 'a', 't', 'a'))
			{
				pbWaveData = LPVOID(pdw);
				cbWaveSize = dwLength;
				if( WaveForMatex )	break;
			}
			pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
		}

		DSBUFFERDESC BufferDesc;
		memset(&BufferDesc, 0, sizeof(DSBUFFERDESC)); // Zero it out.
		BufferDesc.dwSize = sizeof(DSBUFFERDESC);
		BufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;   //???
		BufferDesc.dwBufferBytes = cbWaveSize;
		BufferDesc.lpwfxFormat = WaveForMatex;    // Create buffer.
		if(m_pDirectSound->CreateSoundBuffer(&BufferDesc, &SoundBuffer, NULL) != DS_OK){	ASSERT(FALSE); return false;}

		LPVOID lpvPtr1;
		DWORD dwBytes1;
		if(SoundBuffer->Lock(0, cbWaveSize, &lpvPtr1, &dwBytes1, NULL, NULL, NULL) != DS_OK) {	ASSERT(FALSE); return false;}
		//��ȡ�����ļ�����ʧ��
		::CopyMemory(lpvPtr1, pbWaveData, dwBytes1);
		//�������������ʧ��
		if(SoundBuffer->Unlock(lpvPtr1, dwBytes1, NULL, NULL) != DS_OK){	ASSERT(FALSE); return false;}
	}
	return true;
}