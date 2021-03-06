#include "StdAfx.h"
#include "XDate.h"		//公历转换为农历
#include "GameRule.h"

CGameRule::CGameRule(void)
{

}

CGameRule::~CGameRule(void)
{

}

int CGameRule::GetSecByHMS(int h,int m,int s)
{
	int sec=3600*h+60*m+s;
	return sec;
}

time_t CGameRule::GetMorningTime(time_t t)
{ 
    struct tm * tm=localtime(&t);  
    tm->tm_hour = 0;  
    tm->tm_min = 0;  
    tm->tm_sec = 0;  
    time_t ct0=mktime(tm);
	return ct0;
}

//离下次开奖时间还剩下的时间
long CGameRule::GetKjShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	CTime nexttime = GetNextKjShj();
// 
// 	CString strLog;
// 	strLog.Format(L"CHQSSC %02d:%02d:%02d now  %02d:%02d:%02d   ",nexttime.GetHour(),nexttime.GetMinute(),nexttime.GetSecond(),t.GetHour(),t.GetMinute(),t.GetSecond());
// 	OutputDebugString(strLog);
// 
	return (long)span.GetTotalSeconds();
}

//离下次开奖时间还剩下的时间-字符串描述
CString CGameRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	if(nSecond==10&&secDiff <= nSecond)
	{
		secDiff+=10;
	}
	else if(nSecond == 30)
	{
		if(secDiff <= nSecond)
			secDiff += 30;
	}

	if (secDiff <= nSecond &&nSecond != 5)		//|| secDiff > 540
	{
		return _T("00:00");
	}

	int minute = 0;
	int second = 0;
	if(nSecond==10)
	{
		minute = 0;
		second = secDiff % 60-10;
		if(second < 0)
		{
			second = 0;
		}
	}
	else if(nSecond == 30)
	{
		secDiff-=nSecond;

		minute = secDiff / 60;
		second = secDiff % 60;

	}
	else if(nSecond == 20)
	{
 		if(secDiff > nSecond)
 		{
 			secDiff-=20;
 		}
		minute = secDiff / 60 /*- 1*/;
		second = secDiff % 60;
	}
	else if(nSecond == 5)
	{
		if(secDiff > nSecond)
		{
			secDiff-=5;
		}

		CString strLog;
		strLog.Format(L"FFCSHJ secDiff:%d",secDiff);
		OutputDebugString(strLog);
		minute = secDiff / 60 /*- 1*/;
		second = secDiff % 60;
	}
	else
	{
		minute = secDiff / 60 - 1;
		second = secDiff % 60;
	}
	CString str;
	str.Format(_T("%02d:%02d"), minute, second);
// 	strLog.Format(L"KAIJIANGDEF %s",str);
// 	OutputDebugString(strLog);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CGameRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	CString strLog;
	strLog.Format(L"CANCANCEL qihao:%s,GetNext:%s,kjshj:%d",qihao,GetNextExpect(),GetKjShjDiff());
	OutputDebugString(strLog);

	return GetKjShjDiff() > 60; 
}

//每期之间间隔时间，标准时间
long CGameRule::GetQiSpan()
{
	return 300;
}

void CGameRule::SetTimeSpan(CTimeSpan timeSpan)
{
	m_timeSpan = timeSpan;
}

void CGameRule::SetStartQihao(int nQihao,CTime tStartTime)
{
	m_nStartQihao = nQihao;
	m_tStartTime = tStartTime;
}

//下期期号
string CGameRule::GetNextExpect_TJ(int nDelta)
{
	//if(getIsStopSell()) return "0";

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();
	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu_TJ(sec);
	////做出调整
	//qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec>=m_iKjShjLast)//期号算到第二天的第一期
	{
	    time_t ct1=GetMorningTime(ct+86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, m_para1, temp, qishu);
	return last;
}

time_t CGameRule::GetNextKjShj_TJ()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();
	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);

	int qishu=GetQiShu_TJ(sec);
    int kjshj=GetKjShj_TJ(qishu);
	time_t ct0=GetMorningTime(sec>=m_iKjShjLast?ct+86400:ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

int CGameRule::GetQiShu_TJ(int sec)
{
	int qishu = 0;
	if (sec < m_iKjShjFirst||sec>=m_iKjShjLast) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec >= m_iKjShjFirst && sec < m_iKjShjLast) //001期开奖——m_qishu期没开奖
	{
		long total = sec - m_iKjShjFirst;
		qishu = (int)(total / m_timespan+2);
	}
	return qishu;
}

int CGameRule::GetKjShj_TJ(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=m_qishu)
	{
	   int iKjShj=m_iKjShjFirst+m_timespan*(qishu-1);
	   return iKjShj;
	}
	return m_iKjShjFirst;
}

//下期期号
string CGameRule::GetNextExpect_PK10(int nDelta)
{
	//if(getIsStopSell()) return "0";
	
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct_now=t.GetTime();
	tm *tmLocal = localtime(&ct_now);
    int sec=GetSecByHMS(tmLocal->tm_hour,tmLocal->tm_min,tmLocal->tm_sec);
	int qishu=GetQiShu_PK10(sec);
	int qishu0 = GetQiShu0_PK10();
	int nQiHao = qishu0+qishu;
	char sztmp[32];
	sprintf(sztmp, "%ld", nQiHao);
	return string(sztmp);
}

