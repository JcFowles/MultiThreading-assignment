
/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : ImageLoader.cpp
* Description : Implementation file for the ImageLoader class
* Author :	Jc Fowles
* Mail :	Jc.Fowels@mediadesign.school.nz
*/

// This include
#include "ImageLoader.h"

// Static Variables
CImageLoader* CImageLoader::s_pImageLoader = 0;

/***********************
* CSoundLoader: Default Constructor of SoundLoader
* @author: Jc Fowles
* @return: 
********************/
CImageLoader::CImageLoader()
{
	
}

/***********************
* ~CSoundLoader: Default Destructor of SoundLoader
* @author: Jc Fowles
* @return:
********************/
CImageLoader::~CImageLoader()
{

}

/***********************
* ThreadLoadImage: Carries out the work of one thread. Displays one image per thread
* @author: Jc Fowles
* @parameter: _iThreadID: The Thread ID number
* @parameter: _ImageFilePath: Vector of Image File Paths
* @return: void
********************/
void CImageLoader::ThreadLoadImage(unsigned int _iThreadID, vector<wchar_t*> _ImageFilePath, HDC _hDC, int _iWidthImageCount)
{
	//Concate the File path name with only one file using the vector of Image file paths(_ImageFilePath) 
	//Remebering that the directory is the first element in _ImageFilePath
	wstring strDirectory = _ImageFilePath[0];
	wstring strFilePath = strDirectory + L"\\" + _ImageFilePath[_iThreadID];

	//Load the bitmap from the choosen the file path
	HANDLE hBmp = LoadImage(NULL, const_cast<LPWSTR>(strFilePath.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//Create a memory device that is compatible with the past is DC variable
	HDC CompatibleMemDev = CreateCompatibleDC(_hDC);
	
	//Select the new bitmap
	SelectObject(CompatibleMemDev, hBmp);

	BITMAP bmpImage;
	GetObject(hBmp, sizeof(bmpImage), &bmpImage);

	//Aspect ratio of the image
	float fAspect = static_cast<float>(bmpImage.bmWidth) / static_cast<float>(bmpImage.bmHeight);

	int iDestinationWidth, iDestinationHeight;

	if (fAspect >= 1)
	{
		iDestinationWidth = 100;
		iDestinationHeight = static_cast<int>(100 / fAspect);
	}
	else
	{
		iDestinationWidth = static_cast<int>(100 * fAspect);
		iDestinationHeight = 100;
	}

	//Draw the image into the divice Context
	StretchBlt(_hDC, (((_iThreadID - 1) % _iWidthImageCount)) * 100, ((_iThreadID - 1) / _iWidthImageCount) * 100, iDestinationWidth, iDestinationHeight, CompatibleMemDev, 0, 0, bmpImage.bmWidth, bmpImage.bmHeight, SRCCOPY);
	
	//Restore the old bitmap
	DeleteDC(CompatibleMemDev);
	DeleteObject(hBmp);
}

/***********************
* LoadSound: Allows users to select multiple sounds and sends each one to a different thread
* @author: Jc Fowles
* @parameter: _hWnd: Handle to the Window
* @parameter: _hDC: Handle to the Device context
* @return: void
********************/
void CImageLoader::Load(HWND _hWnd , HDC _hDC)
{
	OPENFILENAME ofnImageFile;			// File Explorer structure
	char maxFileStringSize[10000];		// String Buffer to contain the file name(s)

	// Initialize OPENFILENAME Structure
	ZeroMemory(&ofnImageFile, sizeof(ofnImageFile));
	ofnImageFile.lStructSize = sizeof(ofnImageFile);
	ofnImageFile.hwndOwner = _hWnd;
	ofnImageFile.lpstrFile = (LPWSTR)maxFileStringSize;
	ofnImageFile.lpstrFile[0] = '\0';
	ofnImageFile.nMaxFile = sizeof(maxFileStringSize);
	ofnImageFile.lpstrFilter = L"All\0*.*\0BMP\0*.BMP\0";
	ofnImageFile.nFilterIndex = 1;
	ofnImageFile.lpstrFileTitle = NULL;
	ofnImageFile.nMaxFileTitle = 0;
	ofnImageFile.lpstrInitialDir = NULL;
	ofnImageFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	//Opens the file explorer to select sounds file paths
	if (GetOpenFileName(&ofnImageFile) == TRUE)
	{
		//Vector to store all File Paths
		vector<wchar_t*> ImageFilePath;

		//Separate the ofnSoundFile into the vector of file paths(SoundFilePath)
		//Saving the directoty as the first element
		wchar_t* ptr = ofnImageFile.lpstrFile;
		ptr[ofnImageFile.nFileOffset - 1] = 0;
		ImageFilePath.push_back(ptr);
		
		//Then Saving the file names into SoundFilePath
		ptr += ofnImageFile.nFileOffset;
		while (*ptr)
		{
			ImageFilePath.push_back(ptr);
			ptr += (lstrlenW(ptr) + 1);		//go to next file name
		}

		RECT recWindow;
		GetWindowRect(_hWnd, &recWindow);

		int iWndWidth = recWindow.right - recWindow.left;

		int iWidthImageCount = iWndWidth / 100;

		SetStretchBltMode(_hDC, HALFTONE);
		//Create the threads and tell them to play sound
		for (unsigned int iID = 1; iID < ImageFilePath.size(); iID++)
		{
			m_vecThreads.push_back(thread(ThreadLoadImage, iID, ImageFilePath, _hDC, iWidthImageCount));
		}

		vector<thread>::iterator iterCurrent = m_vecThreads.begin();
		vector<thread>::iterator iterEnd = m_vecThreads.end();
		
		// Join all the threads
		while (iterCurrent != iterEnd)
		{
			iterCurrent->join();
			iterCurrent++;
		}
	}
}

/***********************
* GetInstance: Returns the singleton instance of the SoundLoader, if it doesnt exist creates it.
* @author: Jc Fowles
* @return: CSoundLoader&: The current instance of the SoundLoader
********************/
CImageLoader& CImageLoader::GetInstance()
{
	//create instance of sound loader if one does not excist
	if (s_pImageLoader == 0)
	{
		s_pImageLoader = new CImageLoader();
	}
	//return the singleton instance of the sound loader
	return (*s_pImageLoader);
}

/***********************
* DestroyInstance: Deletes the instance of the SoundLoader
* @author: Jc Fowles
* @return: void
********************/
void CImageLoader::DestroyInstance()
{
	//Destroy the instance of sound loader if it excist
	if (s_pImageLoader != 0)
	{
		delete s_pImageLoader;
		s_pImageLoader = 0;
	}
}