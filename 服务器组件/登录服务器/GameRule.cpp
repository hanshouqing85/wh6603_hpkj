#include "StdAfx.h"

#include "GameRule.h"

CGameRule::CGameRule(void)
{

}

CGameRule::~CGameRule(void)
{

}

//���´ο���ʱ�仹ʣ�µ�ʱ��
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

//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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

//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CGameRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������

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

//ÿ��֮����ʱ�䣬��׼ʱ��
long CGameRule::GetQiSpan()
{
	return 300;
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

}

//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
long CChqSSCRule::GetQiSpan()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;
	int hour = ct.GetHour();
	if (hour >= m_t2_start && hour < m_t2_end)
		return timespan_kj_shj;
	else
		return timespan_ye_kj_shj;
}

//�����ں�
CString CChqSSCRule::GetNextExpect(int nDelta)
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;

	int hour = ct.GetHour();
	int qishu = 0;
	if (hour < m_t1_end)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t1_start, 0, 0);
		CTimeSpan tSpan = ct - t;
// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 280)
// 		{
// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 2;
// 		}
// 		else
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
		}
	}
	else if (hour >= m_t2_start && hour < m_t2_end)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t2_start, 0, 0);
		CTimeSpan tSpan = ct - t;
// 		if (tSpan.GetTotalSeconds() % timespan_kj_shj >= 580)
// 		{
// 			qishu = 24 + (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 2;
// 		}
// 		else
		{
			qishu = 24 + (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
		}
	}
	else if (hour >= m_t3_start)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t3_start, 0, 0);
		CTimeSpan tSpan = ct - t;
// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 280)
// 		{
// 			qishu = 96 + (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj)+2;
// 		}
// 		else
		{
			qishu = 96 + (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
		}

		if(qishu > 120)
		{
			ct+=CTimeSpan(0,0,0,20);

			CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t1_start, 0, 0);
			CTimeSpan tSpan = ct - t;
			if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 280)
			{
				qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 2;
			}
			else
			{
				qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
			}

		}
	}
	else
	{
		qishu = 24;
	}
	//��������
	qishu += nDelta;
	CString tmp = ct.Format(_T("%Y%m%d"));
	CString rQh;
	rQh.Format(_T("%s%03d"), tmp, qishu);
	return rQh;
}

//���ڿ���ʱ��
CTime CChqSSCRule::GetNextKjShj()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	int hour = ct.GetHour();
	if (hour < m_t1_end)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t1_start, 0, 0);
		CTimeSpan tSpan = ct - t;
		int qishu = 0;

// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 280)
// 		{
// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj)+2;
// 		}
// 		else
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
		}

		t += CTimeSpan(qishu * timespan_ye_kj_shj);
		return t;
	}
	else if (hour >= m_t2_start && hour < m_t2_end)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t2_start, 0, 0);
		CTimeSpan tSpan = ct - t;
		int qishu = 0;


		//if (tSpan.GetTotalSeconds() % timespan_kj_shj >= 580)
		//{
		//	qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+2;
		//}
		//else
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
		}
		t += CTimeSpan(qishu * timespan_kj_shj);
		return t;
	}
	else if (hour >= m_t3_start)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t3_start, 0, 0);
		CTimeSpan tSpan = ct - t;
		int qishu = 0;

// 		if (tSpan.GetTotalSeconds() % timespan_ye_kj_shj >= 280)
// 		{
// 			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj)+2;
// 		}
// 		else
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_ye_kj_shj) + 1;
		}
		t += CTimeSpan(qishu * timespan_ye_kj_shj);
		return t;
	}
	else
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), m_t2_start-1, 50, 0);
		t += CTimeSpan(timespan_kj_shj);
		return t;
	}
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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

//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CChqSSCRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������

	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}
	CString strLog;
	strLog.Format(L"CHQSSC Cancel:%d",GetKjShjDiff());
	OutputDebugString(strLog);

	return GetKjShjDiff() > 45; 
}

