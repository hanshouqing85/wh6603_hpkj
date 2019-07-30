//-----------------------------------------------------------------------------
// File: mp_wrap.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "mp_wrap.h"

#pragma warning ( disable : 4996)

int MP_Atlas_WRAP::now_segment=0;
int MP_Atlas_WRAP::k_particles=0;

VERTEX_PARTICLE* MP_Atlas_WRAP::vertex=NULL;

// eng: Class storing the textural atlas
// rus: Класс, который хранит текстурный атлас
MP_Atlas_WRAP::MP_Atlas_WRAP(int width, int height, const char* file) : MP_Atlas(width, height, file)
{
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();

	if (file_name.empty())
	{
		if (width>0 && height>0)
		{
			device_3d->CreateTexture(atlas_width,atlas_height,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&texture,NULL);

			// cleaning up of created texture
			// rus: очистка созданной текстуры
			IDirect3DSurface9* back=NULL;
			HRESULT hr=device_3d->GetRenderTarget(0,&back);

			IDirect3DSurface9* dest=NULL;
			texture->GetSurfaceLevel(0,&dest);
			hr=device_3d->SetRenderTarget(0,dest);
			device_3d->Clear(0, NULL, D3DCLEAR_TARGET, 0x0, 0.0f, 0);
			dest->Release();

			device_3d->SetRenderTarget(0,back);

			back->Release();
		}
	}
	else
	{
		std::string texture_file=MP->GetPathToTexture();
		texture_file+=file_name.c_str();
		const wchar_t* texture_file_utf16=(const wchar_t*)Magic_UTF8to16((const unsigned char*)texture_file.c_str());
		D3DXCreateTextureFromFileW(device_3d, texture_file_utf16, &texture);
	}
}

// eng: Destroy atlas texture
// rus: Уничтожить текстуру атласа
void MP_Atlas_WRAP::Destroy()
{
	if (texture)
	{
		texture->Release();
		texture=NULL;
	}
}

// eng: Loading of frame texture
// rus: Загрузка текстурного кадра
void MP_Atlas_WRAP::LoadTexture(const MAGIC_CHANGE_ATLAS* c)
{   
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();

	DWORD state_alphaenable;
	device_3d->GetRenderState(D3DRS_ALPHABLENDENABLE, &state_alphaenable);
	device_3d->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	IDirect3DSurface9* dest=NULL;
	texture->GetSurfaceLevel(0,&dest);

	IDirect3DSurface9* back=NULL;
	HRESULT hr=device_3d->GetRenderTarget(0,&back);

	hr=device_3d->SetRenderTarget(0,dest);

	device_3d->BeginScene();

	LPDIRECT3DTEXTURE9 texture_frame=NULL;

	D3DXIMAGE_INFO info;
	if (c->data)
	{
		// eng: loading from memory
		// rus: загрузка из памяти
		hr=D3DXCreateTextureFromFileInMemoryEx(device_3d,c->data,c->length,0,0,D3DX_DEFAULT,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_DEFAULT,0,&info,NULL,&texture_frame);
	}
	else
	{
		// eng: loading from file
		// rus: загрузка из файла
		std::string texture_file=MP->GetPathToTexture();
		texture_file+=c->file;
		const wchar_t* texture_file_utf16=(const wchar_t*)Magic_UTF8to16((const unsigned char*)texture_file.c_str());
		hr=D3DXCreateTextureFromFileExW(device_3d,texture_file_utf16,0,0,D3DX_DEFAULT,0,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,D3DX_FILTER_NONE,D3DX_DEFAULT,0,&info,NULL,&texture_frame);
	}

	ID3DXSprite* sprite=NULL;

	D3DXCreateSprite(device_3d, &sprite);

	RECT rect_from;
	rect_from.left=0;
	rect_from.top=0;
	rect_from.right=info.Width;
	rect_from.bottom=info.Height;

	float scale_x=((float)c->width)/((float)info.Width);
	float scale_y=((float)c->height)/((float)info.Height);

	D3DXMATRIX scale_matrix;
	D3DXMatrixScaling(&scale_matrix,scale_x,scale_y,1);
	sprite->SetTransform(&scale_matrix);

	sprite->Begin(0);
	hr=sprite->Draw(texture_frame,  // rus: используемая текстура
	&rect_from,						// rus: прямоугольник источника
	NULL,
	&D3DXVECTOR3((float)c->x/scale_x, (float)c->y/scale_y, 0.f), 0xffffffff);

	sprite->End();

	sprite->Release();
	sprite=NULL;

	texture_frame->Release();
	texture_frame=NULL;

	dest->Release();

	device_3d->SetRenderTarget(0,back);

	back->Release();

	device_3d->EndScene();

	device_3d->SetRenderState(D3DRS_ALPHABLENDENABLE, state_alphaenable);
}

