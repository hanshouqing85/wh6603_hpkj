//-----------------------------------------------------------------------------
// File: mp_wrap.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES_DX
#define MAGIC_PARTICLES_DX

#include "mp.h"
#include <d3d9.h>
#include <d3dx9tex.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "magic.h"

#define MAX_SEGMENT	4
#define SIZE_SEGMENT 512

#ifdef MAGIC_3D
#define FVF_PARTICLE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#else
#define FVF_PARTICLE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#endif

struct VERTEX_PARTICLE
{
	float x, y, z;		// eng: coordinates
						// rus: координаты
	#ifndef MAGIC_3D
	float rhw;
	#endif

	unsigned long color;// eng: color
						// rus: цвет

	float u, v;			// eng: textural coordinates
						// rus: текстурные координаты
};

// eng: Class storing the textural atlas
// rus: Класс, который хранит текстурный атлас
class MP_Atlas_WRAP : public MP_Atlas
{
protected:
	LPDIRECT3DTEXTURE9 texture;

protected:
	static int now_segment;				// rus: текущий сегмент буфера вершин
	static int k_particles;				// rus: количество частиц загруженных в сегмент
	static VERTEX_PARTICLE* vertex;		// rus: указатель на очередную вершину в сегменте

public:
	MP_Atlas_WRAP(int width, int height, const char* file);

	// eng: Destroy atlas texture
	// rus: Уничтожить текстуру атласа
	virtual void Destroy();

	// eng: Returns texture
	// rus: Возвращает текстуру
	LPDIRECT3DTEXTURE9 GetTexture(){return texture;}

	// eng: Loading of frame texture
	// rus: Загрузка текстурного кадра
	virtual void LoadTexture(const MAGIC_CHANGE_ATLAS* c);

	// eng: Cleaning up of rectangle
	// rus: Очистка прямоугольника
	virtual void CleanRectangle(const MAGIC_CHANGE_ATLAS* c);

	// eng: Particle drawing
	// rus: Отрисовка частицы
	virtual void Draw(MAGIC_PARTICLE_VERTEXES* vertexes);

	// eng: Beginning of emitter drawing
	// rus: Начало отрисовки эмиттера
	virtual void BeginDrawEmitter(MP_Emitter* emitter);

	// eng: Beginning of particles drawing
	// rus: Начало отрисовки частиц
	virtual void BeginDrawParticles(int particles_count);

	// eng: End of particles drawing
	// rus: Конец отрисовки частиц
	virtual void EndDrawParticles();

	// eng: Setting of intense
	// rus: Установить интенсивность
	virtual void SetIntense(bool intense);

	// rus: Рисования сегмента
	static void DrawSegment();

	// rus: Захват текущего сегмента
	static void GetSegment();
};


// eng: Class controlling drawing
// rus: Класс, который управляет рисованием
class MP_Device_WRAP : public MP_Device
{
protected:
	HWND hwnd;

	LPDIRECT3D9 direct_3d;
	LPDIRECT3DDEVICE9 device_3d;

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
	LPDIRECT3DINDEXBUFFER9 index_buffer;

	DWORD state_addressu;
	DWORD state_addressv;
	DWORD state_colorarg1;
	DWORD state_colorarg2;
	DWORD state_colorop;
	DWORD state_alphaarg1;
	DWORD state_alphaarg2;
	DWORD state_alphaop;
	DWORD state_srcblend;
	DWORD state_alphaenable;
	DWORD state_lighting;
	DWORD state_zenable;
	DWORD state_cullmode;
	
public:
	MP_Device_WRAP(int width, int height, HWND hwnd);
	
	LPDIRECT3D9 GetDirect3D(){return direct_3d;}

	LPDIRECT3DDEVICE9 GetDevice3D(){return device_3d;}

	LPDIRECT3DVERTEXBUFFER9 GetVetexBuffer(){return vertex_buffer;}
	LPDIRECT3DINDEXBUFFER9 GetIndexBuffer(){return index_buffer;}
	void set1(LPDIRECT3D9 direct_3d1,LPDIRECT3DDEVICE9 device_3d1){direct_3d=direct_3d1;device_3d=device_3d1;};

protected:
	void InitBuffer();
	void DestroyBuffer();

public:

	// eng: Creating
	// rus: Создание
	virtual bool Create();

	// eng: Destroying
	// rus: Уничтожение
	virtual void Destroy();

	// eng: Setting of scene
	// rus: Настройка сцены
	virtual void SetScene3d();

	// eng: Beginning of scene drawing
	// rus: Начало отрисовки сцены
	virtual void BeginScene();

	// eng: End of scene drawing
	// rus: Конец отрисовки сцены
	virtual void EndScene();

	// eng: Indicates that device is lost
	// rus: Проверка на потерю устройства рисования
	virtual bool IsLost();
};

#endif