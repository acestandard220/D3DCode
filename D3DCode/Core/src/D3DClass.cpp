#include "../headers/D3DClass.h"

D3DClass::D3DClass()
{
	m_swapChain          = nullptr;
	m_device             = nullptr;
	m_deviceContext      = nullptr;
	m_RenderTargetView   = nullptr;
	m_depthstencilbuffer = nullptr;
	m_depthStencilState  = nullptr;
	m_depthStencilView   = nullptr;
	m_RasterState        = nullptr;
}

bool D3DClass::Initialize(int screenWidth,
	                      int screenHeight, 
	                      bool vsync_enabled, 
	                      HWND hwnd, 
	                      bool fullscreen, 
	                      float Near,
	                      float Far)
{
	unsigned int numerator, denominator;
	
	m_vsync_enabled = vsync_enabled;
	GetRefreshRate(screenWidth, screenHeight, numerator, denominator);
	CreateSwapChain(screenWidth, screenHeight, fullscreen, numerator, denominator, hwnd);

	ID3D11Texture2D* backbufferptr;
	m_result = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backbufferptr));//Initial set to getting a view for the back buffer
	if (FAILED(m_result))
	{
		return false;
	}
	m_result = m_device->CreateRenderTargetView(backbufferptr, NULL, &m_RenderTargetView);
	backbufferptr->Release();
	backbufferptr = nullptr;

	//Main description for the depth buffer (m_depthstencilbuffer).
	D3D11_TEXTURE2D_DESC depthbuffer_desc;
	depthbuffer_desc.Height             = screenHeight;
	depthbuffer_desc.Width              = screenWidth;
	depthbuffer_desc.MipLevels          = 1;
	depthbuffer_desc.ArraySize          = 1;
	depthbuffer_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthbuffer_desc.SampleDesc.Count   = 1;
	depthbuffer_desc.SampleDesc.Quality = 0;
	depthbuffer_desc.Usage              = D3D11_USAGE_DEFAULT;
	depthbuffer_desc.CPUAccessFlags     = 0;
	depthbuffer_desc.MiscFlags          = 0;
	depthbuffer_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

	m_result = m_device->CreateTexture2D(&depthbuffer_desc, NULL, &m_depthstencilbuffer);
	if (FAILED(m_result))
	{
		return false;
	}
	

	//Describe the main state for depth/stencil buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilState_desc; 
	depthStencilState_desc.DepthEnable    = true;
	depthStencilState_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilState_desc.DepthFunc      = D3D11_COMPARISON_LESS;

	depthStencilState_desc.StencilEnable    = true;
	depthStencilState_desc.StencilReadMask  = 0xFF;
	depthStencilState_desc.StencilWriteMask = 0xFF;

	depthStencilState_desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilState_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilState_desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilState_desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	depthStencilState_desc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilState_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilState_desc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilState_desc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	m_result = m_device->CreateDepthStencilState(&depthStencilState_desc, &m_depthStencilState);
	if (FAILED(m_result))
	{
		return false;
	}
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	
	

	//Describe a view for the depthStencilbuffer itself.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilView_desc;
	depthStencilView_desc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilView_desc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilView_desc.Texture2D.MipSlice = 0;
	m_result = m_device->CreateDepthStencilView(m_depthstencilbuffer, &depthStencilView_desc, &m_depthStencilView);

	m_deviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_depthStencilView);

	//Describe the rasterization state which defines how geometry is rasteriszed
	D3D11_RASTERIZER_DESC rasterstate_desc;
	rasterstate_desc.AntialiasedLineEnable = false;
	rasterstate_desc.CullMode = D3D11_CULL_BACK;
	rasterstate_desc.DepthBias = 0;
	rasterstate_desc.DepthBiasClamp = 0;
	rasterstate_desc.DepthClipEnable = true;
	rasterstate_desc.FillMode = D3D11_FILL_SOLID;
	rasterstate_desc.FrontCounterClockwise = false;
	rasterstate_desc.MultisampleEnable = false;
	rasterstate_desc.ScissorEnable = false;
	rasterstate_desc.SlopeScaledDepthBias = 0.0f;

	m_result = m_device->CreateRasterizerState(&rasterstate_desc, &m_RasterState);
	if (FAILED(m_result))
	{
		return false;
	}

	m_deviceContext->RSSetState(m_RasterState);

	SetUpViewPort(screenWidth, screenHeight);

	float FOV         = 3.141592654f / 4.0f;
	float aspectRatio = float(screenWidth) / (float)screenHeight;

	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, Near, Far);
	m_WorldMatrix = DirectX::XMMatrixIdentity();

	m_orthographicMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, Near, Far);

	return true;
}