// eng: Cleaning up of rectangle
// rus: Очистка прямоугольника
void MP_Atlas_WRAP::CleanRectangle(const MAGIC_CHANGE_ATLAS* c)
{
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();

	IDirect3DSurface9* back=NULL;
	HRESULT hr=device_3d->GetRenderTarget(0,&back);

	IDirect3DSurface9* dest=NULL;
	texture->GetSurfaceLevel(0,&dest);
	hr=device_3d->SetRenderTarget(0,dest);
	D3DRECT rect;
	rect.x1=c->x;
	rect.y1=c->y;
	rect.x2=c->x+c->width;
	rect.y2=c->y+c->height;
	device_3d->Clear(1, &rect, D3DCLEAR_TARGET, 0x0, 0.0f, 0);
	dest->Release();

	device_3d->SetRenderTarget(0,back);

	back->Release();
}

// eng: Particle drawing
// rus: Отрисовка частицы
void MP_Atlas_WRAP::Draw(MAGIC_PARTICLE_VERTEXES* vertexes)
{
	unsigned int color=vertexes->color;

	vertex->x=vertexes->vertex1.x;
	vertex->y=vertexes->vertex1.y;
	vertex->u=vertexes->u1;
	vertex->v=vertexes->v1;
	vertex->color=color;
	#ifdef MAGIC_3D
	vertex->z=vertexes->vertex1.z;
	#else
	vertex->z=0;
	vertex->rhw=1.0f;
	#endif
	vertex++;

	vertex->x=vertexes->vertex2.x;
	vertex->y=vertexes->vertex2.y;
	vertex->u=vertexes->u2;
	vertex->v=vertexes->v2;
	vertex->color=color;
	#ifdef MAGIC_3D
	vertex->z=vertexes->vertex2.z;
	#else
	vertex->z=0;
	vertex->rhw=1.0f;
	#endif
	vertex++;

	vertex->x=vertexes->vertex3.x;
	vertex->y=vertexes->vertex3.y;
	vertex->u=vertexes->u3;
	vertex->v=vertexes->v3;
	vertex->color=color;
	#ifdef MAGIC_3D
	vertex->z=vertexes->vertex3.z;
	#else
	vertex->z=0;
	vertex->rhw=1.0f;
	#endif
	vertex++;

	vertex->x=vertexes->vertex4.x;
	vertex->y=vertexes->vertex4.y;
	vertex->u=vertexes->u4;
	vertex->v=vertexes->v4;
	vertex->color=color;
	#ifdef MAGIC_3D
	vertex->z=vertexes->vertex4.z;
	#else
	vertex->z=0;
	vertex->rhw=1.0f;
	#endif
	vertex++;

	k_particles++;

	if (k_particles>=SIZE_SEGMENT)
	{
		DrawSegment();
		GetSegment();
	}
};