//下期开奖时间
time_t CGameRule::GetNextKjShj_PK10()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct_now=t.GetTime();
	tm *tmLocal = localtime(&ct_now);

    int sec=GetSecByHMS(tmLocal->tm_hour,tmLocal->tm_min,tmLocal->tm_sec);
	
	int qishu=GetQiShu_PK10(sec);
    int kjshj=GetKjShj_TJ(qishu+1);
	time_t ct0=GetMorningTime(sec>=m_iKjShjLast?ct_now+86400:ct_now);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

int CGameRule::GetQiShu0_PK10()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();
    time_t tStartTime=GetMorningTime(ct)+m_iKjShjFirst;//今天第一期的开奖时间
	int qishu0 = m_nStartQihao + (tStartTime - m_tStartTime.GetTime() )/ 86400 * m_qishu;
	return qishu0;
}

int CGameRule::GetQiShu_PK10(int sec)
{
	int qishu = GetQiShu_TJ(sec)-1;
	return qishu;
}

CChqSSCRule::CChqSSCRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{

}

CChqSSCRule::~CChqSSCRule(void)
{
	//fenDanDuration = 35;
}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CChqSSCRule::GetQiSpan()
{
	return 1200;
}

int CChqSSCRule::GetQiShu(int sec)
{
	int qishu = 0;
	if (sec < 1800||sec>=85800) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec >= 1800 && sec<11400) //001期开奖——009期没开奖
	{				
		long total = sec - 1800;
		qishu = (int)(total / 1200 + 2);
	}
	else if (sec >= 11400 && sec<27000) //010期没开奖
	{				
		qishu = 10;
	}
	else if (sec >= 27000 && sec < 85800) //010期开奖——059期没开奖
	{
		long total = sec - 27000;
		qishu = (int)(total / 1200+11);
	}
	return qishu;
}

int CChqSSCRule::GetKjShj(int qishu)
{
	//前9期,等差数列求通项公式
	if(qishu>=1 && qishu<=9)
	{
	   int iKjShj=1800+1200*(qishu-1);
	   return iKjShj;
	}
	//后50期,等差数列求通项公式
	else if(qishu>=10 && qishu<=59)
	{
	   int iKjShj=27000+1200*(qishu-10);
	   return iKjShj;
	}
	return 1800;
}

//下期期号
CString CChqSSCRule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();

	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	//做出调整
	qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec>=85800)//期号算到第二天的第一期
	{
	    time_t ct1=GetMorningTime(ct+86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, "%s%03d", temp, qishu);

	string str=last;
	CString ret=CA2T(str.c_str());
	return ret;
}

//下期开奖时间
CTime CChqSSCRule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();

	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu);
	time_t ct0=GetMorningTime(sec>=85800?ct+86400:ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

//离下次开奖时间还剩下的时间-字符串描述
CString CChqSSCRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

 	if(secDiff >= 600)
 	{
 		secDiff -= 600;
 	}

	int minute = 0;
	int second = 0;
	if(secDiff > nSecond)
	{
		secDiff-=nSecond;
	}
	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CChqSSCRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > 35; 
}

//////////////加拿大3.5分彩/////////////////////////////////
CCanadaRule::CCanadaRule(void)
: m_t1_start(0)
, m_t1_end(19)
, m_t2_start(20)
, m_t2_end(24)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(210)
, timespan_ye_kj_shj(300)
,m_tStartTime(2016,8,11,0,2,30)
{
	m_nStartQihao = 2031613;  //8.2官方开奖 少开100期，初始值减掉100



}
CCanadaRule::~CCanadaRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CCanadaRule::GetQiSpan()
{
	return timespan_kj_shj;
}

//下期期号
CString CCanadaRule::GetNextExpect(int nDelta)
{

	int nQihao = m_nStartQihao;
	if(nQihao == 0)
	{
		return _T("00 00 00");
	}

	CTime tStart= CTime::GetCurrentTime();;
	tStart +=m_timeSpan;

	CTimeSpan tSpan = tStart - m_tStartTime;

	nQihao += ((tSpan.GetTotalSeconds()/ 210) + 1);
	CString strQihao;
	strQihao.Format(L"%d",nQihao);
	return strQihao;
}

//下期开奖时间
CTime CCanadaRule::GetNextKjShj()
{
	if (m_nStartQihao != 0)
	{
		CTime tStart= CTime::GetCurrentTime();
		tStart+=m_timeSpan;

		CTime t = m_tStartTime;
		CTimeSpan tSpan = tStart - m_tStartTime;
		int nQiHao =((tSpan.GetTotalSeconds()/ 210) + 1);
		t += CTimeSpan(0, 0, 0, nQiHao * 210);
		return t;
	}
	else
	{
		m_tStartTime += CTimeSpan(0,0,0,210);
		return m_tStartTime;
	}

}
void CCanadaRule::SetQihaoStart(int nQihao,CTime time)
{
	m_nStartQihao = nQihao;
	m_tStartTime = time;
	return ;

}

//离下次开奖时间还剩下的时间-字符串描述
CString CCanadaRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();


	int minute = 0;
	int second = 0;
	if(secDiff > nSecond)
	{
		secDiff-=nSecond;
	}
	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CCanadaRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > 45; 
}