TCHAR CJxSSCRule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CJxSSCRule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CJxSSCRule::CJxSSCRule(void)
: timespan_fd_shj(60*9+15)
, timespan_kj_shj(600)
{

}

CJxSSCRule::~CJxSSCRule(void)
{

}

//�����ں�
CString CJxSSCRule::GetNextExpect(int nDelta)
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if ((t.GetHour() >= 9 )	&& (t.GetHour() < 23 )	)
	{
		CTime ct(t.GetYear(), t.GetMonth(), t.GetDay(), 8, 59, 20);
		CTimeSpan tSpan = t - ct;
		int nQiHao = 0;

		if (tSpan.GetTotalSeconds() % timespan_kj_shj >= 580)
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 2;
		}
		else
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
		}

		CString tmp = t.Format(_T("%Y%m%d"));
			nQiHao += nDelta;
		rQh.Format(_T("%s%03d"), tmp, nQiHao);


	}
	else if (t.GetHour() >= 23) {
		CTime mt = t + CTimeSpan(1, 0, 0, 0);
		rQh.Format(_T("%d%02d%02d001"), mt.GetYear(), mt.GetMonth(), mt.GetDay());
	}
	else {
//		t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d001"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

CTime CJxSSCRule::GetNextFdShj()
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;
	CString rQh;
	if ((ct.GetHour() >= 9 )	&& (ct.GetHour() < 23 )	)
	{
		CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 8, 59, 20);
		CTimeSpan tSpan = ct - t;
		int qishu = 0;

		CString strLog;
		strLog.Format(L"TJSSC sec:%d",tSpan.GetTotalSeconds() % timespan_kj_shj);
		OutputDebugString(strLog);
		if (tSpan.GetTotalSeconds() % timespan_kj_shj >= 580)
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj)+2;
		}
		else
		{
			qishu = (int)(tSpan.GetTotalSeconds() / timespan_kj_shj) + 1;
		}
		t += CTimeSpan(qishu * timespan_kj_shj);
		return t;
	}
	else if (ct.GetHour() >= 23) {
		CTime mt = ct + CTimeSpan(1, 0, 0, 0);
		CTime ctm(mt.GetYear(), mt.GetMonth(), mt.GetDay(), 8, 59, 20);
		//t += CTimeSpan(timespan_fd_shj);
		return ctm;
	}
	else {
//		t+=CTimeSpan(1L,0,0,0);
		CTime ctm(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 8, 59, 20);
		//t += CTimeSpan(timespan_fd_shj);
		return ctm;
	}
}

long CJxSSCRule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CTimeSpan span = GetNextFdShj() - t;

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


