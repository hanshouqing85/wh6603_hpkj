#include "Stdafx.h"
#include "D3DSound.h"

CD3DSound *				CD3DSound::m_pD3DSound=NULL;						//对象指针

CD3DSound::CD3DSound()
{
	//设置对象
	ASSERT(m_pD3DSound==NULL);
	if (m_pD3DSound==NULL) m_pD3DSound=this;
}

CD3DSound::~CD3DSound()
{
	
}

//创建环境
bool CD3DSound::CreateD3DSound(HWND hWnd)
{
	m_hWnd = hWnd;
	HRESULT Result = DirectSoundCreate(NULL, &m_pDirectSound, NULL);
	if(Result != DS_OK){	return false;}

	Result = m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if(Result != DS_OK){ return false;}
	DSBUFFERDESC					dsBufferDesc;             //声明描述结构，用来初始化缓冲区域
	memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));			//清空结构内容
	dsBufferDesc.dwSize = sizeof(dsBufferDesc);             //配制描述结构大小
	dsBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsBufferDesc.dwBufferBytes = 0;
	dsBufferDesc.lpwfxFormat = NULL;
	Result = m_pDirectSound->CreateSoundBuffer(&dsBufferDesc, &m_pDirectSoundBuffer, NULL);
	if(Result != DS_OK){	ASSERT(FALSE); return false;}
	return true;
}

//停止声音
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

//播放声音
bool CD3DSound::PlaySound(LPCTSTR pszFileName, bool bRepetition)
{
	CString StrFileName;
	StrFileName.Format(TEXT("%s"),pszFileName);
	//模块路径
	TCHAR szModulePath[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModulePath,CountArray(szModulePath));
	//分析文件
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
//播放声音
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
	WAVEFORMATEX	WaveForMatex;    //声明声音结构
	memset(&WaveForMatex,0,sizeof(WaveForMatex));
	LPVOID pAudio;
	DWORD bytesAudio;
	m_Hmmio = mmioOpen(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);  //打开文件
	if(m_Hmmio == NULL) TRACE(TEXT("文件不存在！"));
	MMCKINFO ckRiff;   //RIFF区块的信息
	MMCKINFO ckInfo;   //子区块的信息

	//搜索类型
	ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');//设定文件类型
	if(mmioDescend(m_Hmmio, &ckRiff, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) TRACE(TEXT("文件格式错误！"));

	//搜索区块
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');//设定区块类型
	if(mmioDescend(m_Hmmio, &ckInfo, &ckRiff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		TRACE(TEXT("文件格式错误！"));
		ASSERT(FALSE);
		return false;
	}

	if(mmioRead(m_Hmmio, (HPSTR)&WaveForMatex, sizeof(WaveForMatex)) == -1)
	{
		TRACE(TEXT("读取格式失败！"));
		ASSERT(FALSE);
		return false;
	}
	mmioAscend(m_Hmmio, &ckInfo, 0);   //跳出子区块
	//搜索区块
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(m_Hmmio, &ckInfo, &ckRiff, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		TRACE(TEXT("文件格式错误！"));
		ASSERT(FALSE);
		return false;
	}

	DSBUFFERDESC					dsBufferDesc;             //声明描述结构，用来初始化缓冲区域
	memset(&dsBufferDesc, 0, sizeof(dsBufferDesc));					//清空结构内容
	dsBufferDesc.dwSize = sizeof(dsBufferDesc);						//配制描述结构大小
	dsBufferDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS ;   //???
	dsBufferDesc.dwBufferBytes = ckInfo.cksize;
	dsBufferDesc.lpwfxFormat = &WaveForMatex;

	if(m_pDirectSound->CreateSoundBuffer(&dsBufferDesc, &SoundBuffer, NULL) != DS_OK){	ASSERT(FALSE); return false;}

	if(SoundBuffer->Lock(0, ckInfo.cksize, &pAudio, &bytesAudio, NULL, NULL, NULL) != DS_OK) {	ASSERT(FALSE); return false;}
	//读取声音文件资料失败
	if(mmioRead(m_Hmmio, (HPSTR)pAudio, bytesAudio) == -1){	ASSERT(FALSE); return false;}
	//解除锁定缓冲区失败
	if(SoundBuffer->Unlock(pAudio, bytesAudio, NULL, NULL) != DS_OK){	ASSERT(FALSE); return false;}
	mmioClose(m_Hmmio, 0);
	return true;
}

bool CD3DSound::ReadFileBuffer(LPDIRECTSOUNDBUFFER& SoundBuffer,HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName)
{
	WAVEFORMATEX	*WaveForMatex = NULL;    //声明声音结构

	LPVOID pbWaveData =0;
	DWORD  cbWaveSize =0;
	//查找资源
	HRSRC hResource=FindResource(hInstance,pszResource,pszTypeName);
	if (hResource==NULL) return false;

	//读取资源
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
		//读取声音文件资料失败
		::CopyMemory(lpvPtr1, pbWaveData, dwBytes1);
		//解除锁定缓冲区失败
		if(SoundBuffer->Unlock(lpvPtr1, dwBytes1, NULL, NULL) != DS_OK){	ASSERT(FALSE); return false;}
	}
	return true;
}