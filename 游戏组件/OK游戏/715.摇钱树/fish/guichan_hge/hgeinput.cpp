/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naess閚 and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naess閚 a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * For comments regarding functions please see the header file.
 */

#include "hgeinput.hpp"
#include "guichan/exception.hpp"
#include "WinUser.h"


//键盘灵敏度，毫秒
#define			FEEL_TIME_TICK			500

namespace gcn
{
    HGE *HGEInput::mHGE = NULL;

    HGEInput::HGEInput()
    {
        mHGE = hgeCreate(HGE_VERSION);

        mMouseX = 0;
        mMouseY = 0;

        mLeftMouseButtonDown = false;
        mRightMouseButtonDown = false;
        mMiddleMouseButtonDown = false;

		HIDHandle=NULL;
		OnButtonStartscan();
    }

    HGEInput::~HGEInput()
    {
        mHGE->Release();
    }

    bool  HGEInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    bool  HGEInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    KeyInput  HGEInput::dequeueKeyInput()
    {
        if (isKeyQueueEmpty())
        {
            throw GCN_EXCEPTION("Key queue is empty.");
        }

        KeyInput keyInput;

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;

    }

    MouseInput HGEInput::dequeueMouseInput()
    {
        if (isMouseQueueEmpty())
        {
            throw GCN_EXCEPTION("Mouse queue is empty.");
        }

        MouseInput mouseInput;

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void  HGEInput::_pollInput()
    {
        hgeInputEvent ie;
    
        mHGE->Input_GetEvent(&ie);
        
        pollMouseInput();
        pollKeyInput(ie);
    }

    void  HGEInput::pollMouseInput()
    {
        float curMouseX, curMouseY;
        int mouseWheel;

        bool leftBtn, rightBtn, centerBtn;

        mHGE->Input_GetMousePos(&curMouseX, &curMouseY);
        mouseWheel = mHGE->Input_GetMouseWheel();

        leftBtn = mHGE->Input_GetKeyState(HGEK_LBUTTON);
        rightBtn = mHGE->Input_GetKeyState(HGEK_RBUTTON);
        centerBtn = mHGE->Input_GetKeyState(HGEK_MBUTTON);

        // Check mouse movement
        if (mMouseX != curMouseX 
            || mMouseY != curMouseY )
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::MOVED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
     
            mMouseX = curMouseX;
            mMouseY = curMouseY;
        }

        // Check mouse wheel
        if (mouseWheel > 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::WHEEL_MOVED_UP,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mouseWheel < 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::WHEEL_MOVED_DOWN,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }

        //check mouse buttons
        if (!mLeftMouseButtonDown && leftBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::LEFT,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mLeftMouseButtonDown && !leftBtn )
        {
            mMouseInputQueue.push(MouseInput(MouseInput::LEFT,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (!mRightMouseButtonDown && rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::RIGHT,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mRightMouseButtonDown &&  !rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::RIGHT,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (!mMiddleMouseButtonDown && centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mMiddleMouseButtonDown && !centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }

        mLeftMouseButtonDown  = leftBtn;
        mRightMouseButtonDown = rightBtn;
        mMiddleMouseButtonDown  = centerBtn;
    }

    void  HGEInput::pollKeyInput(hgeInputEvent &ki)
    {
        if (ki.type != INPUT_KEYDOWN 
            && ki.type != INPUT_KEYUP )
        {
             return ;
        }
        
        if (ki.type == INPUT_KEYDOWN)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::PRESSED);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed((ki.flags & HGEINP_SHIFT) ? true : false);
            keyInput.setAltPressed((ki.flags & HGEINP_ALT) ? true : false);
            keyInput.setControlPressed((ki.flags & HGEINP_CTRL) ? true : false);
        
            mKeyInputQueue.push(keyInput);
        }
  
        if (ki.type == INPUT_KEYUP)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::RELEASED);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed((ki.flags & HGEINP_SHIFT) ? true : false);
            keyInput.setAltPressed((ki.flags & HGEINP_ALT) ? true : false);
            keyInput.setControlPressed((ki.flags & HGEINP_CTRL) ? true : false);
        
            mKeyInputQueue.push(keyInput);
        }
    }

