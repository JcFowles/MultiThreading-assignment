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
#include <windowsx.h>
#include "resource.h"
//#include <atlimage.h>
//#include <afxwin.h>

#define WIN32_LEAN_AND_MEAN
#define WINDOW_CLASS_NAME L"MultiThreaded Loader"

bool FileOpen(OPENFILENAME* _DestFileName, HWND _hwnd); 
bool LoadBMPIntoDC(HDC hDC, LPCTSTR bmpfile);
BOOL LoadBitmapFromBMPFile(LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette);


HINSTANCE g_hInst;
bool image = false;
OPENFILENAME pFileNamePath;

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
	//OPENFILENAME pFileNamePath;
	HANDLE hBmp;
	//HBITMAP bmpExercising;
	HDC hDC;
	HDC MemDCExercising;
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
			image = FileOpen(&pFileNamePath, _hWnd);
			InvalidateRect(_hWnd, NULL, TRUE);
			UpdateWindow(_hWnd);
		}
		break;
		case ID_FILE_LOADSOUND:
		{
			GetOpenFileName(&pFileNamePath);
		}
		break;
		}
		
	}
	break;
	case WM_PAINT:
	{
		

		hDC = BeginPaint(_hWnd, &Ps);

		
		if (image)
		{
		
			//CImage Pic;
			//Pic.Load(pFileNamePath.lpstrFile); // just change extension to load jpg
			//CBitmap bitmap;
			//bitmap.Attach(Pic.Detach());

			// Load the bitmap from the resource
			hBmp = LoadImage(g_hInst, pFileNamePath.lpstrFile, IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE);
				
			// Create a memory device compatible with the above DC variable
			MemDCExercising = CreateCompatibleDC(hDC);
			// Select the new bitmap
			SelectObject(MemDCExercising, hBmp);

			// Copy the bits from the memory DC into the current dc
			BitBlt(hDC, 10, 10, 450, 400, MemDCExercising, 0, 0, SRCCOPY);

			// Restore the old bitmap
			DeleteDC(MemDCExercising);
			DeleteObject(hBmp);
		}
		EndPaint(_hWnd, &Ps);

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
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
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

	//Non-Full Screen
	hWnd = CreateWindowEx(NULL,								// Extend style.
		WINDOW_CLASS_NAME,									// Class.		
		_pcTitle,											// Title.
		WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CAPTION |
		WS_SYSMENU | WS_VISIBLE,							//Window stlye
		CW_USEDEFAULT, CW_USEDEFAULT,						// Initial x,y.
		_iWidth, _iHeight,									// Initial width, height.
		NULL,												// Handle to parent.
		NULL,												// Handle to menu.
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
	const int kiWidth = 500;
	const int kiHeight = 400;

	//This holds Windows event messages
	MSG msg;

	//Clear out the event message for use
	ZeroMemory(&msg, sizeof(MSG));

	g_hInst = _hInstance;

	//Create and register the window
	HWND hWnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"MultiThreaded Loader");

	// display the window on the screen
	ShowWindow(hWnd, _iCmdshow);
	
	char szFile[MAX_PATH];

	ZeroMemory(&pFileNamePath, sizeof(pFileNamePath));
	pFileNamePath.lStructSize = sizeof(pFileNamePath);
	pFileNamePath.hwndOwner = hWnd;
	pFileNamePath.lpstrFile = (LPWSTR)szFile;
	pFileNamePath.lpstrFile[0] = '\0';
	pFileNamePath.nMaxFile = sizeof(szFile);
	pFileNamePath.lpstrFilter = L"All\0*.*\0BMP\0*.bmp\0";
	pFileNamePath.nFilterIndex = 1;
	pFileNamePath.lpstrFileTitle = NULL;
	pFileNamePath.nMaxFileTitle = 0;
	pFileNamePath.lpstrInitialDir = NULL;
	pFileNamePath.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

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

bool FileOpen(OPENFILENAME* _DestFileName, HWND _hwnd)
{
	if (GetOpenFileName(_DestFileName) == TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LoadBMPIntoDC(HDC hDC, LPCTSTR bmpfile)
{
	// check if params are valid
	if ((NULL == hDC) || (NULL == bmpfile))
	{
		MessageBox(0, L"Window Parm Invalid!", L"Error!", MB_ICONSTOP | MB_OK);
		return FALSE;
	}
	// load bitmap into a bitmap handle
	HANDLE hBmp = LoadImage(NULL, bmpfile, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	if (NULL == hBmp)
	{
		MessageBox(0, L"Window Handler Invalid!", L"Error!", MB_ICONSTOP | MB_OK);
		return FALSE;        // failed to load image
	}
	// bitmaps can only be selected into memory dcs:
	HDC dcmem = CreateCompatibleDC(NULL);

	// now select bitmap into the memory dc
	if (NULL == SelectObject(dcmem, hBmp))
	{	// failed to load bitmap into device context
		DeleteDC(dcmem);
		MessageBox(0, L"Window Load Failed!", L"Error!", MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// now get the bmp size
	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);
	// and blit it to the visible dc
	if (BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, dcmem,
		0, 0, SRCCOPY) == 0)
	{	// failed the blit
		DeleteDC(dcmem);
		MessageBox(0, L"Window Blit Failed!", L"Error!", MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	DeleteDC(dcmem);  // clear up the memory dc
	return TRUE;
}

BOOL LoadBitmapFromBMPFile(LPTSTR szFileName, HBITMAP *phBitmap,HPALETTE *phPalette)
{

	BITMAP  bm;

	*phBitmap = NULL;
	*phPalette = NULL;

	// Use LoadImage() to get the image loaded into a DIBSection
	*phBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (*phBitmap == NULL)
		return FALSE;

	// Get the color depth of the DIBSection
	GetObject(*phBitmap, sizeof(BITMAP), &bm);
	// If the DIBSection is 256 color or less, it has a color table
	if ((bm.bmBitsPixel * bm.bmPlanes) <= 8)
	{
		HDC           hMemDC;
		HBITMAP       hOldBitmap;
		RGBQUAD       rgb[256];
		LPLOGPALETTE  pLogPal;
		WORD          i;

		// Create a memory DC and select the DIBSection into it
		hMemDC = CreateCompatibleDC(NULL);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, *phBitmap);
		// Get the DIBSection's color table
		GetDIBColorTable(hMemDC, 0, 256, rgb);
		// Create a palette from the color tabl
		pLogPal = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + (256 * sizeof(PALETTEENTRY)));
		pLogPal->palVersion = 0x300;
		pLogPal->palNumEntries = 256;
		for (i = 0; i<256; i++)
		{
			pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}
		*phPalette = CreatePalette(pLogPal);
		// Clean up
		free(pLogPal);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	else   // It has no color table, so use a halftone palette
	{
		HDC    hRefDC;

		hRefDC = GetDC(NULL);
		*phPalette = CreateHalftonePalette(hRefDC);
		ReleaseDC(NULL, hRefDC);
	}
	return TRUE;

}