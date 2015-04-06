//
// Bachelor of Software Engineering
// Media Design School 
// Auckland
// New Zealand 
// 
// (c) 2005 - 2015 Media Design School 
// 
// File Name : CImageLoader.h 
// Description : Header file for the Image Loader class
// Author : Jc Fowles 
// Mail : Jc.Fowles@mediadesign.school.nz 
//
#pragma once

//Library includes
#include <Windows.h>
#include <vector>
#include <thread>

using namespace std;
class CImageLoader
{
public:
	~CImageLoader();

	void Load(HWND _hWnd, HDC _hDC);

	// Singleton Methods
	static CImageLoader& GetInstance();
	static void DestroyInstance();
	
	//void ThreadCreate(HDC _hDc, vector<wchar_t*>* _vecImageFilePaths);


private:
	//Disallowing copies and extra constructions
	CImageLoader();
	CImageLoader(const CImageLoader& _kr);
	CImageLoader& operator= (const CImageLoader& _kr);

	// Private Prototypes
	static void ThreadLoadImage(unsigned int _iThreadID, vector<wchar_t*> _vecImageFilePaths, HDC _hDC);

private:
	// Singleton Instance
	static CImageLoader* s_pImageLoader;
	//vector<wchar_t*> vecImageFilePaths;
	// Member Variables
	std::vector<std::thread> m_vecThreads;
	//HWND m_Hwnd;

	

};