//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CJxSSCRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;
	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 200; 
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
CString CJxSSCRule::GetKjShjDiffDesc(int nSecond)
{
	long secDiff = GetKjShjDiff();

	CString strLog;
	strLog.Format(L"TJSSC secDiff:%d",secDiff);
	OutputDebugString(strLog);

	if(secDiff >= 200)
	{
		secDiff -= 180;
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

}

CXJSSCRule::~CXJSSCRule(void)
{

}

//�����ں�
CString CXJSSCRule::GetNextExpect(int nDelta)
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 10 && t.GetMinute()>=10)||(t.GetHour()>10)) && t.GetHour() <= 24)
	{
		CTime ct(t.GetYear(), t.GetMonth(), t.GetDay(), 10, 0, 0);
		CTimeSpan tSpan = t - ct;
		int nQiHao = 0;

		if (tSpan.GetTotalSeconds() % timespan_fd_shj >= 580)
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 2;
		}
		else
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 1;
		}

		CString tmp = t.Format(_T("%Y%m%d"));

		rQh.Format(_T("%s%02d"), tmp, nQiHao);

	}
	else if (t.GetHour() >= 0 && t.GetHour() < 2)
	{
		CTime ct(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);

		CTimeSpan tSpan = t - ct;
		int nQiHao = 0;

		if (tSpan.GetTotalSeconds() % timespan_fd_shj >= 580)
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 2;
		}
		else
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 1;
		}

		nQiHao+=84;
		t -= CTimeSpan(1,0,0,0);
		CString tmp = t.Format(_T("%Y%m%d"));

		rQh.Format(_T("%s%02d"), tmp, nQiHao);
	}
	else {
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

CTime CXJSSCRule::GetNextFdShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 10 && t.GetMinute()>=10)||(t.GetHour()>10)) && t.GetHour() <= 24)
	{
		CTime ct(t.GetYear(), t.GetMonth(), t.GetDay(), 10, 0, 0);
		CTimeSpan tSpan = t - ct;
		int nQiHao = 0;

		if (tSpan.GetTotalSeconds() % timespan_fd_shj >= 580)
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 2;
		}
		else
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 1;
		}

		ct += CTimeSpan(nQiHao * timespan_fd_shj);
		return ct;
	}
	else if ((t.GetHour() >= 10 && t.GetHour() <= 24) || (t.GetHour() >= 0 && t.GetHour() <= 2)){
		CTime ct(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);
		CTimeSpan tSpan = t - ct;
		int nQiHao = 0;

		if (tSpan.GetTotalSeconds() % timespan_fd_shj >= 580)
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 2;
		}
		else
		{
			nQiHao = (int)(tSpan.GetTotalSeconds() / timespan_fd_shj) + 1;
		}


		ct += CTimeSpan(nQiHao * timespan_fd_shj);

		CString strLog;
		strLog.Format(L"KAIJIANG %d-%d-%d %d:%d:%d",ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
		OutputDebugString(strLog);
		return ct;
	}
	else {
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 10, 10, 0);
		return ctm;
	}
}

long CXJSSCRule::GetFdShjDiff()
{

	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CTimeSpan span = GetNextFdShj() - t;

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

//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CXJSSCRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
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

//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
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
//�����ں�
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
//   	//��������
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CFenFenCaiRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 5; 
}

//���ڿ���ʱ��
CTime CFenFenCaiRule::GetNextKjShj()
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

//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
long CWuFenCaiRule::GetQiSpan()
{
	return timespan_kj_shj;
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CWuFenCaiRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 45; 
}

//�����ں�
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
	   	//��������
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

//���ڿ���ʱ��
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

}

CGD11X5Rule::~CGD11X5Rule(void)
{

}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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

//�����ں�
CString CGD11X5Rule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CString rQh;
	int nQihao = 1;
	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour() > 9) && (t.GetHour() < 23 ||(t.GetHour() == 23&& t.GetMinute()<1)) ){
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),9,0,0);
		CTimeSpan tSpan = t-starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();

// 		if(nTotalSeconds%timespan_kj_shj >= 540)
// 			nQihao = tSpan.GetTotalMinutes()/10+2;
// 		else
			nQihao = tSpan.GetTotalMinutes()/10+1;
			nQihao += nDelta;


		rQh.Format(L"%d%02d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay(),nQihao);
	}
	else {
		if((t.GetHour() == 22&& t.GetMinute()>55) || t.GetHour() >=23)
			t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

//���ڿ���ʱ��
CTime CGD11X5Rule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour() > 9) && (t.GetHour() < 23 ||(t.GetHour() == 23&& t.GetMinute()<1)) ){
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),9,0,0);
		CTimeSpan tSpan = t-starttime;
		int nTotalSeconds = tSpan.GetTotalSeconds();
		int nQihao = 1;

