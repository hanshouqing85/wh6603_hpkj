#ifndef NOTE_CARD_HEAD_FILE
#define NOTE_CARD_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////

//积分控件
class CNoteCard : public CVirtualWindow
{
	//变量定义
protected:
	BYTE							m_cbCardData[54];					//扑克数量	

	//灰度动画
protected:
	BYTE							m_cbAlphaIndex;						//透明索引
	CLapseCount						m_AlphaLapseCount;					//流逝计数

	//资源变量
protected:
	CD3DTexture						m_TextureNoteCard;					//记牌视图
	CD3DTexture						m_TextureMiniCard;					//扑克视图

	//资源变量
protected:
	CSize							m_SizeMiniCard;						//扑克大小

	//函数定义
public:
	//构造函数
	CNoteCard();
	//析构函数
	virtual ~CNoteCard();

	//系统事件
protected:
	//动画消息
	virtual VOID OnWindowMovie();
	//创建消息
	virtual VOID OnWindowCreate(CD3DDevice * pD3DDevice);
	//销毁消息
	virtual VOID OnWindowDestory(CD3DDevice * pD3DDevice);

	//重载函数
protected:
	//鼠标事件
	virtual VOID OnEventMouse(UINT uMessage, UINT nFlags, INT nXMousePos, INT nYMousePos);
	//按钮事件
	virtual VOID OnEventButton(UINT uButtonID, UINT uMessage, INT nXMousePos, INT nYMousePos);
	//绘画窗口
	virtual VOID OnEventDrawWindow(CD3DDevice * pD3DDevice, INT nXOriginPos, INT nYOriginPos);

	//功能函数
public:
	//隐藏窗口
	VOID CloseControl();
	//设置扑克
	VOID SetCardInfo();
	//设置扑克
	VOID SetCardInfo(BYTE cbCardData[], BYTE cbCardCount);
	//删除扑克
	VOID RemoveCard(BYTE cbCardData[], BYTE cbCardCount);
	// 绘画数字
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);

	//内部函数
private:
	//转换索引
	int SwitchToIndex(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////////////

#endif