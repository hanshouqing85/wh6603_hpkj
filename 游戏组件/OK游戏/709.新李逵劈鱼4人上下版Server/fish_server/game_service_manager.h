
#ifndef GAME_SERVICE_MANAGER_H_
#define GAME_SERVICE_MANAGER_H_
#pragma once

#include "FishDataBaseEngine.h"

class GameServiceManager : public IGameServiceManager, public IGameServiceCustomRule
{
 public:
  GameServiceManager();
  virtual ~GameServiceManager();

  virtual void Release();
  virtual void* QueryInterface(REFGUID guid, DWORD query_ver);

  virtual void* CreateTableFrameSink(REFGUID guid, DWORD query_ver);
  virtual void* CreateAndroidUserItemSink(REFGUID guid, DWORD query_ver);
  virtual void* CreateGameDataBaseEngineSink(REFGUID guid, DWORD query_ver);

  virtual bool GetServiceAttrib(tagGameServiceAttrib& game_service_attrib);
  virtual bool RectifyParameter(tagGameServiceOption& game_service_option);
public:
	//默认配置
	virtual bool DefaultCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize);
	//获取配置
	virtual bool SaveCustomRule(LPBYTE pcbCustomRule, WORD wCustonSize);
	//创建窗口
	virtual HWND CreateCustomRule(CWnd * pParentWnd, CRect rcCreate, LPBYTE pcbCustomRule, WORD wCustonSize);

 private:
  tagGameServiceAttrib game_service_attrib_;
  tagGameServiceOption  game_service_option_;
};

#endif // GAME_SERVICE_MANAGER_H_
