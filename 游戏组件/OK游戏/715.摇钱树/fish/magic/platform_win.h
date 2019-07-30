//-----------------------------------------------------------------------------
// File: platform_win.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES_PLATFORM
#define MAGIC_PARTICLES_PLATFORM

#include <windows.h>
#include "mp.h"

class MP_Platform_WINDOWS : public MP_Platform
{
protected:
	WIN32_FIND_DATAW fd;
	HANDLE hFindFile;

public:
	MP_Platform_WINDOWS();
	virtual ~MP_Platform_WINDOWS();

	// eng: Returns time in milliseconds
	// rus: Возвращает время в милисекундах
	virtual unsigned long GetTick();

	// eng: Returns path to folder with emitters
	// rus: Возвращает путь к папке с ptc-файлами
	virtual const wchar_t* GetPathToPTC();

	// eng: Returns path to folder with textures
	// rus: Возвращает путь к папке с текстурами
	virtual const wchar_t* GetPathToTexture();

	// eng: Returns path to folder which could be used by wrapper to store temporary files
	// rus: Возвращает путь к временной папке
	virtual const wchar_t* GetPathToTemp();

	// eng: Finds the first ptc-file in emitters folder
	// rus: Поиск первого ptc-файла в папке с эмиттерами
	virtual const wchar_t* GetFirstFile();

	// eng: Finds the next ptc-file in emitters folder
	// rus: Поиск очередного ptc-файла в папке с эмиттерами
	virtual const wchar_t* GetNextFile();

	// eng: Converts wchar_t string into utf8
	// rus: Конвертирует строку типа wchar_t в строку типа utf8
	virtual const char* wchar_to_utf8(const wchar_t* str);

	// eng: Converts utf8 string into wchar_t
	// rus: Конвертирует строку типа utf8 в строку типа wchar_t
	virtual const wchar_t* utf8_to_wchar(const char* str);
};

#endif