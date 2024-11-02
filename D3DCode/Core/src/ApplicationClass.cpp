#include "../headers/ApplicationClass.h"

ApplicationClass::ApplicationClass()
{
	m_d3dclass = nullptr;
	m_colorshaderclass = nullptr;
	m_modelclass = nullptr;
	m_cameraclass = nullptr;
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

	m_cameraclass = new CameraClass;
	m_cameraclass->SetPosition(0.0f, 0.0f, -5.0f);

	m_modelclass = new ModelClass;
	m_result = m_modelclass->Initialize(m_d3dclass->GetDevice());
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialize ModelClass", L"Initialization Error", MB_OK);
		return false;
	}

	m_colorshaderclass = new ColorShaderClass;
	m_result = m_colorshaderclass->Initialize(m_d3dclass->GetDevice(), hwnd);
	if (!m_result)
	{
		m_colorshaderclass->initOkay = false;
		MessageBox(hwnd, L"Could not initialize ColorShaderClass", L"Initialization Error", MB_OK);
		return false;
	}
	else {
		m_colorshaderclass->initOkay = true;
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
	if (m_colorshaderclass)
	{
		m_colorshaderclass->Shutdown();
		m_colorshaderclass = nullptr;
	}
	if (m_modelclass)
	{
		m_modelclass->Shutdown();
		m_modelclass = nullptr;
	}
	if (m_cameraclass)
	{
		delete m_cameraclass;
		m_cameraclass = nullptr;
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
	DirectX::XMMATRIX world, view, projection;

	m_d3dclass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	m_cameraclass->GetViewMatrix(view);
	m_d3dclass->GetWorldMatrix(world);
	m_d3dclass->GetProjectionMatrix(projection);

	m_cameraclass->Render();
	m_modelclass->Render(m_d3dclass->GetDeviceContext());
	if (m_colorshaderclass->initOkay)
	{
		m_colorshaderclass->Render(m_d3dclass->GetDeviceContext(), m_modelclass->GetIndexCount(), world, view, projection);
	}
	else {
		MessageBox(NULL, L"ColorClass initialization failed.\n..Cannot step into render function.",L"Class initialization Error",MB_OK);
		return false;
	}

	m_d3dclass->EndScene();
	return true;
}