// 		if(nTotalSeconds%600 >= 540)
// 			nQihao = tSpan.GetTotalMinutes()/10+2;
// 		else
			nQihao = tSpan.GetTotalMinutes()/10+1;

		starttime += CTimeSpan(nQihao * timespan_kj_shj);

		return starttime;
	}
	else {
		if((t.GetHour() == 22&& t.GetMinute()>55) || t.GetHour() >=23)
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 10, 0);
		return ctm;
	}
}
long CGD11X5Rule::GetFdShjDiff()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CTimeSpan span = GetNextKjShj() - t;

	return (long)span.GetTotalSeconds();
}
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CGD11X5Rule::IsCanCancel(CString qihao)
{
	CString strLog;
	strLog.Format(L"CGD11X5Rule qihao:%s,nextqh:%s,fdshj:%d",qihao,GetNextExpect(),GetFdShjDiff());
	OutputDebugString(strLog);
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

//////////////����1.5�ֲ�/////////////////////////////////
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
	m_nStartQihao = 1588749;



}
CHgydwfcRule::~CHgydwfcRule(void)
{

}

//��ȡÿ��ʱ����,������ʱʱ�ʣ���ʱ��5���ӣ���ʱ��10���ӡ�
long CHgydwfcRule::GetQiSpan()
{
	return timespan_kj_shj;
}

//�����ں�
CString CHgydwfcRule::GetNextExpect(int nDelta)
{
	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);


	int hour = ct.GetHour();


	CTimeSpan tSpan = ct - m_tStartTime;
	int nDays = tSpan.GetDays();
	int nQihao = m_nStartQihao + (tSpan.GetDays()*880);

	if (hour<m_t1_end)
	{
		nQihao += ((tSpan.GetTotalSeconds() / 90) + 1);
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

	//��������
	nQihao += nDelta;
	//CString tmp = ct.Format(_T("%Y%m%d"));
	CString rQh;
	rQh.Format(_T("%d"), nQihao);
	return rQh;
}

//���ڿ���ʱ��
CTime CHgydwfcRule::GetNextKjShj()
{

	CTime ct= CTime::GetCurrentTime();
	ct+=m_timeSpan;


	CTime t(ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0);


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
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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

//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CHgydwfcRule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������

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

//�����ں�
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
		//��������
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

//���ڿ���ʱ��
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CCQ11X5Rule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CJX11X5Rule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}

//�����ں�
CString CJX11X5Rule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;

	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour()>=10) && (t.GetHour() < 22) ){

		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),9,0,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();

// 		if(nTotalSeconds%timespan_kj_shj >= 540)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}
		qishu+=nDelta;
		rQh.Format(_T("%d%02d%02d%02d"), t.GetYear(),t.GetMonth(),t.GetDay(), qishu);
	}
	else {
		if(t.GetHour() >=22 && t.GetHour() <=24)
			t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

//���ڿ���ʱ��
CTime CJX11X5Rule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=10)||t.GetHour()>=10) && (t.GetHour() < 22) ){
		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),9,0,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();
// 		if(nTotalSeconds%timespan_kj_shj >= 540)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}

		starttime += CTimeSpan(qishu*timespan_kj_shj);
		return starttime;

	}
	else {
		if(t.GetHour() >=22 && t.GetHour() <=24)
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 10, 0);
		return ctm;
	}
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CSD11X5Rule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//�����ں�
CString CSD11X5Rule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=5)||t.GetHour()>9) && (t.GetHour() < 21 || (t.GetHour() == 21 && t.GetMinute()<55))){

		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),8,55,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();

// 		if(nTotalSeconds%timespan_kj_shj >= 540)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}

		qishu+=nDelta;

		rQh.Format(_T("%d%02d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay(),qishu);
	}
	else {
		if(t.GetHour() >= 22 ||((t.GetHour() == 21 && t.GetMinute()>55)))
			t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

//���ڿ���ʱ��
CTime CSD11X5Rule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 9 && t.GetMinute()>=5)||t.GetHour()>9) && (t.GetHour() < 21 || (t.GetHour() == 21 && t.GetMinute()<55)) ){

		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),8,55,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();

