// CollectDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
using namespace std;
// CCollectDlg �Ի���
class CCollectDlg : public CDialog
{
// ����
public:
	bool	ChangeStringToA(CString strInput, string&  strOutput);

	bool GetDataBaseString(TCHAR szAppName[30],TCHAR szKeyName[30],const TCHAR szName[],TCHAR szFileName[126],TCHAR szGetData[126],int nResult = 0);


};
