/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : SoundLoader.cpp
* Description : Implementation file for the SoundLoader class
* Author :	Jc Fowles
* Mail :	Jc.Fowels@mediadesign.school.nz
*/

// This Includes
#include "SoundLoader.h"

// Static Variables
CSoundLoader* CSoundLoader::s_pSoundLoader = 0;

/***********************
* CSoundLoader: Default Constructor of SoundLoader
* @author: Jc Fowles
* @return:
********************/
CSoundLoader::CSoundLoader()
{

}

/***********************
* ~CSoundLoader: Default Destructor of SoundLoader
* @author: Jc Fowles
* @return:
********************/
CSoundLoader::~CSoundLoader()
{
}

/***********************
* SoundThread: Carries out the work of one thread. Loads a single sound file
* @author: Jc Fowles
* @parameter: _iThreadID: The Thread ID number
* @parameter: _SoundFilePath: Vector of Sound File Paths
* @return: void
********************/
void CSoundLoader::ThreadPlaySound(unsigned int _iThreadID, vector<wchar_t*> _SoundFilePath)
{
	wstring strDirectory = _SoundFilePath[0];
	wstring strPlayCommand = L"Play " + strDirectory + L"\\" + _SoundFilePath[_iThreadID];

	mciSendString(const_cast<LPWSTR>(strPlayCommand.c_str()), NULL, 0, NULL);
}

/***********************
* Load: Allows users to select multiple sounds and sends each one to a different thread
* @author: Jc Fowles
* @parameter: _hWnd: Handle to the Window
* @return: void
********************/
void CSoundLoader::Load(HWND _hWnd)
{
	OPENFILENAME ofnSoundFile;			// File Explorer structure
	char maxFileStringSize[1024];		// String Buffer to contain the file name(s)

	// Initialize OPENFILENAME Structure
	ZeroMemory(&ofnSoundFile, sizeof(ofnSoundFile));
	ofnSoundFile.lStructSize = sizeof(ofnSoundFile);
	ofnSoundFile.hwndOwner = _hWnd;
	ofnSoundFile.lpstrFile = (LPWSTR)maxFileStringSize;
	ofnSoundFile.lpstrFile[0] = '\0';
	ofnSoundFile.nMaxFile = sizeof(maxFileStringSize);
	ofnSoundFile.lpstrFilter = L"All\0*.*\0WAV\0*.wav\0";
	ofnSoundFile.nFilterIndex = 1;
	ofnSoundFile.lpstrFileTitle = NULL;
	ofnSoundFile.nMaxFileTitle = 0;
	ofnSoundFile.lpstrInitialDir = NULL;
	ofnSoundFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	//Opens the file explorer to select sounds file paths
	if (GetOpenFileName(&ofnSoundFile) == TRUE)
	{
		//Vector to store all File Paths
		vector<wchar_t*> SoundFilePath;

		//Separate the ofnSoundFile into the vector of file paths(SoundFilePath)
		//Saving the directoty as the first element
		wchar_t* ptr = ofnSoundFile.lpstrFile;
		ptr[ofnSoundFile.nFileOffset - 1] = 0;
		SoundFilePath.push_back(ptr);
		
		//Then Saving the file names into SoundFilePath
		ptr += ofnSoundFile.nFileOffset;
		while (*ptr)
		{
			SoundFilePath.push_back(ptr);
			ptr += (lstrlenW(ptr) + 1);		//go to next file name
		}

		//Create the threads and tell them to play sound
		for (unsigned int iID = 1; iID < SoundFilePath.size(); iID++)
		{
			m_vecThreads.push_back(thread(ThreadPlaySound, iID, SoundFilePath));
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

/***********************
* GetInstance: Returns the singleton instance of the SoundLoader, if it doesnt exist creates it.
* @author: Jc Fowles
* @return: CSoundLoader&: The current instance of the SoundLoader
********************/
CSoundLoader& CSoundLoader::GetInstance()
{
	//create instance of sound loader if one does not excist
	if (s_pSoundLoader == 0)
	{
		s_pSoundLoader = new CSoundLoader();
	}
	//return the singleton instance of the sound loader
	return (*s_pSoundLoader);
}

/***********************
* DestroyInstance: Deletes the instance of the SoundLoader
* @author: Jc Fowles
* @return: void
********************/
void CSoundLoader::DestroyInstance()
{
	//Destroy the instance of sound loader if it excist
	if (s_pSoundLoader != 0)
	{
		delete s_pSoundLoader;
		s_pSoundLoader = 0;
	}
}