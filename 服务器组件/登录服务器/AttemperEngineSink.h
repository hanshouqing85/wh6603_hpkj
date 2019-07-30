#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "InitParameter.h"
#include "ServerListManager.h"
#include "DataBaseEngineSink.h"
//////////////////////////////////////////////////////////////////////////////////
//���ݶ���

//��������
#define CLIENT_KIND_FALSH			1									//��ҳ����
#define CLIENT_KIND_MOBILE			2									//�ֻ�����
#define CLIENT_KIND_COMPUTER		3									//��������

//������Ϣ
#define	KJ_QIHAO_LENGTH	30
#define	KJ_HAOMA_LENGTH	45
#define	KJ_SHIJIAN_LENGTH	30

struct tagLuckNumXinXi
{
	WORD  wKindID;
	TCHAR qihao[KJ_QIHAO_LENGTH];
	TCHAR haoma[KJ_HAOMA_LENGTH];
	TCHAR shijian[KJ_SHIJIAN_LENGTH];
};
struct tagLuckNum
{
	tagLuckNumXinXi LuckNum[5];
};
//�󶨲���
struct tagBindParameter
{
	//�������
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��

	//������Ϣ
	WORD							wModuleID;							//ģ���ʶ
	BYTE							cbClientKind;						//��������
	DWORD							dwPlazaVersion;						//�����汾
};


//////////////////////////////////////////////////////////////////////////////////
#include <map>


//���ȹ���
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CServiceUnits;
private:
	CTimeSpan						m_timespan ;						//���ݿ�ͷ�������ʱ���
	map<int,DWORD>							m_dwLuckyNumTickCount;
	int								m_nLuckNumID;
	//״̬����
protected:
	bool							m_bNeekCorrespond;					//Э����־
	bool                            m_bShowServerStatus;                //��ʾ������״̬
	//��������
protected:
	tagBindParameter *				m_pBindParameter;					//��������
//	AllLuckNum						m_AllLuckNum;
	tagLuckNum						m_MyLuckNum[30] ;					//����
	CString							m_strNewsTime;						//����ʱ��
	//�������
protected:
	CServerListManager				m_ServerListManager;				//�б����

	//����ӿ�
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketService;				//Э������

	//��������
public:
	CInitParameter *				m_pInitParameter;					//���ò���
	CDataBaseEngineSink	*			m_DataBaseEngineSink[4];
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool OnAttemperEngineConclude(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize);
	//�Զ��¼�
	virtual bool OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize);
	CString ChangeStringToT(CStringA strInput);
	//�����¼�
public:
	//Ӧ���¼�
	virtual bool OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID);
	//�ر��¼�
	virtual bool OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���Ӵ���
protected:
	bool OnGetSqlserverTimer();
	bool OnGetNews();
	bool OnGetWinUserID();
	bool OnGetMapBonusRet();

	//ע���¼�
	bool OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//Զ�̷���
	bool OnTCPSocketMainRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//�����¼�
