#pragma once
#define	KJ_QIHAO_LENGTH	30
#define	KJ_SHIJIAN_LENGTH	30

class CGameRule
{
public:
	virtual ~CGameRule(void);
protected:
	CGameRule(void);
	
public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0) = 0;
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj() = 0;
	//���´ο���ʱ�仹ʣ�µ�ʱ��
	virtual long GetKjShjDiff();
	//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
	//�Ƿ�ɳ���-�뿪��ʱ�����������
	virtual bool IsCanCancel(CString qihao);
	//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
	virtual long GetQiSpan();
	CTimeSpan m_timeSpan;
	void SetTimeSpan(CTimeSpan timeSpan)
	{
		m_timeSpan = timeSpan;
	}
};

class CChqSSCRule : public CGameRule
{
public:
	CChqSSCRule(void);
	virtual ~CChqSSCRule(void);

public:
	//���������ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	
	//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
	virtual long GetQiSpan();
	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//������
class CJxSSCRule  : public CGameRule
{
public:
	CJxSSCRule(void);
	virtual ~CJxSSCRule(void);
	int timespan_kj_shj;

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj(){return GetNextFdShj();}
	//���ڷⵥʱ��
	CTime GetNextFdShj();
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();

	CString GetFdShjDiffDesc();

	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
	virtual long GetQiSpan();
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

private:
	int timespan_fd_shj;
};


//�½���
class CXJSSCRule  : public CGameRule
{
public:
	CXJSSCRule(void);
	virtual ~CXJSSCRule(void);

public:
	//�����ں�
	CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj(){return GetNextFdShj();}
	//���ڷⵥʱ��
	CTime GetNextFdShj();
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	CString GetFdShjDiffDesc();

	bool IsCanCancel(CString qihao);

public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

private:
	int timespan_fd_shj;
};

class CFenFenCaiRule : public CGameRule
{
public:
	CFenFenCaiRule(void);
	virtual ~CFenFenCaiRule(void);

public:
	//���������ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();

	//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
	virtual long GetQiSpan();
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

class CWuFenCaiRule : public CGameRule
{
public:
	CWuFenCaiRule(void);
	virtual ~CWuFenCaiRule(void);

public:
	//���������ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond = 60);

	//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
	virtual long GetQiSpan();
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//�㶫11ѡ5
class CGD11X5Rule : public CGameRule
{
public:
	CGD11X5Rule(void);
	virtual ~CGD11X5Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);
private:		
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
class CHgydwfcRule : public CGameRule
{
public:
	CHgydwfcRule(void);
	virtual ~CHgydwfcRule(void);

public:
	//���������ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();

	//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
	virtual long GetQiSpan();
	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
	int  m_nStartQihao;

	CTime	m_tStartTime;
	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//����11ѡ5
class CCQ11X5Rule : public CGameRule
{
public:
	CCQ11X5Rule(void);
	virtual ~CCQ11X5Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();

private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//����11ѡ5
class CJX11X5Rule : public CGameRule
{
public:
	CJX11X5Rule(void);
	virtual ~CJX11X5Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond);
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
//ɽ��11ѡ5
class CSD11X5Rule : public CGameRule
{
public:
	CSD11X5Rule(void);
	virtual ~CSD11X5Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);

private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
//����PK10
class CBJPK10Rule : public CGameRule
{
public:
	CBJPK10Rule(void);
	virtual ~CBJPK10Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);

private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};


//������11ѡ5
class CHLJ11X5Rule : public CGameRule
{
public:
	CHLJ11X5Rule(void);
	virtual ~CHLJ11X5Rule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);
private:
	//ʱ��1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//ʱ��2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//ʱ��3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};


//���ǲ�
class CQiXingCaiRule : public CGameRule
{
public:
	CQiXingCaiRule(void);
	virtual ~CQiXingCaiRule(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj(){return GetNextFdShj();}
	//���ڷⵥʱ��
	CTime GetNextFdShj();

	//���´ηⵥʱ�仹ʣ�µ�ʱ��
	//long GetFdShjDiff();

	//CString GetFdShjDiffDesc();

	//bool IsCanCancel(CString qihao);

public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

private:
	int timespan_fd_shj;
	int timespan_kj_shj;
};

class CKuaiLe8RUle : public CGameRule
{
public:
	CKuaiLe8RUle(void);
	virtual ~CKuaiLe8RUle(void);

public:
	//�����ں�
	virtual CString GetNextExpect(int nDelta=0);
	//���ڿ���ʱ��
	virtual CTime GetNextKjShj();


private:

	int timespan_kj_shj;

	int startqihao;
	CTime startTime;

};