    Key HGEInput::convertToKey(int key, int chr)
    {
        int keysym;

        switch(key)
        {
            case HGEK_TAB:
              keysym = Key::TAB;  
              break;
            case HGEK_ESCAPE:
              keysym = Key::ESCAPE;
              break;
            case HGEK_ALT:
              keysym = Key::LEFT_ALT;
              break;
            /*
            Unsupported by HGE.
            case HGEK_ALT: 
            keysym = Key::RIGHT_ALT;
            break;
            */
            case HGEK_SHIFT:
              keysym = Key::LEFT_SHIFT;
              break;
            /*
            Unsuppored by HGE.
            case HGEK_SHIFT:
            keysym = Key::RIGHT_SHIFT;
            break;
            */
            case HGEK_CTRL:
              keysym = Key::LEFT_CONTROL;
              break;
            /*
            Unsupported by HGE.
            case HGEK_CTRL:
            keysym = Key::RIGHT_CONTROL;
            break;
            */
            case HGEK_LWIN:
              keysym = Key::LEFT_META;
              break;
            case HGEK_RWIN:
              keysym = Key::RIGHT_META;
              break;   
            case HGEK_INSERT:
              keysym = Key::INSERT;
              break;  
            case HGEK_HOME:
              keysym = Key::HOME;
              break;  
            case HGEK_PGUP:
              keysym = Key::PAGE_UP;
              break;
            case HGEK_PGDN:
              keysym = Key::PAGE_DOWN;
              break;    
            case HGEK_DELETE:
              keysym = Key::DELETE;
              break;
            /*
            Unsupported by HGE.
            case HGEK_BACKSPACE:
            keysym = Key::DELETE;
            pad = true;
            break;
            */
            case HGEK_END:
              keysym = Key::END;
              break;
            case HGEK_CAPSLOCK:
              keysym = Key::CAPS_LOCK;
              break;
            case HGEK_BACKSPACE:
              keysym = Key::BACKSPACE;
              break;
            case HGEK_F1:
              keysym = Key::F1;
              break;
            case HGEK_F2:
              keysym = Key::F2;
              break;
            case HGEK_F3:
              keysym = Key::F3;
              break;
            case HGEK_F4:
              keysym = Key::F4;
              break;
            case HGEK_F5:
              keysym = Key::F5;
              break;
            case HGEK_F6:
              keysym = Key::F6;
              break;
            case HGEK_F7:
              keysym = Key::F7;
              break;
            case HGEK_F8:
              keysym = Key::F8;
              break;
            case HGEK_F9:
              keysym = Key::F9;
              break;
            case HGEK_F10:
              keysym = Key::F10;
              break;
            case HGEK_F11:
              keysym = Key::F11;
              break;
            case HGEK_F12:
              keysym = Key::F12;
              break;
            /* 
            Unsupported by HGE
            case HGEK_PRTSCR:
            keysym = Key::PRINT_SCREEN;
            break;
            */
            case HGEK_PAUSE:
              keysym = Key::PAUSE;
              break;
            case HGEK_SCROLLLOCK:
              keysym = Key::SCROLL_LOCK;
              break;
            case HGEK_NUMLOCK:
              keysym = Key::NUM_LOCK;
              break;
            case HGEK_LEFT:
              keysym = Key::LEFT;
              break;
            case HGEK_RIGHT:
              keysym = Key::RIGHT;
              break;
            case HGEK_UP:
              keysym = Key::UP;
              break;
            case HGEK_DOWN:
              keysym = Key::DOWN;
              break;
            case HGEK_ENTER:
              keysym = Key::ENTER;
              break;
			case HGEK_SPACE:
			  keysym = Key::SPACE;
			  break;

            default:
              keysym = chr;
        }

        Key k = Key(keysym);

        return k;
    }

    bool  HGEInput::isNumericPad(int keyCode)
    {
        switch(keyCode)
        {
          case HGEK_NUMPAD0:
          case HGEK_NUMPAD1:
          case HGEK_NUMPAD2:
          case HGEK_NUMPAD3:
          case HGEK_NUMPAD4:
          case HGEK_NUMPAD5:
          case HGEK_NUMPAD6:
          case HGEK_NUMPAD7:
          case HGEK_NUMPAD8:
          case HGEK_NUMPAD9:
              return true;
          default: 
              return false;
        }
    }