protected:
	//��¼����
	bool OnTCPNetworkMainPCLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainPCServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//������
	bool OnTCPNetworkMainPCUserService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//Զ�̴���
	bool OnTCPNetworkMainPCRemoteService(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//��¼����
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�б���
	bool OnTCPNetworkMainMBServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	bool GetUserYueInfo(int nUserID,DWORD dwSocketID);
	//�����¼�
protected:
	//�ʺŵ�¼
	bool OnTCPNetworkSubPCLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubPCRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//��ȡ��Ʊ�û���Ϣ
	bool OnTCPNetworkSubPCGetCpUserInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//�ֻ��¼�
protected:
	//�ʺŵ�¼
	bool OnTCPNetworkSubMBLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�ʺ�ע��
	bool OnTCPNetworkSubMBRegisterAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//��Ϸ�¼�
protected:
	//��¼�ɹ�
	bool OnDBPCLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBPCLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ͷ��
	bool OnDBPCUserFaceInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnDBPCUserIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnDBPCUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���гɹ�
	bool OnDBPCUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ɹ�
	bool OnDBPCOperateSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ʧ��
	bool OnDBPCOperateFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ʊ�û���Ϣ
	bool OnGetCpUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ʊ�û���Ϣ
	bool OnGetMyUserMapBonus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ʊ�û�������Ϣ
	bool OnGetUserFandianRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ò�Ʊ�û�������Ϣ
	bool OnSetUserBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡMAPBONUS��Ϣ
	bool OnGetMapBonusRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ͷ�URL
	bool OnGetKefuUrlRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ϵͳʱ�䷵��
	bool OnGetSysTimeRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�н��û�ID����
	bool OnGetWinUserIDRet(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�ֻ��¼�
protected:
	//��¼�ɹ�
	bool OnDBMBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��¼ʧ��
	bool OnDBMBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize);
protected:
	//��ѯ��Ϸ�������
	bool OnDBQueryOpenResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ���з���
	bool OnDBQueryYinHangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ע����
	bool OnDBQueryTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ע��־��������
	bool OnDBQueryTouzhuLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ע�����ӷ���
	bool OnDBQueryRegUrlResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����
	bool OnDBQueryPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�¼�����
	bool OnDBQueryXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����¼�����
	bool OnDBQuerySetXjPeieResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������־��������
	bool OnDBQueryTixianLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��־��������
	bool OnDBQueryChongzhiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��־��������
	bool OnDBQueryHYXXLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����־��������
	bool OnDBQueryYingkuiLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����ϸ��������
	bool OnDBQueryYingkuiMxCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ӯ����ϸ��������
	bool OnDBQueryQiPaiYingkuiCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��־��������
	bool OnDBQueryXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�Ͷע��־��������
	bool OnDBCHKXITZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ����־��������
	bool OnDBXJYKLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ����������
	bool OnDBXJYKTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���Ϸӯ��ͳ����������
	bool OnDBXJYXTjCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ֵ��־��������
	bool OnDBXJCHZHLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�������־��������
	bool OnDBXJTxLogCountResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��Ϣ����
	bool OnDBGetChongzhiXinxiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ȡ����Ϣ����
	bool OnDBGetQukuanInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����Ϣ����
	bool OnDBGetYueInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���࿪����¼����
	bool OnDBGetMoreRecordResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����������
	bool OnDBGetDailiHuikuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����콱����
	bool OnDBDailiLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�˳���Ϸ����
	bool OnDBQuitGameResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ������
	bool OnDBUserQiandaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ͷ���
	bool OnDBUserHuanlesongResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�콱����
	bool OnDBUserLingjiangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ȡ���
	bool OnDBDoQukuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ע��־����
	bool OnDBGetTouzhuLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���ŷ���
	bool OnDBGetNewsResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ת������
	bool OnDBZhuanhuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ֵ��־����
	bool OnDBGetXJCHZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�������־����
	bool OnDBGetXJTxLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ����־����
	bool OnDBGetXJYKLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼�ӯ��ͳ�Ʒ���
	bool OnDBGetXJYKTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���Ϸӯ��ͳ�Ʒ���
	bool OnDBGetXJYXTjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ע��־����
	bool OnDBGetXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�¼���ע��־����
	bool OnDBCHKXJTZHLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������־����
	bool OnDBGetTixianLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣ��־����
	bool OnDBGetHYXXLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��־����
	bool OnDBGetChongzhiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����־����
	bool OnDBGetYingkuiLogResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ӯ����ϸ����
	bool OnDBGetYingkuiMxResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ӯ����ϸ����
	bool OnDBGetQiPaiYingkuiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����
	bool OnDBCancelTouzhuResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ӻ�Ա����
	bool OnDBAddHuiyuanResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���ϲ��ںŷ���
	bool OnDBGetLhcQihaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������ҳע�᷵�㷵��
	bool OnDBSetWebFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����Ϣ����
	bool OnDBGetUserInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�QQ����
	bool OnDBXGqqResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸��Ա𷵻�
	bool OnDBXGGenderResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸Ļ�Ա���㷵��
	bool OnDBXGHYFandianResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϊ������
	bool OnDBSzhidailiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ա��Ϣת�˷���
	bool OnDBHyxxZhuanZhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�QQ����
	bool OnDBXGLoginPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������������
	bool OnDBLockMachineResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�ȡ�����뷵��
	bool OnDBXGQukuanPassResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ȡ�������
	bool OnDBSetQukuanProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ҵ������˻�����
	bool OnDBQueryMyYinhangResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ҵ��˺ű�������
	bool OnDBQueryMyProtectResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ȡ���˻�����
	bool OnDBSetQukuanZhanghaoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡͶע��ϸ��Ϣ����
	bool OnDBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ֻ�Ͷע��ϸ��Ϣ����
	bool OnDBMBGetTouzhuXXResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ��Ա���ݷ���
	bool OnDBGetHyShjResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���������
	bool OnDBGetDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ҽ𵰷���
	bool OnDBGetZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����ת�̷���
	bool OnDBGetLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ������ʸ񷵻�
	bool OnDBCanDaLiBaoRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ������ʸ񷵻�
	bool OnDBCanZaJinDanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ����ת���ʸ񷵻�
	bool OnDBCanLuckyZhuanRet(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ֵ��ʾ����
	bool OnDBChongzhiTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//������ʾ����
	bool OnDBTixianTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//Ͷע��ʾ����
	bool OnDBTouzhuTishiResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�б��¼�
protected:
	//��Ϸ����
	bool OnDBPCGameTypeItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	bool OnDBPCGameKindItem(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnDBPCLoadVirtualCount(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��Ϸ�б�
	bool OnDBPCGameListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��������
protected:
	//�汾���
	bool CheckPlazaVersion(DWORD dwPlazaVersion, DWORD dwSocketID);
	//��������
	bool SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize);

	//�����б�
protected:
	//��������
	VOID SendGameTypeInfo(DWORD dwSocketID);
	//��������
	VOID SendGameKindInfo(DWORD dwSocketID);
	//���ͷ���
	VOID SendGameServerInfo(DWORD dwSocketID);

	//�ֻ��б�
protected:
	//��������
	VOID SendMobileKindInfo(DWORD dwSocketID, WORD wModuleID);
	//���ͷ���
	VOID SendMobileServerInfo(DWORD dwSocketID, WORD wModuleID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif