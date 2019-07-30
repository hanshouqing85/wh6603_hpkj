// stdafx.cpp : 只包括标准包含文件的源文件
// Dsfish.pch 将是预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


TCHAR g_szWorkDir[MAX_PATH];

//输出信息
void TraceString(LPCTSTR pszMessage)
{
	try
	{
		//设置文件名
		//打开文件
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

