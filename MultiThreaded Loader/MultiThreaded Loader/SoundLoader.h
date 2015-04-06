/*
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2015 Media Design School
*
* File Name : SoundLoader.h
* Description : Declaration file for the SoundLoader class
* Author :	Jc Fowles
* Mail :	Jc.Fowels@mediadesign.school.nz
*/

#pragma once

// Library Includes
#include <iostream>
#include <windows.h>		// Include all the windows headers.
#include <string>
#include <thread>
#include <vector>

using namespace std;

class CSoundLoader
{
public:
	// Constructors / Destructors
	~CSoundLoader();

	// Prototypes
	void Load(HWND _hWnd);

	// Singleton Methods
	static CSoundLoader& GetInstance();
	static void DestroyInstance();

private:
	//Disallowing copies and extra constructions
	CSoundLoader();
	CSoundLoader(const CSoundLoader& _kr);
	CSoundLoader& operator= (const CSoundLoader& _kr);

	// Private Prototypes
	static void ThreadPlaySound(unsigned int _iThreadID, vector<wchar_t*> _SoundFilePath);

private:
	// Singleton Instance
	static CSoundLoader* s_pSoundLoader;

	// Member Variables
	vector<thread> m_vecThreads;
};

