#ifndef GOLD_VIEW_HEAD_FILE
#define GOLD_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//筹码视图类
class CGoldView
{
	//变量定义
public:
	__int64								m_lGold;						//筹码数目
	INT									m_nGoldIndex;					//

	//辅助变量
protected:
	static bool							m_bInit;						//初始标志
	static CBitImage					m_ImageGold;					//筹码图片
	CFont								ViewFont1;
	CFont								ViewFont2;
	//函数定义
public:
	//构造函数
	CGoldView();
	//析构函数
	virtual ~CGoldView();

	//功能函数
public:
	//设置筹码
	void SetGold(__int64 lGold);
	//获取筹码
	__int64 GetGold() { return m_lGold; };
	//绘画筹码
	void DrawGoldView(CWnd* pWnd, CDC * pDC, int nXPos, int nYPos,int iIndex=0);
	//设置绘画筹码索引
	VOID SetGoldIndex( INT nIndex );

	//内部函数
private:
	//整性变字符
	LPCTSTR GetGlodString(__int64 lGold, TCHAR szString[]);
};

//////////////////////////////////////////////////////////////////////////

#endif