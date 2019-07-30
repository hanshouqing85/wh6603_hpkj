#pragma once

#include "resource.h"

// CDlgTest dialog

class CDlgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgTest)

public:
	CDlgTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTest();

// Dialog Data
	enum { IDD = IDD_DIALOG_TE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
