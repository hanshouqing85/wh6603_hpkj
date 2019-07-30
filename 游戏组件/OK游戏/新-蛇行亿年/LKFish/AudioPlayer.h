#ifndef AUDIO_PLAYER_HEAD_FILE
#define AUDIO_PLAYER_HEAD_FILE
#include <mmsystem.h>
#define MAX_SYN_SOUND					8								//�첽����
#pragma comment(lib, "Winmm.lib") 
//��������
class CAudioPlayer :public CWnd
{
	//��������
private:
	UINT							m_nDeviceType;							//�豸����
	//CGameFrameDlg					* m_pGameFrameDlg;						//��Ϸ���
	TCHAR							m_szAudioName[MAX_PATH];				//��������

	//��������
public:
	//���캯��
	CAudioPlayer();
	//��������
	virtual ~CAudioPlayer();

	//���ýӿ�
public:
	//���ÿ��
	//void SetGameFrameDlg(CGameFrameDlg *pGameFrameDlg) {m_pGameFrameDlg=pGameFrameDlg;}

	//���ܺ���
public:
	//��������
	bool Resume();
	//��ͣ����
	bool Pause();
	//ֹͣ����
	bool Stop();
	//�ر�����
	bool Close();
	//��������
	bool Play(LPCTSTR pszFileName, bool bLoop=false);
	//�ز�����
	bool RePlay();
	//��������
	UINT GetDeviceType() {return m_nDeviceType;}
	//��ȡ����
	LPCTSTR GetAudioName() {return m_szAudioName;}
	//��������
	void SetVoliceValue(int m_value);

	//��Ϣӳ��
private:
	//�����ص�
	LRESULT OnMCINotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

#endif