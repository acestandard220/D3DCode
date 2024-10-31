#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <vector>

#include "InputClass.h"
#include "ApplicationClass.h"



class SystemClass {
    
    public:
    	SystemClass();
    	~SystemClass();
    	SystemClass(const SystemClass& s);
    
    	bool Initialze();
    	bool Shutdown();
    	void Run();
    
        LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
    
    private:
    	bool Frame();
    	bool InitializeWindow(int&, int&);
    	bool ShutdownWindow();
    
    private:
    	LPCWSTR   m_applicationName;
    	HWND      m_hwnd;
    	HINSTANCE m_hInstance;
    
    	InputClass*       m_inputClass;
    	ApplicationClass* m_applicationClass;
    
    private:
    	bool m_result;
    	
};

static SystemClass* ApplicationHandle = 0;

static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
#endif