	//以下为游戏手柄相关
	bool HGEInput::InitHIDDev()
	{
		struct _GUID GUID;
		SP_INTERFACE_DEVICE_DATA DeviceInterfaceData;
		struct {
			DWORD cbSize; 
			TCHAR DevicePath[256];
		} FunctionClassDeviceData;

	//	HIDD_ATTRIBUTES HIDAttributes;
		SECURITY_ATTRIBUTES SecurityAttributes;
		int Device;
		ULONG BytesReturned;
		bool Success;
	//	WCHAR ManufacturerBuffer[256], ProductBuffer[256];
		const WCHAR NotSupplied[] = L"NULL";

	// Initialize the GUID array and setup the security attributes for Win2000
		HidD_GetHidGuid(&GUID);
		SecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
		SecurityAttributes.lpSecurityDescriptor = NULL; 
		SecurityAttributes.bInheritHandle = false; 

	// Get a handle for the Plug and Play node and request currently active devices
		PnPHandle = SetupDiGetClassDevs(&GUID, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
		if (int(PnPHandle) == -1) {	
			printf("Could not attach to PnP node"); 
			int iRetErr = GetLastError();
			return false; 
		}

		//型号属性
		 HIDD_ATTRIBUTES Attributes;
	// Lets look for a maximum of 20 Devices
		for (Device = 0; (Device < 20); Device++) 
		{
	// Initialize our data
			DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);
	// Is there a device at this table entry
			Success = SetupDiEnumDeviceInterfaces(PnPHandle, NULL, &GUID, Device, &DeviceInterfaceData);
			if (Success) 
			{

	// There is a device here, get it's name
				FunctionClassDeviceData.cbSize = 6;
				Success = SetupDiGetDeviceInterfaceDetail(PnPHandle, &DeviceInterfaceData, 
					(PSP_INTERFACE_DEVICE_DETAIL_DATA)&FunctionClassDeviceData, 256, &BytesReturned, NULL);
				if (!Success) 
				{	
					printf("Could not find the system name for this device\n"); 
					int iRetErr = GetLastError();
					return false; 
				}

	// Can now open this device
				if(HIDHandle && HIDHandle != INVALID_HANDLE_VALUE)
					CloseHandle(HIDHandle);
				HIDHandle = CreateFile(FunctionClassDeviceData.DevicePath, GENERIC_READ|GENERIC_WRITE, 
					FILE_SHARE_READ|FILE_SHARE_WRITE, &SecurityAttributes, OPEN_EXISTING, 0, NULL);
				if (HIDHandle == INVALID_HANDLE_VALUE) 
					printf("Could not open HID #%d, Errorcode = %d\n", Device, GetLastError());
				else 
				{
//					TRACE("Create File handle:%ld\n", HIDHandle);


					// Get the information about this HID
					Success = HidD_GetAttributes(HIDHandle, &Attributes);
					if (!Success) { 
						printf("Could not get HID attributes\n"); 
						return false; //GetLastError(); 
					}
					
					//找到指定设备，记录下handle
					if(Attributes.VendorID == 2064 && Attributes.ProductID == 0x0003)
					{
	#if	0
					ManufacturerName = HidD_GetManufacturerString(HIDHandle, ManufacturerBuffer, 256);
					ProductName = HidD_GetProductString(HIDHandle, ProductBuffer, 256);

						// And display it!
					printf("VID = %4.4x, Name = ", HIDAttributes.VendorID);
					printf("%ws, ", ManufacturerName ? ManufacturerBuffer : NotSupplied);
					printf("PID = %4.4x, Name = ", HIDAttributes.ProductID);
					printf("%ws\n", ProductName ? ProductBuffer : NotSupplied);
	#endif

						Success = HidD_GetPreparsedData(HIDHandle, &PreparsedData);
						HidP_GetCaps(PreparsedData, &Capabilities);
					
						return true;
					}
					//CloseHandle(HIDHandle);
					}
				} // if (SetupDiEnumDeviceInterfaces . .
			} // for (Device = 0; (Device < 20); Device++)
		//SetupDiDestroyDeviceInfoList(PnPHandle);
		return false;
	}

	void HGEInput::CloseHIDDev()
	{
		if(HIDHandle && HIDHandle != INVALID_HANDLE_VALUE)
			CloseHandle(HIDHandle);

		if(PnPHandle && PnPHandle != INVALID_HANDLE_VALUE)
			SetupDiDestroyDeviceInfoList(PnPHandle);

	}

	DWORD HGEInput::StaticIO_Thread(LPVOID Param)
	{
		if(Param != NULL)
			return ((HGEInput*)Param)->ReadReport();
		else
			return -1;
	}

