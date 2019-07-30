#ifndef DOUBLE_CONTROL_HEAD_FILE
#define DOUBLE_CONTROL_HEAD_FILE

#pragma once
#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//倍数信息
struct tagDoubleInfo
{
	//公共位数
	WORD							wStartTime;								//初始倍数
	WORD							wValidCardTime;							//明牌倍数
	WORD							wRodBankerTime;							//抢地主倍数
	WORD							wBackCardTime;							//底牌倍数
	WORD							wBombTime;								//炸弹倍数
	WORD							wChunTianbTime;							//春天倍数
	WORD							wCardTypeTime;							//牌型倍数
	
	//加倍过程
	WORD							wUserDouble[GAME_PLAYER];				//用户加倍

	//用户信息
	WORD							wBankerUser;							//庄家用户
	WORD							wMeChairID;								//玩家自己
};

//////////////////////////////////////////////////////////////////////////////////

//倍数控件
class CDoubleControl : public CVirtualWindow
{
	//变量定义
protected:
	tagDoubleInfo					m_DoubleInfo;							//倍数信息
	TCHAR							m_szNickName[GAME_PLAYER][LEN_NICKNAME];//用户昵称
	WORD							m_wTotalTime;							//公共倍数
	WORD							m_wAddDoubleTime;						//加倍倍数

	//灰度动画
protected:
	BYTE							m_cbAlphaIndex;						//透明索引
	CLapseCount						m_AlphaLapseCount;					//流逝计数

	//资源变量
protected:
	CD3DFont						m_Font;								//倍数字体
	CD3DTexture						m_TextureGameDouble;				//倍数视图
	CD3DTexture						m_TextureBaseNumber;				//基础数字
	CD3DTexture						m_TextureProcessNumber;				//加倍数字
	CD3DTexture						m_TexturePlayersNumber;				//玩家数字

	//函数定义
public:
	//构造函数
	CDoubleControl();
	//析构函数
	virtual ~CDoubleControl();

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
	// 绘画数字
	void DrawNumber(CD3DDevice * pD3DDevice, CD3DTexture* ImageNumber, TCHAR * szImageNum, SCORE lOutNum, INT nXPos, INT nYPos, UINT uFormat = DT_LEFT);

	//功能函数
public:
	//隐藏窗口
	VOID CloseControl();
	//设置位数
	VOID SetDoubleInfo();
	//设置位数
	VOID SetDoubleInfo(tagDoubleInfo &DoubleInfo);

};

//////////////////////////////////////////////////////////////////////////////////

#endif