// eng: Setting of intense
// rus: Установить интенсивность
void MP_Atlas_WRAP::SetIntense(bool intense)
{
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();
	if (intense)
	{
		// eng: turn on intense
		// rus: включить интенсивность
		device_3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		// eng turn off intense
		// rus: отключить интенсивность
		device_3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

// eng: Beginning of emitter drawing
// rus: Начало отрисовки эмиттера
void MP_Atlas_WRAP::BeginDrawEmitter(MP_Emitter* emitter)
{
	MP_Atlas::BeginDrawEmitter(emitter);

	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();

	LPDIRECT3DVERTEXBUFFER9 vertex_buffer=device->GetVetexBuffer();
	LPDIRECT3DINDEXBUFFER9 index_buffer=device->GetIndexBuffer();

	device_3d->SetVertexShader(NULL);
	device_3d->SetFVF(FVF_PARTICLE);
	device_3d->SetStreamSource(0, vertex_buffer, 0,sizeof(VERTEX_PARTICLE));

	device_3d->SetIndices(index_buffer);

	device_3d->SetTexture(0,texture);
}

// eng: Beginning of particles drawing
// rus: Начало отрисовки частиц
void MP_Atlas_WRAP::BeginDrawParticles(int particles_count)
{
	MP_Atlas::BeginDrawParticles(particles_count);
	GetSegment();
}

// eng: End of particles drawing
// rus: Конец отрисовки частиц
void MP_Atlas_WRAP::EndDrawParticles()
{
	MP_Atlas::EndDrawParticles();
	DrawSegment();
}

// rus: Рисования сегмента
void MP_Atlas_WRAP::DrawSegment()
{
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer=device->GetVetexBuffer();
	LPDIRECT3DINDEXBUFFER9 index_buffer=device->GetIndexBuffer();

	vertex_buffer->Unlock();

	if (k_particles)
	{
		HRESULT hr=device_3d->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,now_segment*SIZE_SEGMENT*4,0,k_particles*4,0,k_particles*2);

		now_segment++;
		if (now_segment>=MAX_SEGMENT)
			now_segment=0;
	}
}

// rus: Захват текущего сегмента
void MP_Atlas_WRAP::GetSegment()
{
	LPDIRECT3DDEVICE9 device_3d=device->GetDevice3D();
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer=device->GetVetexBuffer();
	LPDIRECT3DINDEXBUFFER9 index_buffer=device->GetIndexBuffer();

	k_particles=0;
	
	int step_vertex_segment=SIZE_SEGMENT*sizeof(VERTEX_PARTICLE)*4;
	vertex_buffer->Lock(
	step_vertex_segment*now_segment,
	step_vertex_segment,
	(void**)&vertex,
	now_segment ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
}

// --------------------------------------------------------------------------------

// eng: Class controlling drawing
// rus: Класс, который управляет рисованием
MP_Device_WRAP::MP_Device_WRAP(int width, int height, HWND hwnd) : MP_Device(width, height)
{
	this->hwnd=hwnd;

	direct_3d=NULL;
	device_3d=NULL;
	vertex_buffer=NULL;
	index_buffer=NULL;
}

// eng: Creating
// rus: Создание
bool MP_Device_WRAP::Create()
{
	//Destroy();
    /*
	bool result=false;
	direct_3d=Direct3DCreate9(D3D_SDK_VERSION);
	if (direct_3d)
	{
		bool full_screen=false;

		D3DDISPLAYMODE Display;
		HRESULT ok=direct_3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &Display);
		if (ok==D3D_OK)
		{
		    D3DPRESENT_PARAMETERS d3d9pp;
			ZeroMemory( &d3d9pp, sizeof(d3d9pp) );
			d3d9pp.SwapEffect=D3DSWAPEFFECT_DISCARD;
			d3d9pp.BackBufferFormat=Display.Format;

			d3d9pp.BackBufferCount=1;
			#ifdef TEST
			d3d9pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
			#endif

			d3d9pp.BackBufferWidth=Display.Width;
			d3d9pp.BackBufferHeight=Display.Height;

			if (full_screen)
			{
				d3d9pp.Windowed=false;
				d3d9pp.hDeviceWindow=NULL;
				
				d3d9pp.FullScreen_RefreshRateInHz=Display.RefreshRate;

				d3d9pp.MultiSampleType=D3DMULTISAMPLE_NONE;
				d3d9pp.MultiSampleQuality=0;
				d3d9pp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;

				window_width=Display.Width;
				window_height=Display.Height;
			}
			else
			{
				d3d9pp.Windowed=true;
				d3d9pp.hDeviceWindow=hwnd;

				RECT client_rect;
				GetClientRect(hwnd,&client_rect);
				window_width=client_rect.right;
				window_height=client_rect.bottom;

				#ifdef TEST
				d3d9pp.BackBufferWidth=client_rect.right;
				d3d9pp.BackBufferHeight=client_rect.bottom;
				#endif
			}

			while (true)
			{
				ok=direct_3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
											  D3DCREATE_HARDWARE_VERTEXPROCESSING,
											  &d3d9pp, &device_3d );

				if (ok==D3D_OK)
					break;

				ok=direct_3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											  &d3d9pp, &device_3d );

				if (ok==D3D_OK)
					break;

				switch (ok)
				{
					case D3DERR_OUTOFVIDEOMEMORY:
						return false;
					case D3DERR_INVALIDCALL:
						// eng: MS-DOS mode
						// rus: режим MS-DOS
						Sleep(300);
						break;
					case D3DERR_NOTAVAILABLE:
						// rus: на всякий случай
						Sleep(300);
						break;
					default:
						Sleep(300);
						break;
				}
			}
              */
			InitBuffer();

			//result=true;
		//}
	//}

	return true;
}

// eng: Setting of scene
// rus: Настройка сцены
void MP_Device_WRAP::SetScene3d()
{
	#ifdef MAGIC_3D
	// eng: viewport
	// rus: вьюпорт
	D3DVIEWPORT9 view_port;
	view_port.X=0;
	view_port.Y=0;
	view_port.Width=(unsigned int)window_width;
	view_port.Height=(unsigned int)window_height;
	view_port.MinZ=0.f;
	view_port.MaxZ=1.f;
	device_3d->SetViewport(&view_port);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	device_3d->SetTransform(D3DTS_WORLD, &world);

	// eng: projection matrix
	// rus: проектная матрица
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, (float)(MAGIC_PI * 0.15f), ((float)window_width)/((float)window_height), 0.0f, 1000.0f);
	device_3d->SetTransform(D3DTS_PROJECTION, &proj);

	// eng: view matrix
	// rus: матрица вида
	D3DXMATRIX view;
	D3DXVECTOR3 eye(0.f,0.f,-840.f);
	D3DXVECTOR3 at(0.f,0.f,0.f);
	D3DXVECTOR3 up(0.f,1.f,0.f);

	D3DXMatrixLookAtLH(&view,&eye,&at,&up);
	device_3d->SetTransform(D3DTS_VIEW, &view);

	Magic_SetViewMatrix((MAGIC_MATRIX*)&view);
	#endif
}

// eng: Beginning of scene drawing
// rus: Начало отрисовки сцены
void MP_Device_WRAP::BeginScene()
{
	MP_Device::BeginScene();

	device_3d->Clear(0, NULL, D3DCLEAR_TARGET, 0, 0.0f, 0);

	device_3d->BeginScene();

	// eng: save current states
	// rus: запоминаем текущие настройки
	device_3d->GetSamplerState(0,D3DSAMP_ADDRESSU,&state_addressu);
	device_3d->GetSamplerState(0,D3DSAMP_ADDRESSV,&state_addressv);

	device_3d->GetTextureStageState(0, D3DTSS_COLORARG1, &state_colorarg1);
	device_3d->GetTextureStageState(0, D3DTSS_COLORARG2, &state_colorarg2);
	device_3d->GetTextureStageState(0, D3DTSS_COLOROP,   &state_colorop);

	device_3d->GetTextureStageState(0, D3DTSS_ALPHAARG1, &state_alphaarg1);
	device_3d->GetTextureStageState(0, D3DTSS_ALPHAARG2, &state_alphaarg2);
	device_3d->GetTextureStageState(0, D3DTSS_ALPHAOP,   &state_alphaop);

	device_3d->GetRenderState(D3DRS_SRCBLEND, &state_srcblend);
	device_3d->GetRenderState(D3DRS_ALPHABLENDENABLE, &state_alphaenable);

	device_3d->GetRenderState(D3DRS_LIGHTING, &state_lighting);
	device_3d->GetRenderState(D3DRS_ZENABLE, &state_zenable);
	device_3d->GetRenderState(D3DRS_CULLMODE, &state_cullmode);

	// eng: change states
	// rus: меняем настройки
	device_3d->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	device_3d->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);

	device_3d->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device_3d->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device_3d->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);

	device_3d->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	device_3d->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	device_3d->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	device_3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device_3d->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	device_3d->SetRenderState(D3DRS_LIGHTING, FALSE);
	device_3d->SetRenderState(D3DRS_ZENABLE, FALSE);

	device_3d->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