// 		if(nTotalSeconds%timespan_kj_shj >= 540)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}
		starttime += CTimeSpan(qishu*timespan_kj_shj);
		return starttime;
	}
	else {
		if(t.GetHour() >= 22 ||((t.GetHour() == 21 && t.GetMinute()>55)))
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 5, 0);
		return ctm;
	}
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CBJPK10Rule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 30; 
}
//�����ں�
CString CBJPK10Rule::GetNextExpect(int nDelta)
{

	CTime startTime = CTime(2016,3,3,0,0,0);
	int startqihao = 540746;
	int nQiHao =0;
	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	//һ���еڼ���
	CString strDay = tm.Format(_T("%j"));
	int nDay = _wtoi(strDay.GetBuffer(strDay.GetLength()));

	int TodayQihao =1;
	if ( (tm.GetHour()>9)&&(tm.GetHour()<23)  || (tm.GetHour()==23)&&(tm.GetMinute()<57)||(tm.GetHour()==9)&&(tm.GetMinute()>=7))
	{
		CTime daystart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,7,30);
		CTimeSpan tCha = tm - daystart;


// 		if(tCha.GetTotalSeconds()%300 > 240)
// 			TodayQihao = tCha.GetTotalMinutes()/5+2;
// 		else
			TodayQihao = tCha.GetTotalMinutes()/5+1;
	}
	else if((tm.GetHour()==23)&&(tm.GetMinute()>=57))
	{
		tm += CTimeSpan(1,0,0,0);
	}
	else
	{
		startqihao-=1;
	}

	CTimeSpan temp = tm-startTime;

	nQiHao = startqihao + temp.GetDays()*179 +TodayQihao;



	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%ld",  nQiHao);
	return strQiHao;

}

//���ڿ���ʱ��
CTime CBJPK10Rule::GetNextKjShj()
{
	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;
	CString rQh;

	if ( (tm.GetHour()>9)&&(tm.GetHour()<23)  || (tm.GetHour()==23)&&(tm.GetMinute()<57)||(tm.GetHour()==9)&&(tm.GetMinute()>=7))
	{
		int TodayQihao =1;

		CTime daystart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,7,30);
		CTimeSpan tCha = tm - daystart;


// 		if(tCha.GetTotalSeconds()%300 > 240)
// 			TodayQihao = tCha.GetTotalMinutes()/5+2;
// 		else
			TodayQihao = tCha.GetTotalMinutes()/5+1;

		CTime NextKj = daystart;

		NextKj += CTimeSpan(0,0,TodayQihao*5,0);
		
		return NextKj;
	}
	else if((tm.GetHour()<9)||(tm.GetHour() == 9 && tm.GetMinute()<7))
	{
		CTime daystart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,6,30);
		return daystart;
	}	
	else
	{
		CTime daystart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,6,30);
		daystart += CTimeSpan(1,0,0,0);
		return daystart;
	}

	return tm;

}

//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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


/////////////////������11ѡ5///////////////////////////

TCHAR CHLJ11X5Rule::m_lastExpect[KJ_QIHAO_LENGTH] = _T("");
TCHAR CHLJ11X5Rule::m_lastKjShj[KJ_SHIJIAN_LENGTH] = _T("");

CHLJ11X5Rule::CHLJ11X5Rule(void)
: timespan_kj_shj(600)
, timespan_ye_kj_shj(300)
{

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
//�Ƿ�ɳ���-�뿪��ʱ�����������
bool CHLJ11X5Rule::IsCanCancel(CString qihao)
{
	//��������Ż�Ҫ�磬������
	if(qihao > GetNextExpect())
		return true;

	if(qihao != GetNextExpect())
	{
		return false;
	}

	return GetFdShjDiff() > 60; 
}
//�����ں�
CString CHLJ11X5Rule::GetNextExpect(int nDelta)
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;

	CString rQh;
	if (((t.GetHour() == 8 && t.GetMinute()>=5)||t.GetHour()>8) && (t.GetHour() < 22 || (t.GetHour() == 22 && t.GetMinute()<5))){

		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),7,55,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();
// 
// 		if(nTotalSeconds%timespan_kj_shj >= 580)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}
		qishu+= nDelta;
		rQh.Format(_T("%d%02d%02d%02d"),t.GetYear(),t.GetMonth(),t.GetDay(),qishu);
	}
	else {
		if(t.GetHour() > 22 ||((t.GetHour() == 22 && t.GetMinute()>=25)))
			t+=CTimeSpan(1L,0,0,0);
		rQh.Format(_T("%d%02d%02d01"), t.GetYear(), t.GetMonth(), t.GetDay());
	}

	return rQh;
}

