// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Dsfish.pch ����Ԥ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


TCHAR g_szWorkDir[MAX_PATH];

//�����Ϣ
void TraceString(LPCTSTR pszMessage)
{
	try
	{
		//�����ļ���
		//���ļ�
		CFile FileRecord;
		if (FileRecord.Open(L"fish.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(pszMessage,lstrlen(pszMessage));
			FileRecord.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));
			FileRecord.Close();
		}
	}
	catch(...){}
}

