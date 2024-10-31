#include "../headers/SystemClass.h"


SystemClass::SystemClass()
{
	m_applicationClass = nullptr;
	m_inputClass       = nullptr;
	m_applicationName  = nullptr;
	m_result = NULL;
}
SystemClass::SystemClass(const SystemClass& s){}
SystemClass::~SystemClass() {};

bool SystemClass::Initialze()
{
	int screenWidth = 0;
	int screenHeight = 0;

	m_result = InitializeWindow(screenWidth, screenHeight);

	m_applicationClass = new ApplicationClass;
	m_result           = m_applicationClass->Initialize(screenWidth,screenHeight,m_hwnd);

	m_inputClass       = new InputClass;
	m_inputClass->Initialize();

	return true;
	
}

bool SystemClass::Shutdown()
{
	if (m_applicationClass)
	{
		m_applicationClass->Shutdown();
		delete m_applicationClass;
		m_applicationClass = nullptr;
	}

	if (m_inputClass)
	{
		m_inputClass->Shutdown();
		delete m_inputClass;
		m_inputClass = nullptr;

	}
	return true;
}

void SystemClass::Run()
{
	MSG msg;

	bool Done = false;

	while (!Done)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			Done = true;
		}
		else {
			m_result = Frame();
			if (!m_result)
			{
				Done = true;
			}
		}
	}
}

bool SystemClass::Frame()
{
	if (m_inputClass->isKeyPressed(VK_ESCAPE))
	{
		return false;
	}
	else {
		m_result = m_applicationClass->Frame();
		if (!m_result)
		{
			return false;
		}
	}
	return true;
}

bool SystemClass::InitializeWindow(int& screenWidth, int&screenHeight)
{
	DEVMODE devmode;
	int x, y;
	m_applicationName = L"DXEngine";
	ApplicationHandle = this;

	m_hInstance = GetModuleHandle(0);


	WNDCLASSEX wc    = {};
	wc.lpfnWndProc   = WinProc;
	wc.lpszClassName = m_applicationName;
	wc.hInstance     = m_hInstance;

	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.cbSize        = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	if (FULL_SCREEN)
	{
		memset(&devmode, 0, sizeof(devmode));
		devmode.dmPelsWidth  = (unsigned long)screenWidth;
		devmode.dmPelsHeight = (unsigned long)screenHeight;
		devmode.dmSize       = sizeof(devmode);
		devmode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		devmode.dmBitsPerPel = 32;

		ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
		x = y = 0;
	}
	else {
		screenHeight = 600;
		screenWidth = 800;

		x = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		x, y, 
		screenWidth, screenHeight, 
		NULL, NULL, 
		m_hInstance, 
		NULL);
	
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//ShowCursor(false);
	
	return true;
}
bool SystemClass::ShutdownWindow()
{
	ShowCursor(true);
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;

	UnregisterClass(m_applicationName, m_hInstance);

	m_hInstance = nullptr;

	ApplicationHandle = nullptr;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	    case WM_KEYDOWN:
	    {
			
		    m_inputClass->KeyDown((unsigned int)wparam);
		    break;
	    }
		case WM_KEYUP:
		{
			m_inputClass->KeyUp((unsigned int)wparam);
			break;
		}
		default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	    case WM_CLOSE:
	    {
			PostQuitMessage(0);
			break;
	    }
	    case WM_DESTROY:
	    {
			DestroyWindow(hwnd);
			break;
	    }
	    default:
		    return ApplicationHandle->MessageHandler(hwnd,msg,wparam,lparam);
	}
	return 0;
}

