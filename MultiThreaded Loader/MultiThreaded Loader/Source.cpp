//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : Source.cpp 
// Description : Hold the main functionality for the program
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma comment( lib, "winmm.lib" )

//Library includes
#include <windows.h>
#include <vector>
#include <thread>
#include "resource.h"

#include "ImageLoader.h"
#include "SoundLoader.h"

#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"MultiThreaded Loader"

using namespace std;

/***********************
* WindowProc: This is the message handler for the Window,
* @author: Jc Fowles
* @Parameter: _hWnd: Handle to the Window sending the message
* @Parameter: _uiMsg: The message ID being sent
* @Parameter: _wParam: Additional detail about the message being sent
* @Parameter: _lParam: Additional detail about the message being sent
* @return: LRESULT: Program termination value
********************/
LRESULT CALLBACK WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	HDC hDC;
	PAINTSTRUCT Ps;
	
	//Procces the given message
	switch (_uiMsg)
	{
		
		case WM_COMMAND:
		{
			switch (LOWORD(_wParam))
			{
				case ID_EXIT:	//Close the application
				{
					DestroyWindow(_hWnd);
				}
				break;
				case ID_FILE_LOADIMAGE:
				{
					InvalidateRect(_hWnd, NULL, TRUE);
					hDC = BeginPaint(_hWnd, &Ps);
			
					CImageLoader& rImageLoader = CImageLoader::GetInstance();
					rImageLoader.Load(_hWnd, hDC);
					rImageLoader.DestroyInstance();
			
					EndPaint(_hWnd, &Ps);
									
				}
				break;
				case ID_FILE_LOADSOUND:
				{
					CSoundLoader& rSoundLoader = CSoundLoader::GetInstance();
					rSoundLoader.Load(_hWnd);
					rSoundLoader.DestroyInstance();
				}
				break;
			}
		
		}
		break;
		case WM_PAINT:
		{
			return (0);
		}
		break;
		case WM_CREATE:
		{
			return 0;
		}
		break;
		case WM_CLOSE:
		{
			DestroyWindow(_hWnd);
		}
		break;
		//Closing the window
		case WM_DESTROY:
		{
			//Send message to close the entire application
			PostQuitMessage(0);
			return 0;
		} break;
		
	}
	
	//Handles any messages the switch statement doesn't
	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

/***********************
* CreateAndRegisterWindow: Create and register the window of the application
* @author: Jc Fowles
* @Parameter: _hInstance: Instance handler for the application
* @Parameter: _iWidth: Width of the window
* @Parameter: _iHeight: Height of the window
* @Parameter: _pcTitle: Title of the window
* @return: HWND: Handle to the window
********************/
HWND CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, LPCWSTR _pcTitle)
{
	WNDCLASSEX winclass;

	//Clear out the window class for use
	ZeroMemory(&winclass, sizeof(WNDCLASSEX));

	//Fill in the struct with the needed information
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_OWNDC ;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(R2_WHITE));
	winclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	
	//Register the window class
	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	//Create the window and return the result as the handle
	HWND hWnd;

	HMENU hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU1));

	//Non-Full Screen
	hWnd =	CreateWindowEx(NULL,								// Extend style.
			WINDOW_CLASS_NAME,									// Class.		
			_pcTitle,											// Title.
			WS_SYSMENU | WS_VISIBLE |
			WS_OVERLAPPEDWINDOW  ,										//Window stlye
			0, 0,												// Initial x,y.
			_iWidth, _iHeight,									// Initial width, height.
			NULL,												// Handle to parent.
			hMenu,												// Handle to menu.
			_hInstance,											// Instance of this application.
			NULL);												// Extra creation parameters.

		

	//Check if window was created succesfully
	if (!hWnd)
	{
		// Failed to create.
		return (0);
	}

	//Return the handle to the window
	return (hWnd);
}

/***********************
* WinMain: The entry point for any Windows program
* @author: Jc Fowles
* @Parameter: _hInstance: Instance handle that Windows generates for your application
* @Parameter: _hPrevInstance: Tracker for the previous instance for the application
* @Parameter: _lpCmdline: Wide char string that contains the passed in arguments
* @Parameter: _iCmdshow: Integer passed in during launch, indicating how the application window is opened
* @return: int: Program termination value
********************/
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	//Screen Resolution
	const int kiWidth = 1015;
	const int kiHeight = 815;

	//This holds Windows event messages
	MSG msg;
	
	//Clear out the event message for use
	ZeroMemory(&msg, sizeof(MSG));
	
	//g_hInst = _hInstance;
	
	//Create and register the window
	HWND hWnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"MultiThreaded Loader");
	
	// display the window on the screen
	//ShowWindow(hWnd, _iCmdshow);
		
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			
	}
	
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}


	