TCHAR CJxSSCRule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CJxSSCRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CJxSSCRule::CJxSSCRule(void)
: timespan_fd_shj(60*9+15)
, timespan_kj_shj(600)
{
	m_iKjShjFirst=33600;
	m_iKjShjLast=82800;
	m_qishu=42;
	m_timespan=1200;
	//fenDanDuration = 45; //封单时间
	//m_gameKind=CZ_TianJinSSC;
	strcpy(m_para1,"%s%03d");
}

CJxSSCRule::~CJxSSCRule(void)
{

}

//下期期号
CString CJxSSCRule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_TJ(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CJxSSCRule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_TJ();
	CTime t=ct;
	return t;
}

long CJxSSCRule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}

CString CJxSSCRule::GetFdShjDiffDesc()
{

	long secDiff = GetFdShjDiff();
	if (secDiff <= 0 || secDiff > 480)
	{
		return _T("00:00");
	}

	int minute = secDiff / 60;
	int second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);


	return str;
}
long CJxSSCRule::GetQiSpan()
{
	return timespan_kj_shj;
}


//是否可撤单-离开奖时间大于两分钟
bool CJxSSCRule::IsCanCancel(CString qihao)
{
	CString strLog;
	strLog.Format(L"jxssc qihao:%s,nextqihao:%s",qihao , GetNextExpect() );
	OutputDebugString(strLog);
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

//CString strLog;
	strLog.Format(L"jxssc fdshijian:%d",GetFdShjDiff() );
	OutputDebugString(strLog);
	return GetFdShjDiff() > 140; 
}
//离下次开奖时间还剩下的时间-字符串描述
CString CJxSSCRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"TJSSC secDiff:%d",secDiff);
	OutputDebugString(strLog);

	if(secDiff >= 140)
	{
		secDiff -= 120;
	}

	if(nSecond==10&&secDiff <= nSecond)
	{
		secDiff+=10;
	}
	else if(nSecond == 30)
	{
		if(secDiff <= nSecond)
			secDiff += 30;
	}

	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		return _T("00:00");
	}

	int minute = 0;
	int second = 0;
	if(nSecond==10)
	{
		minute = 0;
		second = secDiff % 60-10;
		if(second < 0)
		{
			second = 0;
		}
	}
	else if(nSecond == 30)
	{
		secDiff-=nSecond;

		minute = secDiff / 60;
		second = secDiff % 60;

	}
	else if(nSecond == 20)
	{
		if(secDiff > nSecond)
		{
			secDiff-=20;
		}
		minute = secDiff / 60 /*- 1*/;
		second = secDiff % 60;
	}
	else
	{
		minute = secDiff / 60 - 1;
		second = secDiff % 60;
	}
	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//----------------------------------------------------------------
TCHAR CXJSSCRule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CXJSSCRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CXJSSCRule::CXJSSCRule(void)
: timespan_fd_shj(600)
{
	m_iKjShjFirst=37200;
	m_iKjShjLast=93600;//第二天的凌晨2时整
	m_qishu=48;
	m_timespan=1200;
	m_fenDanDuration = 60;//封单时间
	strcpy(m_para1,"%s%02d");
}

CXJSSCRule::CXJSSCRule(const char *para1,int iKjShjFirst,int iKjShjLast,int qishu,int timespan,int fdtimespan)
{
	m_iKjShjFirst=iKjShjFirst;
	m_iKjShjLast=iKjShjLast;
	m_qishu=qishu;
	m_timespan=timespan;
	m_fenDanDuration = fdtimespan;
	strcpy(m_para1,para1);
}

CXJSSCRule::~CXJSSCRule(void)
{

}

int CXJSSCRule::GetQiShu(int sec)
{
	int sc=m_iKjShjLast-86400;//时差为2小时
	int sec1=(sec+86400-sc)%86400;//乌鲁木齐时间sec1比北京时间sec晚两个小时
	int qishu = 0;
	if (sec1 < (m_iKjShjFirst-sc)) //001期没开奖
	{				
		qishu = 1;
	}
	else if (sec1 >= (m_iKjShjFirst-sc) && sec1 < (m_iKjShjLast-sc)) //001期开奖——m_qishu期没开奖
	{
		long total = sec1 - (m_iKjShjFirst-sc);
		qishu = (int)(total / m_timespan+2);
	}
	return qishu;
}

int CXJSSCRule::GetKjShj(int qishu)
{
	//等差数列求通项公式
	if(qishu>=1 && qishu<=m_qishu)
	{
	   int iKjShj=m_iKjShjFirst+m_timespan*(qishu-1);//取值范围为[m_iKjShjFirst, m_iKjShjLast]
	   return iKjShj;
	}
	return m_iKjShjFirst;
}

//下期期号
CString CXJSSCRule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();

	struct tm *my_tm = localtime(&ct);
    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	int qishu=GetQiShu(sec);
	////做出调整
	//qishu += nDelta;
 
	tm *tmLocal=my_tm;
	if(sec<m_iKjShjLast-86400)//期号算到前一天的后七期
	{
	    time_t ct1=GetMorningTime(ct-86400);
	    tmLocal = localtime(&ct1);
	}
	char temp[64] = {0};
	strftime(temp, sizeof(temp), "%Y%m%d",tmLocal);

	char last[64] = {0};
	sprintf(last, m_para1, temp, qishu);

	string str=last;
	CString ret=CA2T(str.c_str());
	return ret;
}

