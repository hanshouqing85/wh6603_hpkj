#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CHtmlView��"
#endif 

// CWebBrowser Html ��ͼ

class CWebBrowserLogo : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowserLogo)

public:
	CWebBrowserLogo();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWebBrowserLogo();

public:
	void Navigate(LPCTSTR pszUrl);
	//������ҳ
	bool EnableBrowser(bool bEnableBrowser);

	CString m_strNavigateUrl;					//ҳ���ַ
	bool m_bEnableBrowser;					//�Ƿ�����

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//��ϢԤ����
	BOOL	PreTranslateMessage(MSG*	pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};