//���ڿ���ʱ��
CTime CHLJ11X5Rule::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if (((t.GetHour() == 8 && t.GetMinute()>=5)||t.GetHour()>9) && (t.GetHour() < 22 || (t.GetHour() == 22 && t.GetMinute()<25)) ){

		int qishu = 1;
		CTime starttime(t.GetYear(),t.GetMonth(),t.GetDay(),7,55,0);
		CTimeSpan tSpan = t - starttime;

		int nTotalSeconds = tSpan.GetTotalSeconds();

		CString strLog;
		strLog.Format(L"HLJ11X5 %02d:%02d:%02d yu:%d",t.GetHour(),t.GetMinute(),t.GetSecond(),nTotalSeconds%timespan_kj_shj);
		OutputDebugString(strLog);
// 		if(nTotalSeconds%timespan_kj_shj >= 580)
// 		{
// 			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+2;
// 		}
// 		else
		{
			qishu = tSpan.GetTotalSeconds()/timespan_kj_shj+1;
		}
		starttime += CTimeSpan(qishu*timespan_kj_shj);
		return starttime;
	}
	else {
		if(t.GetHour() > 22 ||((t.GetHour() == 22 && t.GetMinute()>=25)))
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 8, 5, 0);
		return ctm;
	}
}
//���´ο���ʱ�仹ʣ�µ�ʱ��-�ַ�������
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
//�����ں�
CString CQiXingCaiRule::GetNextExpect(int nDelta)
{
	//ע�⣬������Ĭ�������Ϊÿ�ܵ�һ�죬

	int nQiHao =0;
	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	CString strLog;
	strLog.Format(L"\n %d-%d-%d weekday:%d",tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetDayOfWeek());
	OutputDebugString(strLog);

	//���
	int weekDay = tm.GetDayOfWeek();
	//һ���еڼ���
	CString strDay = tm.Format(_T("%j"));
	int nDay = _wtoi(strDay.GetBuffer(strDay.GetLength()));

	//�����һ����ݼ���0-6�������-���6
	int FirstWeekDay = weekDay - nDay%7+1;
	if (FirstWeekDay<0)FirstWeekDay+=7;

	//��������ݿ�������
	int firstWeekKaiJiangCount = 0;
	if (FirstWeekDay == 1)firstWeekKaiJiangCount = 3;//��һ������죬���ܿ�3��
	else if (FirstWeekDay <= 3) firstWeekKaiJiangCount = 2;//��һ�����2�����1 ��2��
	else if (FirstWeekDay <= 6) firstWeekKaiJiangCount = 1;//��һ�����3 4 5����1�Ρ�

	//���ܿ�������
	int thisweekkaiJiangCount = 0;
	//����δ����
	if (weekDay >6 )thisweekkaiJiangCount = 3;//�������6�����ܿ���3��
	else if (weekDay >3 ) thisweekkaiJiangCount = 2;//������� 345  ����2��
	else if (weekDay >1 ) thisweekkaiJiangCount = 1;//12������1�Ρ�
	//�жϽ����Ƿ��Ѿ�����(ÿ��2  5 �쿪����
	if (weekDay == 1|| weekDay == 3|| weekDay == 6)
	{
		//�Ƿ��ѹ�����ʱ��,��������ʱ���һ��
		if (tm.GetHour()>19 || (tm.GetHour()==19&&tm.GetMinute()>=30))
		{
			thisweekkaiJiangCount+=1;
		}
	}

	//�м������ܿ�������			������	��һ���������	 ���������	 ����7�첻����
	int middleWeekKaiJiangCount = (nDay - (6-FirstWeekDay+1)-(weekDay+1)- 7	)/7*3;


	//������ں�
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
	//��ȡ��һ�������գ�Ҳ���ǽ���)
	int OpenDay = weekDay;
	do
	{
		if (OpenDay ==1 || OpenDay==3 ||OpenDay==6)break;
		if (OpenDay ==7)OpenDay=1;
		OpenDay++;
	}while(true);

	if (OpenDay == weekDay)//���쿪��
	{
		if (t.GetHour()>19 ||(t.GetHour()==19&&t.GetMinute()>=30))
		{//�Ѿ��ⵥ ��ȡ��һ��������
			if (OpenDay ==7)OpenDay=1;
			OpenDay++;
		}
	}

	//��ȡʵ�ʾ�������
	int NextDay = OpenDay-weekDay;
	if (NextDay<0)NextDay+=7;

	CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 19, 30, 0);
	ctm+= CTimeSpan(NextDay,0,0,0);
	return ctm;
}

