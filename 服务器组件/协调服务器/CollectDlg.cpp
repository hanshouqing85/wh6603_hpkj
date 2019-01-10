// CollectDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "CollectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <ctime>
#include <cstdlib>

char* substr(const char* _str,int pos,int len)
{
	static char ptr[10];
 	memcpy(ptr,_str+pos-1,len);
 	ptr[len] = '\0';

	return ptr;
}

string longnum(const char * src,const char *key)
{
	int KeyPos = -1;
	int SrcPos = 0;
	int SrcAsc = 0;

	time_t t;

	int KeyLen = strlen(key);
	if(KeyLen == 0)
		return "";

	srand((unsigned )time(&t));
	int offset = 100;//rand()%255;

 	char buff[3];

	sprintf(buff,"%1.2x",offset);

	string dest = buff;

	for(int i = 0;i < strlen(src);i++)
	{
		SrcAsc = (src[i] + offset)%255;

		if(KeyPos < KeyLen-1)
			KeyPos++;
		else
			KeyPos = 0;

		SrcAsc = SrcAsc^key[KeyPos];

		memset(buff,0,sizeof(buff));
		sprintf(buff,"%1.2x",SrcAsc);

		dest += (string)buff;

		offset = SrcAsc;
	}
	return dest;
}

string longnumdesc(const char* src,const char* key)
{
	int KeyLen = strlen(key);
	if(KeyLen == 0)
		return "";

	int KeyPos = -1;
	int offset = 0;
 	string dest = "";
	int SrcPos = 0;
	int SrcAsc = 0;
	int TmpSrcAsc = 0;

	char buff[5];
	sprintf(buff,"0x%s",substr(src,1,2));
	sscanf(buff, "%x", &offset);
	SrcPos = 3;
	while(SrcPos <= strlen(src))
	{
		sprintf(buff, "0x%s", substr(src, SrcPos, 2));
		sscanf(buff, "%x", &SrcAsc);         
		if (KeyPos < KeyLen-1)            
			KeyPos++;         
		else              
			KeyPos = 0;           
		TmpSrcAsc = SrcAsc ^ key[KeyPos];
		if (TmpSrcAsc <= offset)
			TmpSrcAsc = 255 + TmpSrcAsc - offset;
		else
			TmpSrcAsc = TmpSrcAsc - offset;
		dest += char(TmpSrcAsc);
		offset = SrcAsc; 
		SrcPos = SrcPos + 2;
	}
	return dest;

}
//////////////////////////////////////////////////////////////////////////////////


bool	CCollectDlg::ChangeStringToA(CString strInput, string&  strOutput)
{
	int len=WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,NULL,0,NULL,NULL);
	char *str=new char[len];
	memset(str,0,len);
	WideCharToMultiByte(CP_ACP,0,LPCWSTR(strInput),-1,str,len,NULL,NULL);
	strOutput=str;
	delete[] str;
	return true;
}

bool CCollectDlg::GetDataBaseString(TCHAR szAppName[30],TCHAR szKeyName[30],const TCHAR szName[],TCHAR szFileName[126],TCHAR szGetData[126],int nResult)
{
	TCHAR szContent[126];
	CString strLog;
	string str;
	GetPrivateProfileString(szAppName,szKeyName,szName,szContent,sizeof(szContent),szFileName);
	strLog.Format(L"%s",szContent);

	if(nResult == 1)
	{
		_sntprintf(szGetData,126,TEXT("%s"),szContent);
		return true;
	}
	ChangeStringToA(strLog,str);
	char s[25] = "0x258654";

	string strJiami = longnumdesc(str.c_str(),s);

	CString  strTemp(strJiami.c_str()) ;
	//	strTemp.Format(L"%s",strJiami.c_str());

	_sntprintf(szGetData,126,TEXT("%s"),strTemp);

	return true;
}