CTime CXJSSCRule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	time_t ct=t.GetTime();
	struct tm *my_tm = localtime(&ct);

    int sec=GetSecByHMS(my_tm->tm_hour,my_tm->tm_min,my_tm->tm_sec);
	
	int qishu=GetQiShu(sec);
    int kjshj=GetKjShj(qishu);
	time_t ct0=GetMorningTime(ct);//凌晨零时整的时间戳
	time_t t1 = ct0+kjshj;
	return t1;
}

long CXJSSCRule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}

CString CXJSSCRule::GetFdShjDiffDesc()
{

	long secDiff = GetFdShjDiff();
	if (secDiff <= 60 || secDiff > 540)
	{
		return _T("00:00");
	}

	int minute = secDiff / 60 - 1;
	int second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CXJSSCRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > m_fenDanDuration; 
}

////////////////////////////////////////////

CFenFenCaiRule::CFenFenCaiRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(60)
, timespan_ye_kj_shj(300)
{

}

CFenFenCaiRule::~CFenFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CFenFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CFenFenCaiRule::GetFdShjDiff()
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//下期期号
CString CFenFenCaiRule::GetNextExpect(int nDelta)
{
//  	CTime ct;
//  	theApp.GetTime(ct);
//  	int qishu = 0;
//  
//  	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);
//  	CTimeSpan tSpan = ct - t;
//  //	if (tSpan.GetTotalSeconds() % timespan_kj_shj == 0)
//  	{
//  		qishu =(int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 2;
//  	}
// //  	else
// //  	{
// //  		qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
// //  	}
//   	//做出调整
//  	qishu += nDelta;
//  	CString tmp = ct.Format(_T("%Y%m%d"));
//  	CString rQh;
//  	rQh.Format(_T("%s%04d"), tmp, qishu);
//  	return rQh;
	int nQiHao =0;

	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	nQiHao = tm.GetHour() * 60 + tm.GetMinute() +1;

	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%s%04ld", tm.Format(L"%Y%m%d"), nQiHao);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
bool CFenFenCaiRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 5; 
}

//下期开奖时间
CTime CFenFenCaiRule::GetNextKjShj()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);
		CTimeSpan tSpan = ct - t;
		int qishu = 0;

		qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+1;
		t += CTimeSpan(qishu * timespan_kj_shj);

		return t;
}
////////////////////////////////////////////

////////////////////////////////////////////

CTXFenFenCaiRule::CTXFenFenCaiRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(60)
, timespan_ye_kj_shj(300)
{

}

CTXFenFenCaiRule::~CTXFenFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CTXFenFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CTXFenFenCaiRule::GetFdShjDiff()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - ct;

	return (long)span.GetTotalSeconds();
}
//下期期号
CString CTXFenFenCaiRule::GetNextExpect(int nDelta)
{
	int nQiHao =0;

	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	tm+= CTimeSpan(0,0,0,10);
	nQiHao = tm.GetHour() * 60 + tm.GetMinute() +1;

	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%s%04ld", tm.Format(L"%Y%m%d"), nQiHao);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
bool CTXFenFenCaiRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 0; 
}

//下期开奖时间
CTime CTXFenFenCaiRule::GetNextKjShj()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 50);
	CTimeSpan tSpan = ct - t;
	int qishu = 0;

	qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+1;
	t += CTimeSpan(qishu * timespan_kj_shj);

	CString strLog;
	strLog.Format(L"TXFFC FD:%s,NOW:%s",t.Format(_T("%Y%m%d %H:%M:%S")),ct.Format(_T("%Y%m%d %H:%M:%S")));
	OutputDebugString(strLog);
	return t;

}
CString CTXFenFenCaiRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"TXFENCAI secDiff:%d",secDiff);
	OutputDebugString(strLog);

	int minute = 0;
	int second = 0;

	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

////////////////////////////////////////////
////////////////////////////////////////////

CQQFenFenCaiRule::CQQFenFenCaiRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(60)
, timespan_ye_kj_shj(300)
{

}

CQQFenFenCaiRule::~CQQFenFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CQQFenFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CQQFenFenCaiRule::GetFdShjDiff()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - ct;

	return (long)span.GetTotalSeconds();
}
//下期期号
CString CQQFenFenCaiRule::GetNextExpect(int nDelta)
{
	int nQiHao =0;

	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	tm-= CTimeSpan(0,0,0,10);

	nQiHao = tm.GetHour() * 60 + tm.GetMinute() +1;

	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%s%04ld", tm.Format(L"%Y%m%d"), nQiHao);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
bool CQQFenFenCaiRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 5; 
}

//下期开奖时间
CTime CQQFenFenCaiRule::GetNextKjShj()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 1, 10);
	CTimeSpan tSpan = ct - t;
	int qishu = 0;

	qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+1;
	t += CTimeSpan(qishu * timespan_kj_shj);

	CString strLog;
	strLog.Format(L"TXFFC FD:%s,NOW:%s",t.Format(_T("%Y%m%d %H:%M:%S")),ct.Format(_T("%Y%m%d %H:%M:%S")));
	OutputDebugString(strLog);

	return t;

}
CString CQQFenFenCaiRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"TXFENCAI secDiff:%d",secDiff);
	OutputDebugString(strLog);

	int minute = 0;
	int second = 0;

	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

