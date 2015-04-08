
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
void CImageLoader::ThreadLoadImage(unsigned int _iThreadID, vector<wchar_t*> _ImageFilePath, HDC _hDC)
{
	//Concate the File path name with only one file using the vector of Image file paths(_ImageFilePath) 
	//Remebering that the directory is the first element in _ImageFilePath
	wstring strDirectory = _ImageFilePath[0];
	//wstring strBackslash = L"\\";
	wstring strFilePath = strDirectory + L"\\" + _ImageFilePath[_iThreadID];


	//Load the bitmap from the choosen the file path
	HANDLE hBmp = LoadImage(NULL, const_cast<LPWSTR>(strFilePath.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//Create a memory device that is compatible with the past is DC variable
	HDC CompatibleMemDev = CreateCompatibleDC(_hDC);
	//Select the new bitmap
	SelectObject(CompatibleMemDev, hBmp);

	//Copy the bits from the memory DC into the current dc
	//BitBlt(_hDC, 10 + (100 * (_iThreadID - 1)), 10 + (100 * (_iThreadID - 1)), 1000, 1000, CompatibleMemDev, 0, 0, SRCCOPY);
	StretchBlt(_hDC, _iThreadID, 0, 


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
	char maxFileStringSize[1024];		// String Buffer to contain the file name(s)

	// Initialize OPENFILENAME Structure
	ZeroMemory(&ofnImageFile, sizeof(ofnImageFile));
	ofnImageFile.lStructSize = sizeof(ofnImageFile);
	ofnImageFile.hwndOwner = _hWnd;
	ofnImageFile.lpstrFile = (LPWSTR)maxFileStringSize;
	ofnImageFile.lpstrFile[0] = '\0';
	ofnImageFile.nMaxFile = sizeof(maxFileStringSize);
	ofnImageFile.lpstrFilter = L"All\0*.*\0BMP\0*.bmp\0";
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

		//Create the threads and tell them to play sound
		for (unsigned int iID = 1; iID < ImageFilePath.size(); iID++)
		{
			m_vecThreads.push_back(thread(ThreadLoadImage, iID, ImageFilePath, _hDC));
		}
		
		//change this
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

//void CImageLoader::ThreadCreate(HDC _hDc, vector<wchar_t*>* _vecImageFilePaths)
//{
//	// Create Thread pool
//	// Zero Index in SoundFilePaths contains the directory only
//	for (unsigned int iID = 1; iID < _vecImageFilePaths->size(); iID++)
//	{
//		m_vecThreads.push_back(thread(ImageThread, iID, *_vecImageFilePaths, _hDc));
//	}
//
//	vector<thread>::iterator iterCurrent = m_vecThreads.begin();
//	vector<thread>::iterator iterEnd = m_vecThreads.end();
//
//	// Join all the threads
//	while (iterCurrent != iterEnd)
//	{
//		iterCurrent->join();
//		iterCurrent++;
//	}
//}

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