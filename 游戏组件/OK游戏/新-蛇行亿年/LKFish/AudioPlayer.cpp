#include "stdafx.h"
#include "AudioPlayer.h"
#include "Digitalv.h"

////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAudioPlayer, CWnd)
	ON_MESSAGE(MM_MCINOTIFY, OnMCINotify) 
END_MESSAGE_MAP()

//构造函数
CAudioPlayer::CAudioPlayer()
{
	//设置变量
	m_nDeviceType=0;
	m_szAudioName[0]=0;
}

//析构函数
CAudioPlayer::~CAudioPlayer()
{
	//关闭语音
	try 
	{
	if(0!=m_nDeviceType) Close();
	}
	catch (...){}
}

//播放语音
bool CAudioPlayer::Play(LPCTSTR pszFileName, bool bLoop)
{
	//关闭语音
	if(0!=m_nDeviceType) Close();

	

	

	//设置变量
	MCI_OPEN_PARMS pmOpen={0};
	pmOpen.dwCallback=0;

	pmOpen.lpstrAlias=NULL;
	pmOpen.lpstrDeviceType=TEXT("MPEGAudio");
	pmOpen.lpstrElementName=pszFileName;
	pmOpen.wDeviceID=NULL;

	//打开语音
	//TCHAR szError[256];
	MCIERROR mciResult=0;
	try
	{
	mciResult=mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT,(DWORD_PTR)&pmOpen);

	
	if(0==mciResult)
	{
		m_nDeviceType=pmOpen.wDeviceID;
	}
	else
	{
//		mciGetErrorString(mciResult,szError,CountArray(szError));
//#ifdef _DEBUG
//
//		m_pGameFrameDlg->InsertSystemString(szError+CString(TEXT("open")));
//#endif
	}
	}
	catch (...)
	{
//#ifdef _DEBUG
//		m_pGameFrameDlg->InsertSystemString(TEXT("2"));
//#endif
		return false;
	}

	//合法判断
	if (0==m_nDeviceType) 
	{
//#ifdef _DEBUG
//		m_pGameFrameDlg->InsertSystemString(TEXT("1"));
//#endif
		return false;
	}
	
	//设置变量
	MCI_PLAY_PARMS pmPlay={0};
	pmPlay.dwCallback=MAKEWPARAM(GetSafeHwnd(),0);
	pmPlay.dwFrom=0;
	pmPlay.dwTo=0;

	//播放语音
	try
	{

		//循环判断
		if (true==bLoop) mciResult=mciSendCommand(m_nDeviceType,MCI_PLAY,MCI_NOTIFY|MCI_DGV_PLAY_REPEAT,(DWORD_PTR)&pmPlay);
		else mciResult=mciSendCommand(m_nDeviceType,MCI_PLAY,MCI_NOTIFY,(DWORD_PTR)&pmPlay);
	}
	catch (...)
	{
//#ifdef _DEBUG
//		m_pGameFrameDlg->InsertSystemString(TEXT("0"));
//#endif
		return false;
	}

//#ifdef _DEBUG
//	if (0!=mciResult)
//	{
//		mciGetErrorString(mciResult,szError,CountArray(szError));
//		m_pGameFrameDlg->InsertSystemString(szError+CString(TEXT("play")));
//	}
//#endif

	//保留名称
//	lstrcpyn(m_szAudioName,pszFileName,CountArray(m_szAudioName));

	return mciResult==0?true:false;
}

//重播语音
bool CAudioPlayer::RePlay()
{
	//设置变量
	MCI_PLAY_PARMS pmPlay={0};
	pmPlay.dwCallback=MAKEWPARAM(GetSafeHwnd(),0);
	pmPlay.dwFrom=0;
	pmPlay.dwTo=0;

	//播放语音
	try
	{
		mciSendCommand(m_nDeviceType,MCI_PLAY,MCI_NOTIFY,(DWORD_PTR)&pmPlay);
	}
	catch (...)
	{
//#ifdef _DEBUG
//		m_pGameFrameDlg->InsertSystemString(TEXT("RePlay"));
//#endif
		return false;
	}

	return true;
}

//关闭语音
bool CAudioPlayer::Close()
{
	try
	{
	if(0!=m_nDeviceType)
	{
		MCI_GENERIC_PARMS gp={0};
		gp.dwCallback=NULL;
		mciSendCommand(m_nDeviceType,MCI_CLOSE,0,(DWORD_PTR)&gp);
		m_nDeviceType=0;
	}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//暂停语音
bool CAudioPlayer::Pause()
{
	try
	{
	if(0!=m_nDeviceType)
	{
		MCI_GENERIC_PARMS gp={0};
		gp.dwCallback=NULL;
		mciSendCommand(m_nDeviceType,MCI_PAUSE,0,(DWORD_PTR)&gp);
	}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//重置语音
bool CAudioPlayer::Resume()
{
	try
	{
	if(0!=m_nDeviceType)
	{
		MCI_GENERIC_PARMS gp={0};
		gp.dwCallback=NULL;
		mciSendCommand(m_nDeviceType,MCI_RESUME,0,(DWORD_PTR)&gp);
	}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//停止语音
bool CAudioPlayer::Stop()
{
	try
	{
	if(0!=m_nDeviceType)
	{
		MCI_GENERIC_PARMS gp={0};
		gp.dwCallback=NULL;
		mciSendCommand(m_nDeviceType,MCI_STOP,0,(DWORD_PTR)&gp);
	}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//语音回调
LRESULT CAudioPlayer::OnMCINotify(WPARAM wParam, LPARAM lParam)
{
	//关闭判断
	try
	{
		//关闭语音
		if(0!=m_nDeviceType) Close();
				
	/*	if (MCI_NOTIFY_SUCCESSFUL==wParam)
		{*/
			
//#ifdef _DEBUG
//			m_pGameFrameDlg->InsertSystemString(TEXT("MCI_NOTIFY_SUCCESSFUL"));
//#endif
		//}
//#ifdef _DEBUG
//		else if (MCI_NOTIFY_ABORTED==wParam)
//		{
//			m_pGameFrameDlg->InsertSystemString(TEXT("MCI_NOTIFY_ABORTED"));
//		}
//		else if (MCI_NOTIFY_FAILURE==wParam)
//		{
//			m_pGameFrameDlg->InsertSystemString(TEXT("MCI_NOTIFY_FAILURE"));
//		}
//		else if (MCI_NOTIFY_SUPERSEDED==wParam)
//		{
//			m_pGameFrameDlg->InsertSystemString(TEXT("MCI_NOTIFY_SUPERSEDED"));
//		}
//#endif

	}
	catch (...)
	{

	}

	return 0;
}
//设置音量
void  CAudioPlayer::SetVoliceValue(int m_value)
{
	//调节音量大小
		MCI_DGV_SETAUDIO_PARMS mciSetAudioPara;
        mciSetAudioPara.dwCallback = NULL;
        mciSetAudioPara.dwItem = MCI_DGV_SETAUDIO_VOLUME;
        mciSetAudioPara.dwValue = m_value*10; //此处就是音量大小 (0--1000)
        mciSendCommand(m_nDeviceType, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE |  MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&mciSetAudioPara);
   
}