////////////////////////////////////////////

CErFenCaiRule::CErFenCaiRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(120)
, timespan_ye_kj_shj(300)
{

}

CErFenCaiRule::~CErFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CErFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
long CErFenCaiRule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();;
	t+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//下期期号
CString CErFenCaiRule::GetNextExpect(int nDelta)
{
	int nQiHao =0;
	CTime tm=CTime::GetCurrentTime();
	tm+=m_timeSpan;
	CTime t(tm.GetYear(), tm.GetMonth(), tm.GetDay(), 0, 0, 0);
	CTimeSpan tSpan = tm - t;


	nQiHao = (tSpan.GetTotalSeconds())/timespan_kj_shj +1;

	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%s%03ld", tm.Format(L"%Y%m%d"), nQiHao);
	return strQiHao;

}
//是否可撤单-离开奖时间大于两分钟
bool CErFenCaiRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 13; 
}

//下期开奖时间
CTime CErFenCaiRule::GetNextKjShj()
{
	CTime ct=CTime::GetCurrentTime();
	ct+=m_timeSpan;

	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);
	CTimeSpan tSpan = ct - t;

	int qishu = 0;

	qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+1;
	t += CTimeSpan(qishu * timespan_kj_shj);

	CString strLog;
	strLog.Format(L"%s",t.Format(L"ERFENCAI %Y%m%d %H:%M:%S"));
	OutputDebugString(strLog);
	return t;
}
CString CErFenCaiRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"ERFENCAI secDiff:%d",secDiff);
	OutputDebugString(strLog);
	if(secDiff > nSecond)
	{
		secDiff-=15;
	}
	int minute = 0;
	int second = 0;

	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}


////////////////////////////////////////////
CWuFenCaiRule::CWuFenCaiRule(void)
: m_t1_start(0)
, m_t1_end(2)
, m_t2_start(10)
, m_t2_end(22)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(300)
, timespan_ye_kj_shj(300)
{

}

CWuFenCaiRule::~CWuFenCaiRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CWuFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
//离下次开奖时间还剩下的时间-字符串描述
CString CWuFenCaiRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	if(secDiff >= 600)
	{
		secDiff -= 600;
	}

	int minute = 0;
	int second = 0;
	if(secDiff > nSecond)
	{
		secDiff-=nSecond;
	}
	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

long CWuFenCaiRule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CWuFenCaiRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 45; 
}

//下期期号
CString CWuFenCaiRule::GetNextExpect(int nDelta)
{
		CTime ct= CTime::GetCurrentTime();
		ct+=m_timeSpan;

	  	int qishu = 0;
	  
	  	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);
	  	CTimeSpan tSpan = ct - t;
	  //	if (tSpan.GetTotalSeconds() % timespan_kj_shj == 0)
	  	{
	  		qishu =(int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
	  	}
	 //  	else
	 //  	{
	 //  		qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
	 //  	}
	   	//做出调整
	  	qishu += nDelta;
	  	CString tmp = ct.Format(_T("%Y%m%d"));
	  	CString rQh;
	  	rQh.Format(_T("%s%03d"), tmp, qishu);
	  	return rQh;
// 	int nQiHao =0;
// 	CTime tm;//=CTime::GetCurrentTime();
// 	theApp.GetTime(tm);
// 	nQiHao = tm.GetHour() * 60 + tm.GetMinute() +1;
// 
// 	nQiHao+=nDelta;
// 	CString strQiHao;
// 	strQiHao.Format(L"%s%03ld", tm.Format(L"%Y%m%d"), nQiHao);
// 	return strQiHao;

}

//下期开奖时间
CTime CWuFenCaiRule::GetNextKjShj()
{

	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;

	//	int hour = ct.GetHour();
	// 	if (hour < m_t1_end)
	// 	{
	// 		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t1_start, 0, 0);
	// 		CTimeSpan tSpan = ct - t;
	// 		int qishu = 0;
	// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 240)
	// 		{
	// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj)+2;
	// 		}
	// 		else
	// 		{
	// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
	// 		}
	// 
	// 		t += CTimeSpan(qishu * timespan_ye_kj_shj);
	// 		return t;
	// 	}
	// 	else if (hour >= m_t2_start && hour < m_t2_end)
	//	{

	// 	int nQiHao =0;
	// 	CTime tm;//=CTime::GetCurrentTime();
	// 	theApp.GetTime(tm);
	// 	nQiHao = tm.GetHour() * 60 + tm.GetMinute() +2;
	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);
	CTimeSpan tSpan = ct - t;
	int qishu = 0;

	//		if (tSpan.GetTotalSeconds() % timespan_kj_shj == 0)
	{
		qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+1;
	}
	// 		else
	// 		{
	// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
	// 		}
	t += CTimeSpan(qishu * timespan_kj_shj);

	//t -= CTimeSpan(0,0,0,10);
	return t;
	//	}
	// 	else if (hour >= m_t3_start)
	// 	{
	// 		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t3_start, 0, 0);
	// 		CTimeSpan tSpan = ct - t;
	// 		int qishu = 0;
	// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 240)
	// 		{
	// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj)+2;
	// 		}
	// 		else
	// 		{
	// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
	// 		}
	// 		t += CTimeSpan(qishu * timespan_ye_kj_shj);
	// 		return t;
	// 	}
	// 	else
	// 	{
	// 		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t2_start, 0, 0);
	// 		t += CTimeSpan(timespan_kj_shj);
	// 		return t;
	// 	}
}