bool D3DClass::Shutdown()
{
	return true;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	m_deviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void D3DClass::EndScene()
{
	if (m_vsync_enabled)
	{
		m_swapChain->Present(1, 0);
	}
	else {
		m_swapChain->Present(0, 0);
	}
}

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

void D3DClass::GetProjectionMatrix(DirectX::XMMATRIX& mat)
{
	mat = m_projectionMatrix;
}

void D3DClass::GetWorldMatrix(DirectX::XMMATRIX& mat)
{
	mat = m_WorldMatrix;
}

void D3DClass::GetOrthoMatrix(DirectX::XMMATRIX& mat)
{
	mat = m_orthographicMatrix;
}

void D3DClass::GetVideoCardInfo(char* name, int& memory)
{
	strcpy_s(name, 128, m_videocarddescription);
	memory = m_videocardmemory;
	return;
}

void D3DClass::SetBackbufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_depthStencilView);
}

void D3DClass::ResetViewPort()
{
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

bool D3DClass::GetRefreshRate(int& screenWidth, 
	                          int& screenHeight,
	                          unsigned int& numerator,
	                          unsigned int& denominator)
{
	unsigned int numberModes = 0;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput*  output;

	DXGI_MODE_DESC*    outputmodeDesc_list;
	DXGI_ADAPTER_DESC  adapter_desc;
	unsigned long long stringlength;

	m_result = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (FAILED(m_result))
	{
		return false;
	}
	
	m_result = factory->EnumAdapters(NULL, &adapter);
	if (FAILED(m_result))
	{
		return false;
	}

	m_result = adapter->EnumOutputs(0, &output);
	if (FAILED(m_result))
	{
		return false;
	}
	m_result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
	                                      DXGI_ENUM_MODES_INTERLACED,
		                                  &numberModes,
		                                  NULL);
	if (FAILED(m_result))
	{
		return false;
	}

	outputmodeDesc_list = new DXGI_MODE_DESC[numberModes];
	if (!outputmodeDesc_list)
	{
		return false;
	}
	m_result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		                                  DXGI_ENUM_MODES_INTERLACED, 
		                                  &numerator, 
		                                  outputmodeDesc_list);
	if (FAILED(m_result))
	{
		return false;
	}

	for (int i = 0; i < numberModes; i++)
	{
		if (outputmodeDesc_list[i].Width == (unsigned int)screenWidth)
		{
			if (outputmodeDesc_list[i].Height == (unsigned int)screenHeight)
			{
				numerator   = outputmodeDesc_list[i].RefreshRate.Numerator;
				denominator = outputmodeDesc_list[i].RefreshRate.Denominator;
			}
		}
	}
	adapter->GetDesc(&adapter_desc);

	m_videocardmemory = (int)adapter_desc.DedicatedVideoMemory / 1024 / 1024;

	wcstombs_s(&stringlength, m_videocarddescription, 128, adapter_desc.Description, 128);


	delete[] outputmodeDesc_list;

	factory->Release();
	adapter->Release();
	output ->Release();

	factory = nullptr;
	adapter = nullptr;
	outputmodeDesc_list = nullptr;
	return true;
}

bool D3DClass::CreateSwapChain(int& screenWidth,
	                           int& screenHeight,
	                           bool fullscreen,
	                           unsigned int numerator,
	                           unsigned int denominator,
	                           HWND hwnd)
{
	D3D_FEATURE_LEVEL featurelevel;
	DXGI_SWAP_CHAIN_DESC swap_desc;
	swap_desc.BufferCount       = 1;
	swap_desc.Windowed          = fullscreen ? false : true;
	swap_desc.BufferDesc.Height = screenHeight;
	swap_desc.BufferDesc.Width  = screenWidth;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsync_enabled)
	{
		swap_desc.BufferDesc.RefreshRate.Numerator   = numerator;
		swap_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_desc.BufferDesc.RefreshRate.Numerator   = 0;
		swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_desc.SampleDesc.Count   = 1;
	swap_desc.SampleDesc.Quality = 0;

	swap_desc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hwnd;

	swap_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_desc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.Flags      = 0;

	featurelevel = D3D_FEATURE_LEVEL_11_0;
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0,
		&featurelevel,
		1, D3D11_SDK_VERSION,
		&swap_desc,
		&m_swapChain,
		&m_device,
		NULL,
		&m_deviceContext);
	if (m_deviceContext == nullptr)
	{
		std::cout << "The device context is null\n";
	}


	return true;
}

bool D3DClass::SetUpViewPort(int& screenWidth,int& screenHeight)
{
	m_viewport.TopLeftX  = 0.0f;
	m_viewport.TopLeftY  = 0.0f;
	m_viewport.Width     = (float)screenWidth;
	m_viewport.Height    = (float)screenHeight;
	m_viewport.MaxDepth  = 1.0f;
	m_viewport.MinDepth  = 0.0f;

	m_deviceContext->RSSetViewports(1, &m_viewport);
	return true;
}