// eng: End of scene drawing
// rus: Конец отрисовки сцены
void MP_Device_WRAP::EndScene()
{
	device_3d->EndScene();

	RECT rect;
	rect.left=0;
	rect.top=0;
	rect.right=window_width;
	rect.bottom=window_height;
	device_3d->Present(&rect, &rect, NULL, NULL);

	// eng: restore states
	// rus: восстанавливаем настройки
	device_3d->SetSamplerState(0, D3DSAMP_ADDRESSU, state_addressu);
	device_3d->SetSamplerState(0, D3DSAMP_ADDRESSV, state_addressv);

	device_3d->SetTextureStageState(0, D3DTSS_COLORARG1, state_colorarg1);
	device_3d->SetTextureStageState(0, D3DTSS_COLORARG2, state_colorarg2);
	device_3d->SetTextureStageState(0, D3DTSS_COLOROP,   state_colorop);

	device_3d->SetTextureStageState(0, D3DTSS_ALPHAARG1, state_alphaarg1);
	device_3d->SetTextureStageState(0, D3DTSS_ALPHAARG2, state_alphaarg2);
	device_3d->SetTextureStageState(0, D3DTSS_ALPHAOP,   state_alphaop);

	device_3d->SetRenderState(D3DRS_SRCBLEND, state_srcblend);
	device_3d->SetRenderState(D3DRS_ALPHABLENDENABLE, state_alphaenable);

	device_3d->SetRenderState(D3DRS_LIGHTING, state_lighting);
	device_3d->SetRenderState(D3DRS_ZENABLE, state_zenable);
	device_3d->SetRenderState(D3DRS_CULLMODE, state_cullmode);
}