///////////////////////////////////////////////////
TCHAR CGD11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CGD11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CGD11X5Rule::CGD11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{
	m_iKjShjFirst=34200;
	m_iKjShjLast=83400;
	m_qishu=42;
	m_timespan=1200;
	//fenDanDuration = 60; //封单时间
	//m_gameKind=CZGD11Xuan5;
	strcpy(m_para1,"%s%02d");
}

CGD11X5Rule::~CGD11X5Rule(void)
{

}
//离下次开奖时间还剩下的时间-字符串描述
CString CGD11X5Rule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();


	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		secDiff+=nSecond;
	}

	int minute = 0;
	int second = 0;
	minute = secDiff / 60 - 1;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//下期期号
CString CGD11X5Rule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_TJ(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CGD11X5Rule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_TJ();
	CTime t=ct;
	return t;
}

long CGD11X5Rule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CGD11X5Rule::IsCanCancel(CString qihao)
{
	CString strLog;
	strLog.Format(L"CGD11X5Rule qihao:%s,nextqh:%s,fdshj:%d",qihao,GetNextExpect(),GetFdShjDiff());
	OutputDebugString(strLog);
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

//////////////韩国1.5分彩/////////////////////////////////
CHgydwfcRule::CHgydwfcRule(void)
: m_t1_start(0)
, m_t1_end(5)
, m_t2_start(7)
, m_t2_end(24)	
, m_t3_start(22)
, m_t3_end(24)
, timespan_kj_shj(90)
, timespan_ye_kj_shj(300)
,m_tStartTime(2016,7,21,0,0,0)
{
	m_nStartQihao = 1588649;



}
CHgydwfcRule::~CHgydwfcRule(void)
{

}

//获取每期时间间隔,像重庆时时彩，有时候5分钟，有时候10分钟。
long CHgydwfcRule::GetQiSpan()
{
	return timespan_kj_shj;
}

//下期期号
CString CHgydwfcRule::GetNextExpect(int nDelta)
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 10);


	int hour = ct.GetHour();


	CTimeSpan tSpan = ct - m_tStartTime;
	int nDays = tSpan.GetDays();
	int nQihao = m_nStartQihao + (tSpan.GetDays()*880);

	if (hour<m_t1_end)
	{
		CTimeSpan span = ct-t;
		nQihao += ((span.GetTotalSeconds() / 90) + 1);
	}
	else if (hour>=m_t1_end&&hour<m_t2_start)
	{

		CTime tNow(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 7, 0, 0);

		CTimeSpan span = tNow - t; 
		nQihao += ((tSpan.GetTotalSeconds() / 90) + 1);
		nQihao -= 80;
	}
	else
	{

		CTimeSpan span = ct - t; 
		nQihao += ((span.GetTotalSeconds() / 90) + 1);
		nQihao -= 80;
	}   

	//做出调整
	nQihao += nDelta;
	//CString tmp = ct.Format(_T("%Y%m%d"));
	CString rQh;
	rQh.Format(_T("%d"), nQihao);
	return rQh;
}

//下期开奖时间
CTime CHgydwfcRule::GetNextKjShj()
{

	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 10);


	int hour = ct.GetHour();


	CTimeSpan tSpan = ct - t;
	int nDays = tSpan.GetDays();
	int nQihao = m_nStartQihao + (tSpan.GetDays()*880);


	if (hour<m_t1_end)
	{
		nQihao = ((tSpan.GetTotalSeconds() / timespan_kj_shj) + 1);
		t += CTimeSpan(nQihao * timespan_kj_shj);
	}
	else if (hour>=m_t1_end&&hour<m_t2_start)
	{

		CTime tNow(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 7, 0, 0);

		CTimeSpan span = tNow - t; 
		nQihao = ((tSpan.GetTotalSeconds() / timespan_kj_shj) + 1);
		//nQihao -= 80;
		t += CTimeSpan(nQihao * timespan_kj_shj);
	}
	else
	{

		CTimeSpan span = ct - t; 
		nQihao = ((span.GetTotalSeconds() / 90) + 1);
		//nQihao -= 80;
		t += CTimeSpan(nQihao * timespan_kj_shj);
	}   

	return t;
}
//离下次开奖时间还剩下的时间-字符串描述
CString CHgydwfcRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	if(secDiff >= 90&&secDiff<=95)
	{
		secDiff -= 90;
	}

	int minute = 0;
	int second = 0;
	if(secDiff > nSecond)
	{
		secDiff-=nSecond;
	}
	minute = secDiff / 60 /*- 1*/;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

//是否可撤单-离开奖时间大于两分钟
bool CHgydwfcRule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetKjShjDiff() > 5; 
}

////////////////////////////////////////////