CKuaiLe8RUle::CKuaiLe8RUle(void)
: timespan_kj_shj(300)
{
	startqihao = 746175;
	startTime = CTime(2016,3,3,9,5,0);
}

CKuaiLe8RUle::~CKuaiLe8RUle(void)
{


}




//�����ں�
CString CKuaiLe8RUle::GetNextExpect(int nDelta)
{
	int nQiHao =0;
	CTime tm= CTime::GetCurrentTime();
	tm+=m_timeSpan;

	//һ���еڼ���
	CString strDay = tm.Format(_T("%j"));
	int nDay = _wtoi(strDay.GetBuffer(strDay.GetLength()));

	int TodayQihao =1;
	if ( (tm.GetHour()>=9)&&(tm.GetHour()<23)  || (tm.GetHour()==23)&&(tm.GetMinute()<55))
	{
		CTime ctm(tm.GetYear(), tm.GetMonth(), tm.GetDay(), 9, 5, 0);

		CTimeSpan tCha = tm-ctm;

		if(tCha.GetTotalSeconds()%300 > 240)
			TodayQihao = tCha.GetTotalMinutes()/5+2;
		else
			TodayQihao = tCha.GetTotalMinutes()/5+1;
	}
	else //if((tm.GetHour()==23)&&(tm.GetMinute()>55))
	{
		if((tm.GetHour()==23)&&(tm.GetMinute()>55))
			tm += CTimeSpan(1,0,0,0);
		CTime t(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,5,0);
		tm = t;
		TodayQihao = 0;
	}

	CTimeSpan temp = tm-startTime;

	nQiHao = startqihao + temp.GetDays()*179 +TodayQihao;



	nQiHao+=nDelta;
	CString strQiHao;
	strQiHao.Format(L"%ld",  nQiHao);
	return strQiHao;

}



//���ڿ���ʱ��
CTime CKuaiLe8RUle::GetNextKjShj()
{
	CTime t= CTime::GetCurrentTime();
	t+=m_timeSpan;
	CString rQh;
	if ((t.GetHour() >= 9  && t.GetHour()<=23 )  ||  (t.GetHour() == 23 && t.GetMinute()<55))
	{
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 0, 0);

		CTimeSpan tCha = t-ctm;
		int TodayQihao=1;
		if(tCha.GetTotalSeconds()%300 > 240)
			TodayQihao = tCha.GetTotalMinutes()/5+2;
		else
			TodayQihao = tCha.GetTotalMinutes()/5+1;

 		CTime NextKj = ctm;
 
		NextKj += CTimeSpan(0,0,TodayQihao*5,0);

		return NextKj;
	}
	else 
	{
		if(t.GetHour() == 23 && t.GetMinute()>=55)
			t+=CTimeSpan(1L,0,0,0);
		CTime ctm(t.GetYear(), t.GetMonth(), t.GetDay(), 9, 5, 0);
		return ctm;
	}
}
