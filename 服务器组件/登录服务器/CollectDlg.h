// CollectDlg.h : 头文件
//

#pragma once

#include <string>
using namespace std;
// CCollectDlg 对话框
class CCollectDlg : public CDialog
{
// 构造
public:
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	bool GetDataBaseString(TCHAR szAppName[30],TCHAR szKeyName[30],const TCHAR szName[],TCHAR szFileName[126],TCHAR szGetData[126],int nResult = 0);


};