TCHAR CCQ11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CCQ11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CCQ11X5Rule::CCQ11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{

}

CCQ11X5Rule::~CCQ11X5Rule(void)
{

}

//下期期号
CString CCQ11X5Rule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour()>=10)&& (t.GetHour() <= 23) ){
		CString qihao = CCQ11X5Rule::m_lastExpect;
		CString qh1 = qihao.Left(8);
		CString qh2 = qihao.Right(2);

		int iQh = _ttoi(qh2);
		iQh ++;
		//做出调整
		iQh += nDelta;
		rQh.Format(_T("%s%02d"), qh1, iQh);
	}
	else {
		if(t.GetHour() >=23 && t.GetHour() <=24)
			t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

//下期开奖时间
CTime CCQ11X5Rule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour()>=10) && (t.GetHour() <= 23) ){
		CString lastKjShj = m_lastKjShj;

		COleVariant vtime(lastKjShj);
		vtime.ChangeType(VT_DATE);
		COleDateTime time4 = vtime;

		SYSTEMTIME systime;
		VariantTimeToSystemTime(time4, &systime);
		CTime ctm(systime);

		ctm += timespan_kj_shj;
		return ctm;
	}
	else {

		if(t.GetHour() >=23 && t.GetHour() <=24)
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 10, 0);
		return ctm;
	}
}
long CCQ11X5Rule::GetFdShjDiff()
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CCQ11X5Rule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

////////////////////////////////////////////

TCHAR CJX11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CJX11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CJX11X5Rule::CJX11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{
	m_iKjShjFirst=34200;
	m_iKjShjLast=83400;
	m_qishu=42;
	m_timespan=1200;
	//fenDanDuration = 60; //封单时间
	//m_gameKind=CZJX11Xuan5;
	strcpy(m_para1,"%s%02d");
}

CJX11X5Rule::~CJX11X5Rule(void)
{

}
long CJX11X5Rule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CJX11X5Rule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

//下期期号
CString CJX11X5Rule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_TJ(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CJX11X5Rule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_TJ();
	CTime t=ct;
	return t;
}

//离下次开奖时间还剩下的时间-字符串描述
CString CJX11X5Rule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();


	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		secDiff+=nSecond;
	}

	int minute = 0;
	int second = 0;
	minute = secDiff / 60 - 1;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}

////////////////////////////////////////////

TCHAR CSD11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CSD11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CSD11X5Rule::CSD11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{
	m_iKjShjFirst=32400;
	m_iKjShjLast=82800;
	m_qishu=43;
	m_timespan=1200;
	//fenDanDuration = 60; //封单时间
	//m_gameKind=CZSD11Xuan5;
	strcpy(m_para1,"%s%02d");
}

CSD11X5Rule::~CSD11X5Rule(void)
{

}
long CSD11X5Rule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CSD11X5Rule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//下期期号
CString CSD11X5Rule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_TJ(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CSD11X5Rule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_TJ();
	CTime t=ct;
	return t;
}
//离下次开奖时间还剩下的时间-字符串描述
CString CSD11X5Rule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();


	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		secDiff+=nSecond;
	}

	int minute = 0;
	int second = 0;
	minute = secDiff / 60 - 1;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}


////////////////////////////////////////////

TCHAR CBJPK10Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CBJPK10Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CBJPK10Rule::CBJPK10Rule(void)
: timespan_kj_shj(300)
, timespan_ye_kj_shj(300)
{
	m_iKjShjFirst=34200;
	m_iKjShjLast=85800;
	m_qishu=44;
	m_timespan=1200;
	//fenDanDuration = 30; //封单时间
	//m_gameKind=CZ_PK10;
	//strcpy(m_para1,"%s%02d");
    m_tStartTime = 1550885400;
	m_nStartQihao = 729920;
}

CBJPK10Rule::~CBJPK10Rule(void)
{

}
long CBJPK10Rule::GetFdShjDiff()
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CBJPK10Rule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao < GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 30; 
}

//下期期号
CString CBJPK10Rule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_PK10(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CBJPK10Rule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_PK10();
	CTime t=ct;
	return t;
}

//离下次开奖时间还剩下的时间-字符串描述
CString CBJPK10Rule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"BJPK10  %d",secDiff);
	OutputDebugString(strLog);


	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		secDiff+=nSecond;
	}

	int minute = 0;
	int second = 0;

	secDiff-=nSecond;
	minute = secDiff / 60 ;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}


/////////////////黑龙江11选5///////////////////////////

TCHAR CHLJ11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CHLJ11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CHLJ11X5Rule::CHLJ11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{
	m_iKjShjFirst=30300;
	m_iKjShjLast=81900;
	m_qishu=44;
	m_timespan=1200;
	//fenDanDuration = 50; //封单时间
	//m_gameKind=CZ_HLJ11Xuan5;
	strcpy(m_para1,"%s%02d");
}