	//keybd_event(keyvalue, 0, 0, 0); 
	//keybd_event(keyvalue, 0, KEYEVENTF_KEYUP, 0); 
	DWORD HGEInput::ReadReport()
	{
		ULONG	Result;
		int InputReportLength = Capabilities.InputReportByteLength;
		BOOL	DeviceDetected = TRUE;
		BYTE	InputReport[300];
		DWORD	bRead;
		
		bool bLoopFire=false;
		bool bLocked=false;
		
		DWORD	dwTickCount=GetTickCount();
		do
		{
			Sleep(10);
			Result = ReadFile(HIDHandle, InputReport, Capabilities.InputReportByteLength,
				&bRead, NULL);

			if(Capabilities.InputReportByteLength != bRead)
				break;

			//第一个键值
			if(InputReport[6] == 0x1F)				//连发
			{
				bLoopFire=true;
			}
			else if(InputReport[6] == 0x2F)			//取消连发
				bLoopFire=false;

			else if(InputReport[7] == 0x08 || InputReport[1] == 0x08)			//锁定
			{
				if(bLocked)
				{
					keybd_event(81, 0, 0, 0); 
					keybd_event(81, 0, KEYEVENTF_KEYUP, 0);
				}
				else
				{
					keybd_event(83, 0, 0, 0); 
					keybd_event(83, 0, KEYEVENTF_KEYUP, 0);
				}
				bLocked = !bLocked;
			}
			else if(InputReport[1] == 0x01)		//取消
			{
				//keybd_event(81, 0, 0, 0); 
				//keybd_event(81, 0, KEYEVENTF_KEYUP, 0);
			}
			//第二个键值：点击以后，去掉提示信息
			if(InputReport[7] == 0x02 ||InputReport[1] == 0x80 || bLoopFire)				//发炮
			{

				//if(this->IsTopParentActive() )
				//{
				//	//首次登陆，可以去掉提示信息。
				//	//首次登陆，上点分
   					keybd_event(32, 0, 0, 0); 
					keybd_event(32, 0, KEYEVENTF_KEYUP, 0);		
				//}
				//{
				//	Key keyObj = convertToKey(32, 32);

				//	KeyInput keyInput(keyObj, KeyInput::PRESSED);

				//	mKeyInputQueue.push(keyInput);
				//}

				//
		
				//	Key keyObj = convertToKey(32, 32);

				//	KeyInput keyInput(keyObj, KeyInput::RELEASED);

				//	
				//	mKeyInputQueue.push(keyInput);

			}
			//第三个键值
			if(InputReport[6] == 0x4F)				//下分
			{
				if(GetTickCount() - dwTickCount > FEEL_TIME_TICK)
				{
					keybd_event(37, 0, 0, 0); 
					keybd_event(37, 0, KEYEVENTF_KEYUP, 0);
					dwTickCount = GetTickCount();
				}
			}
			else if(InputReport[6] == 0x8F)			//上分
			{
				if(GetTickCount() - dwTickCount > FEEL_TIME_TICK)
				{
					keybd_event(39, 0, 0, 0); 
					keybd_event(39, 0, KEYEVENTF_KEYUP, 0);
					dwTickCount = GetTickCount();
				}
			}
			else if(InputReport[7] == 0x01)			//减炮
			{
				if(GetTickCount() - dwTickCount > FEEL_TIME_TICK)
				{
					keybd_event(40, 0, 0, 0); 
					keybd_event(40, 0, KEYEVENTF_KEYUP, 0);
					dwTickCount = GetTickCount();
				}
			}
			else if(InputReport[7] == 0x04)			//加炮
			{
				if(GetTickCount() - dwTickCount > FEEL_TIME_TICK)
				{
					keybd_event(38, 0, 0, 0); 
					keybd_event(38, 0, KEYEVENTF_KEYUP, 0);
					dwTickCount = GetTickCount();
				}
			}

			//第四个键值
			int nStep1=0;
			int nStep2=0;
			if(InputReport[4] > 0x80)				//向右
			{
				nStep1 = (InputReport[4] - 0x80)/4;
			}
			else if(InputReport[4] < 0x80)			//向左
			{
				nStep1 = (InputReport[4] - 0x80)/4;
			}

			if(InputReport[5] > 0x80)				//上
			{
				nStep2 = (InputReport[5] - 0x80)/5;
			}
			else if(InputReport[5] < 0x80)			//下
			{
				nStep2 = (InputReport[5] - 0x80)/5;
			}
			//鼠标移动
			if(nStep1 != 0 || nStep2 != 0)
			{
				::SetCapture(NULL);
				mouse_event(MOUSEEVENTF_MOVE,nStep1, nStep2,0,0);
				::ReleaseCapture();
			}
			if(Result == 0)
			{
	//			CloseHandle(HIDHandle);
				DeviceDetected = FALSE;
			}

		}
		while((m_bApplicationActive == TRUE) );
		
		return 0;
	}


	void HGEInput::OnButtonStartscan() 
	{
		bool bRet = InitHIDDev();
		if(!bRet)
			return;
		TCHAR	OutputReport[3];
		DWORD	bWritten;
		OutputReport[0] = 0x00;
		OutputReport[1] = 0x01;
		bool Success = WriteFile(HIDHandle, OutputReport, Capabilities.OutputReportByteLength, &bWritten, NULL);
//		TRACE("WriteFile:%ld\n", HIDHandle);
		HANDLE  ThreadHandle;
		DWORD	ThreadID;

		m_bApplicationActive = TRUE;
		ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StaticIO_Thread, 
						this, 0, &ThreadID);


	}
}