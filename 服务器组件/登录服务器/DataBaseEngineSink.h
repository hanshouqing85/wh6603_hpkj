#ifndef DATABASE_ENGINE_SINK_HEAD_FILE
#define DATABASE_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "DataBasePacket.h"
#include <string>
using		namespace		std;

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
class CDataBaseEngineSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;

	//�û����ݿ�
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�
	//������ݿ�
protected:
	CDataBaseAide					m_TreasureDBAide;					//������ݿ�
	CDataBaseHelper					m_TreasureDBModule;					//������ݿ�

	//ƽ̨���ݿ�
protected:
	CDataBaseAide					m_PlatformDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_PlatformDBModule;					//ƽ̨���ݿ�

	//��Ϸ�������ݿ�
protected:
	CDataBaseAide					m_GameSSCDBAide;					//ƽ̨���ݿ�
	CDataBaseHelper					m_GameSSCDBModule;					//ƽ̨���ݿ�

	//�������
protected:
	CInitParameter *				m_pInitParameter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//��������
public:
	//���캯��
	CDataBaseEngineSink();
	//��������
	virtual ~CDataBaseEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnDataBaseEngineConclude(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter);
	//�����¼�
	virtual bool OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize);
	//�����¼�
	virtual bool OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	CString		ChangeStringToT(CStringA strInput);
	//�ʺŵ�¼
	bool OnRequestLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ϷͶע
	bool OnTouzhuCQSSC(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ͷ�����
	bool OnGetKefuUrl(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//У�����
	bool CheckTouzhuNum(CString strHaoma,int nLength,int nUserID,CString strQihao);
	//��ϷͶע��
	bool OnTouzhuCQSSCDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ϷͶע׷��
	bool OnTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	CString AddDouHao(CString strHaoma); //��׼����ʽ
	//�ֻ�Ͷע׷��
	bool OnMBTouzhuCQSSCZhuihao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡͶע��
	int GetTouZhuZongShu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡʱʱ��Ͷע��
	int GetSSCZhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡ11ѡ5Ͷע��
	int Get11X5Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡ3DͶע��
	int Get3DZhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡ���ǲ�Ͷע��
	int GetQXCZhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡPK10Ͷע��
	int GetPK10Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡ����8Ͷע��
	int GetKL8Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//��ȡ����28Ͷע��
	int GetXY28Zhushu(int nTypeID,int nKindID,CString strHaoma);
	//��¼����
protected:
	//�ʺ�ע��
	bool OnRequestRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ȡ��Ʊ�û���Ϣ
	bool OnGetCPUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ���¼
protected:
	//�ʺŵ�¼
	bool OnMobileLogonAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�ע��
	bool OnMobileRegisterAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ������ѯ
	bool OnQueryGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡϵͳʱ��
	bool OnQuerySystemTime(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�н��û�
	bool OnQueryWinUserID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡMAPBONUS
	bool OnQueryMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�����Ϣ
	bool OnQueryCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���bonus
	bool OnGetMyMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ��ҷ���
	bool OnGetUserFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������ҷ���
	bool OnSetUserBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//Ͷע��¼������ѯ
	bool OnGetTouzhuLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ּ�¼������ѯ
	bool OnGetTixianLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��¼������ѯ
	bool OnGetChongzhiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��¼������ѯ
	bool OnGetHYXXLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����¼������ѯ
	bool OnGetYingkuiLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����ϸ������ѯ
	bool OnGetYingkuiMxCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ӯ��������ѯ
	bool OnGetQiPaiYingkuiCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע������ѯ
	bool OnGetXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע������ѯ
	bool OnCHKXJTZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��������ѯ
	bool OnXJYKLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ��������ѯ
	bool OnXJYKTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���Ϸӯ��ͳ��������ѯ
	bool OnXJYXTjCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ֵ��־������ѯ
	bool OnXJCHZHLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�������־������ѯ
	bool OnXJTxLogCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ��ֵ��Ϣ
	bool OnGetChongzhiXinxi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯȡ����Ϣ
	bool OnGetQukuanInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯȡ����Ϣ
	bool OnQueryYinHangName(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�����Ϣ
	bool OnGetYueInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ���࿪����¼
	bool OnGetMoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�������
	bool OnGetDailiHuikui(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�������
	bool OnDailiLingjiang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�˳���Ϸ
	bool OnQuitGame(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ��
	bool OnUserQiandao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������
	bool OnGetUserHuanlesong(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�콱
	bool OnGetUserLingJiang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ȡ��
	bool OnDoQukuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//Ͷע��¼��ѯ
	bool OnGetTouzhuLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ֵ��־��ѯ
	bool OnGetXJCHZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�������־��ѯ
	bool OnGetXJTxLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ����¼��ѯ
	bool OnGetXJYKLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ�Ʋ�ѯ
	bool OnGetXJYKTj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���Ϸӯ��ͳ�Ʋ�ѯ
	bool OnGetXJYXTj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�����
	bool OnGetDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ҽ�
	bool OnGetZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����ת��
	bool OnGetLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ������ʸ�
	bool OnCanDaLiBao(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ҽ��ʸ�
	bool OnCanZaJinDan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����ת���ʸ�
	bool OnCanLuckyZhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼��ѯ
	bool OnGetXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼��ѯ
	bool OnCHKXJTZHLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼ͨ���˻���ѯ
	bool OnGetXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�����ͨ���˻���ѯ
	bool OnGetXJTxLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼ͨ��ID��ѯ
	bool OnGetXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼����ּ�¼ͨ��ID��ѯ
	bool OnGetXJTXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼ͨ���˻���ѯ
	bool OnCHKXJTZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ����¼ͨ��ID��ѯ
	bool OnGetXJYKLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ����¼ͨ���˻���ѯ
	bool OnGetXJYKLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ͨ��ID��ѯ�¼���ֵ��־
	bool OnGetXJCHZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ͨ���˻���ѯ�¼���ֵ��־
	bool OnGetXJCHZHLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ��ͨ��ID��ѯ
	bool OnGetXJYKTjByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ��ͨ���˻���ѯ
	bool OnGetXJYKTjByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��¼ͨ��ID��ѯ
	bool OnCHKXJTZHLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ա����
	bool OnGetHyShj(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ּ�¼��ѯ
	bool OnGetTixianLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��¼��ѯ
	bool OnGetHYXXLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��¼��ѯ
	bool OnGetHYXXList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��¼��ѯ
	bool OnGetHYXXLogByID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��¼��ѯ
	bool OnGetHYXXLogByAct(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸Ļ�Ա����
	bool OnXGHYFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����Ϊ����
	bool OnSWDaili(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣת��
	bool OnHYXXZhuanZhang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�¼����
	bool OnHYXXXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����¼����
	bool OnHYXXSetXiaJiPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��¼��ѯ
	bool OnGetChongzhiLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����¼��ѯ
	bool OnGetYingkuiLog(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����ϸ��ѯ
	bool OnGetYingkuiMx(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ӯ����ϸ��ѯ
	bool OnGetQiPaiYingkui(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯע������
	bool OnGetRegUrl(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ���
	bool OnGetPeie(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����
	bool OnCancelTouzhu(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ӻ�Ա
	bool OnAddHuiyuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������ҳע�᷵��
	bool OnSetWebFandian(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��ʾ
	bool OnChongzhiTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������ʾ
	bool OnTixianTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//Ͷע��ʾ
	bool OnTouzhuTishi(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ȡ�����Ϣ
	bool OnGetUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����
	bool OnGetNewsInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת��
	bool OnZhuanhuan(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�QQ
	bool OnXGqq(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸��Ա�
	bool OnXGGender(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸ĵ�¼����
	bool OnXGLoginPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ȡ������
	bool OnXGQukuanPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnLockMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ȡ���
	bool OnSetQukuanProtectPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ȡ���˻�
	bool OnSetQukuanZhanghuPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�ҵ�ȡ���˻�
	bool OnQueryMyYinHang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�ҵ��˻�����
	bool OnQueryMyProtect(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//Ͷע��ϸ��Ϣ
	bool OnGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ֻ�Ͷע��ϸ��Ϣ
	bool OnMBGetTouzhuXX(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����
	bool OnQueryYinhang(DWORD dwContextID, VOID * pData, WORD wDataSize);
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	//�˺ŷ���
protected:
	//�޸Ļ���
	bool OnRequestModifyMachine(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyLogonPass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyInsurePass(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ͷ��
	bool OnRequestModifySystemFace(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//���з���
protected:
	//������
	bool OnRequestUserSaveScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���
	bool OnRequestUserTakeScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת�˽��
	bool OnRequestUserTransScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���ϲ��ں�
	bool OnRequestLhcQihao(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��Ϣ��ѯ
protected:
	//��ѯ����
	bool OnRequestQueryIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ����
	bool OnRequestQueryInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//ϵͳ����
protected:
	//�����б�
	bool OnRequestLoadGameList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnRequestOnLineCountInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnRequestVirtualUserCount(DWORD dwContextID, VOID * pData, WORD wDataSize);

	void ReconnectSql();
	//�������
protected:
	//��¼���
	VOID OnLogonDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//���н��
	VOID OnInsureDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
	//�������
	VOID OnOperateDisposeResult(DWORD dwContextID, DWORD dwErrorCode, LPCTSTR pszErrorString, bool bMobileClient);
};

//////////////////////////////////////////////////////////////////////////////////

#endif