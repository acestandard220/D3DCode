#include "../headers/ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_d3dclass = nullptr;
}

ApplicationClass::ApplicationClass(const ApplicationClass& a)
{

}

bool ApplicationClass::Initialize(int& screenWidth, int& screenHeight, HWND& hwnd)
{
	m_d3dclass = new D3DClass;
	m_result   = m_d3dclass->Initialize(screenWidth, screenHeight, V_SYNC, hwnd, FULL_SCREEN, Near, Far);
	if (!m_result)
	{
		MessageBox(NULL, L"Couldn't initialize D3DClass", L"ERROR", MB_OK);
		return false;
	}
	return true;
}

bool ApplicationClass::Shutdown()
{
	if (m_d3dclass)
	{
		m_d3dclass->Shutdown();
		delete m_d3dclass;
		m_d3dclass = nullptr;
	}
	return true;
}

bool ApplicationClass::Frame()
{
	m_result = Render();
	if (!m_result)
	{
		return false;
	}
	return true;
}

bool ApplicationClass::Render()
{

	m_d3dclass->BeginScene(0.3f, 0.4f, 0.1f, 1.0f);

	m_d3dclass->EndScene();
	return true;
}