// eng: Indicates that device is lost
// rus: Проверка на потерю устройства рисования
bool MP_Device_WRAP::IsLost()
{
	HRESULT hr=device_3d->TestCooperativeLevel();
	return (hr!=D3D_OK);
}

void MP_Device_WRAP::Destroy()
{
	MP_Device::Destroy();

	DestroyBuffer();

	if (device_3d)
	{
		device_3d->Release();
		device_3d=NULL;
	}

	if (direct_3d)
	{
		direct_3d->Release();
		direct_3d=NULL;
	}
}


void MP_Device_WRAP::InitBuffer()
{
	int buffer_vertex_size=MAX_SEGMENT*SIZE_SEGMENT;
	device_3d->CreateVertexBuffer(buffer_vertex_size*4*sizeof(VERTEX_PARTICLE),D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,FVF_PARTICLE,D3DPOOL_DEFAULT,&vertex_buffer,NULL);
	int buffer_index_size=SIZE_SEGMENT;
	device_3d->CreateIndexBuffer(buffer_index_size*6*sizeof(WORD),D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,D3DPOOL_DEFAULT,&index_buffer, NULL);

	WORD* index;
	index_buffer->Lock(0,buffer_index_size*6*sizeof(WORD),(void**)&index,0);
	for (int i=0;i<buffer_index_size;i++)
	{
		int vertex=i*4;
		WORD* ind=&(index[i*6]);
		*ind=vertex;
		ind++;
		*ind=vertex+1;
		ind++;
		*ind=vertex+2;
		ind++;
		*ind=vertex+3;
		ind++;
		*ind=vertex;
		ind++;
		*ind=vertex+2;
		ind++;
	}
	index_buffer->Unlock();
}

void MP_Device_WRAP::DestroyBuffer()
{
	if (vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer=NULL;
	}
	if (index_buffer)
	{
		index_buffer->Release();
		index_buffer=NULL;
	}
}
