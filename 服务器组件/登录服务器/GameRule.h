#pragma once
#define	KJ_QIHAO_LENGTH	30
#define	KJ_SHIJIAN_LENGTH	30

class CGameRule
{
public:
	virtual ~CGameRule(void);
protected:
	CGameRule(void);
	static int GetSecByHMS(int h,int m,int s);
	//获取今天凌晨零时整的时间戳
	static time_t GetMorningTime(time_t t);	
public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0) = 0;
	//下期开奖时间
	virtual CTime GetNextKjShj() = 0;
	//离下次开奖时间还剩下的时间
	virtual long GetKjShjDiff();
	//离下次开奖时间还剩下的时间-字符串描述
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
	//是否可撤单-离开奖时间大于两分钟
	virtual bool IsCanCancel(CString qihao);
	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();

	void SetTimeSpan(CTimeSpan timeSpan);
	void SetStartQihao(int nQihao,CTime tStartTime);

	//下期期号
	string GetNextExpect_TJ(int nDelta=0);
	//下期开奖时间
	time_t GetNextKjShj_TJ();
	//获取期号
	int GetQiShu_TJ(int sec);
	//获取开奖时间
	int GetKjShj_TJ(int qishu);

	//下期期号
	string GetNextExpect_PK10(int nDelta = 0);
	//下期开奖时间
	time_t GetNextKjShj_PK10();
	//获取今天第一期的期号	
	int GetQiShu0_PK10();
	//今天到了多少期
	int GetQiShu_PK10(int sec);

protected:
	CTimeSpan m_timeSpan;
	int m_nStartQihao;
	CTime m_tStartTime;
	char m_para1[20];//TJ:"%s%03d"
	int m_iKjShjFirst;//每天第一期开奖时间
	int m_iKjShjLast;//每天最后一期开奖时间
	int m_qishu;//每天开奖期数
	int m_timespan;//开奖频率
	int m_fenDanDuration;//封单时间
};

class CChqSSCRule : public CGameRule
{
public:
	CChqSSCRule(void);
	virtual ~CChqSSCRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	
	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);

	//获取期号
	int GetQiShu(int sec);
	//获取开奖时间
	int GetKjShj(int qishu);
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//天津彩
class CJxSSCRule  : public CGameRule
{
public:
	CJxSSCRule(void);
	virtual ~CJxSSCRule(void);
	int timespan_kj_shj;

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	//离下次封单时间还剩下的时间
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


//新疆彩
class CXJSSCRule  : public CGameRule
{
public:
	CXJSSCRule(void);
	CXJSSCRule(const char *para1,int iKjShjFirst,int iKjShjLast,int qishu,int timespan,int fdtimespan);
	virtual ~CXJSSCRule(void);

public:
	//下期期号
	CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	CString GetFdShjDiffDesc();

	bool IsCanCancel(CString qihao);

	//获取期号
	int GetQiShu(int sec);
	//获取开奖时间
	int GetKjShj(int qishu);

public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

private:
	int timespan_fd_shj;
};
class CErFenCaiRule : public CGameRule
{
public:
	CErFenCaiRule(void);
	virtual ~CErFenCaiRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond = 60);

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

class CFenFenCaiRule : public CGameRule
{
public:
	CFenFenCaiRule(void);
	virtual ~CFenFenCaiRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
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
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond = 60);

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

class CTXFenFenCaiRule : public CGameRule
{
public:
	CTXFenFenCaiRule(void);
	virtual ~CTXFenFenCaiRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
class CQQFenFenCaiRule : public CGameRule
{
public:
	CQQFenFenCaiRule(void);
	virtual ~CQQFenFenCaiRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

class CCanadaRule : public CGameRule
{
public:
	CCanadaRule(void);
	virtual ~CCanadaRule(void);

public:
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
	int nQihaocha;
	void SetQihaoStart(int nQihao,CTime time);
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
	int  m_nStartQihao;

	CTime	m_tStartTime;
	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//广东11选5
class CGD11X5Rule : public CGameRule
{
public:
	CGD11X5Rule(void);
	virtual ~CGD11X5Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);
private:		
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
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
	//下期销售期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();

	//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
	virtual long GetQiSpan();
	bool IsCanCancel(CString qihao);
	virtual CString GetKjShjDiffDesc(int nSecond = 60);
	LONG nQihaocha;
	void SetQihaocha(int nQihao)
	{
	//	if(nQihaocha == 0||nQihaocha!=nQihao)
		{
			nQihaocha = nQihao;
			m_nStartQihao += nQihaocha;
		}
	}

private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
	int  m_nStartQihao;

	CTime	m_tStartTime;
	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//重庆11选5
class CCQ11X5Rule : public CGameRule
{
public:
	CCQ11X5Rule(void);
	virtual ~CCQ11X5Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();

private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};

//江西11选5
class CJX11X5Rule : public CGameRule
{
public:
	CJX11X5Rule(void);
	virtual ~CJX11X5Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond);
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
//山东11选5
class CSD11X5Rule : public CGameRule
{
public:
	CSD11X5Rule(void);
	virtual ~CSD11X5Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);

private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};
//北京PK10
class CBJPK10Rule : public CGameRule
{
public:
	CBJPK10Rule(void);
	virtual ~CBJPK10Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};


//黑龙江11选5
class CHLJ11X5Rule : public CGameRule
{
public:
	CHLJ11X5Rule(void);
	virtual ~CHLJ11X5Rule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();
	bool IsCanCancel(CString qihao);
	//离下次封单时间还剩下的时间
	long GetFdShjDiff();
	virtual CString GetKjShjDiffDesc(int nSecond=60);
private:
	//时间1 00:00-02:00
	int m_t1_start;
	int m_t1_end;
	//时间2 10:00-22:00
	int m_t2_start;
	int m_t2_end;	
	//时间3 22:00-24:00
	int m_t3_start;
	int m_t3_end;	
public:
	static TCHAR m_lastExpect[KJ_QIHAO_LENGTH];
	static TCHAR m_lastKjShj[KJ_SHIJIAN_LENGTH];

	int timespan_kj_shj;
	int timespan_ye_kj_shj;
};


//七星彩
class CQiXingCaiRule : public CGameRule
{
public:
	CQiXingCaiRule(void);
	virtual ~CQiXingCaiRule(void);

public:
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj(){return GetNextFdShj();}
	//下期封单时间
	CTime GetNextFdShj();

	//离下次封单时间还剩下的时间
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
	//下期期号
	virtual CString GetNextExpect(int nDelta=0);
	//下期开奖时间
	virtual CTime GetNextKjShj();

private:
	int timespan_kj_shj;
};