#pragma once  

/*  
Title:�ı�MFC CCheckBox�ؼ�Ĭ���������ɫ  
Test Environment: VS2013Update3  
Author: kagula  
[cpp] view plaincopy��CODE�ϲ鿴����Ƭ�������ҵĴ���Ƭ
LastUpdateDate:2014-10-20  
*/  
class CMyCheckBox : public CButton  
{  
	DECLARE_DYNAMIC(CMyCheckBox)  

public:  
	CMyCheckBox();  
	virtual ~CMyCheckBox();  

protected:  
	DECLARE_MESSAGE_MAP()  
public:  
	COLORREF m_clrFore;  
	COLORREF m_clrBK;  
	int      m_fontSize;  
	afx_msg void OnPaint();  
}; 