CHLJ11X5Rule::~CHLJ11X5Rule(void)
{

}
long CHLJ11X5Rule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//是否可撤单-离开奖时间大于两分钟
bool CHLJ11X5Rule::IsCanCancel(CString qihao)
{
	//比下期旗号还要早，允许撤单
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//下期期号
CString CHLJ11X5Rule::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_TJ(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CHLJ11X5Rule::GetNextKjShj()
{
	time_t ct=GetNextKjShj_TJ();
	CTime t=ct;
	return t;
}

//离下次开奖时间还剩下的时间-字符串描述
CString CHLJ11X5Rule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();


	if (secDiff <= nSecond )		//|| secDiff > 540
	{
		secDiff+=nSecond;
	}

	int minute = 0;
	int second = 0;
	minute = secDiff / 60 - 1;
	second = secDiff % 60;

	CString str;
	str.Format(_T("%02d:%02d"), minute, second);

	return str;
}


CQiXingCaiRule::CQiXingCaiRule(void)
: timespan_kj_shj(600)
, timespan_fd_shj(300)
{

}

CQiXingCaiRule::~CQiXingCaiRule(void)
{

}
//下期期号
CString CQiXingCaiRule::GetNextExpect(int nDelta)
{
	//注意，本代码默认礼拜天为每周第一天，

	int nQiHao =0;
	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	CString strLog;
	strLog.Format(L"\n %d-%d-%d weekday:%d",tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetDayOfWeek());
	OutputDebugString(strLog);

	//礼拜
	int weekDay = tm.GetDayOfWeek();
	//一年中第几天
	CString strDay = tm.Format(_T("%j"));
	int nDay = _wtoi(strDay.GetBuffer(strDay.GetLength()));

	//本年第一天礼拜几（0-6）礼拜天-礼拜6
	int FirstWeekDay = weekDay - nDay%7+1;
	if (FirstWeekDay<0)FirstWeekDay+=7;

	//本年首礼拜开奖次数
	int firstWeekKaiJiangCount = 0;
	if (FirstWeekDay == 1)firstWeekKaiJiangCount = 3;//第一天礼拜天，本周开3次
	else if (FirstWeekDay <= 3) firstWeekKaiJiangCount = 2;//第一天礼拜2和礼拜1 开2次
	else if (FirstWeekDay <= 6) firstWeekKaiJiangCount = 1;//第一天礼拜3 4 5，开1次。

	//本周开奖次数
	int thisweekkaiJiangCount = 0;
	//假设未开奖
	if (weekDay >6 )thisweekkaiJiangCount = 3;//今天礼拜6，本周开了3次
	else if (weekDay >3 ) thisweekkaiJiangCount = 2;//今天礼拜 345  开了2次
	else if (weekDay >1 ) thisweekkaiJiangCount = 1;//12，开了1次。
	//判断今天是否已经开奖(每周2  5 天开奖）
	if (weekDay == 1|| weekDay == 3|| weekDay == 6)
	{
		//是否已过开奖时间,超过开奖时间加一期
		if (tm.GetHour()>19 || (tm.GetHour()==19&&tm.GetMinute()>=30))
		{
			thisweekkaiJiangCount+=1;
		}
	}

	//中间完整周开奖次数			总天数	第一个礼拜天数	 本礼拜天数	 过年7天不开奖
	int middleWeekKaiJiangCount = (nDay - (6-FirstWeekDay+1)-(weekDay+1)- 7	)/7*3;


	//本年度期号
	nQiHao=firstWeekKaiJiangCount+thisweekkaiJiangCount+middleWeekKaiJiangCount +1;




	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%s%03ld", tm.Format(L"%Y"), nQiHao);
	return strQiHao;

}

CTime CQiXingCaiRule::GetNextFdShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	int weekDay = t.GetDayOfWeek();
	//获取下一个开奖日（也许是今天)
	int OpenDay = weekDay;
	do
	{
		if (OpenDay ==1 || OpenDay==3 ||OpenDay==6)break;
		if (OpenDay ==7)OpenDay=1;
		OpenDay++;
	}while(true);

	if (OpenDay == weekDay)//当天开奖
	{
		if (t.GetHour()>19 ||(t.GetHour()==19&&t.GetMinute()>=30))
		{//已经封单 获取下一个开奖日
			if (OpenDay ==7)OpenDay=1;
			OpenDay++;
		}
	}

	//获取实际距离天数
	int NextDay = OpenDay-weekDay;
	if (NextDay<0)NextDay+=7;

	CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 19, 30, 0);
	ctm+= CTimeSpan(NextDay,0,0,0);
	return ctm;
}

CKuaiLe8RUle::CKuaiLe8RUle(void)
: timespan_kj_shj(300)
{
	m_iKjShjFirst=32700;
	m_iKjShjLast=86100;
	m_qishu=179;
	m_timespan=300;
	//fenDanDuration = 60; //封单时间
	//m_gameKind=CZ_PK10;
	//strcpy(m_para1,"%s%02d");
	//CTime _tStartTime = CTime(2019,2,23,9,5,0);
    m_tStartTime = 1550883900;
	m_nStartQihao = 937517;
}

CKuaiLe8RUle::~CKuaiLe8RUle(void)
{

}

//下期期号
CString CKuaiLe8RUle::GetNextExpect(int nDelta)
{
	string str=GetNextExpect_PK10(nDelta);
	CString rQh=CA2T(str.c_str());
	return rQh;
}

//下期开奖时间
CTime CKuaiLe8RUle::GetNextKjShj()
{
	time_t ct=GetNextKjShj_PK10();
	CTime t=ct;
	return t;
}
