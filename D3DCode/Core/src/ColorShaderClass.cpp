#include "../headers/ColorShaderClass.h"


ColorShaderClass::ColorShaderClass()
{
	m_vertexshader  = nullptr;
	m_pixelshader   = nullptr;
	m_matrixbuffer  = nullptr;
	m_layout        = nullptr;


}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{

}

ColorShaderClass::~ColorShaderClass()
{

}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	//Getting the shader source file paths
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int err;
	err = wcscpy_s(vsFilename, 128, L"color.vs");

	if (err != 0)
	{
		std::cout << "File path copy failed\n";
		return false;
	}
	err = wcscpy_s(psFilename, 128, L"color.ps");
	if (err != 0)
	{
		std::cout << "File path cop failed 2\n";
		return false;
	}
	//Passing the paths to the Shader initialization function
	m_result = InitializeShaders(device, hwnd, vsFilename, psFilename);
	if (!m_result)
	{
		return false;
	}
	return true;

}

bool ColorShaderClass::InitializeShaders(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT t_result;

	//Compile Shader from the provided path into a buffer.
	//Compile Vertex Shader into a buffer.
	ID3D10Blob* vertesShaderBuffer = nullptr;
	ID3D10Blob* errormessage = nullptr;
	t_result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertesShaderBuffer, &errormessage);
	if (FAILED(t_result))
	{
		if (errormessage)
		{
			OutputShaderErrorMessage(errormessage, hwnd, vsFilename);
		}
		else {
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Compile Pixel Shader into a buffer
	ID3D10Blob* pixelShaderbuffer = nullptr;
	t_result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderbuffer, &errormessage);
	if (FAILED(t_result))
	{
		if (errormessage)
		{
			OutputShaderErrorMessage(errormessage, hwnd, psFilename);
		}
		else {
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create a vertex Shader from the compiled binaries into an interface to handle it.
	t_result = device->CreateVertexShader(vertesShaderBuffer->GetBufferPointer(), vertesShaderBuffer->GetBufferSize(), NULL, &m_vertexshader);
	if (FAILED(t_result))
	{
		return false;
	}
	t_result = device->CreatePixelShader(pixelShaderbuffer->GetBufferPointer(), pixelShaderbuffer->GetBufferSize(), NULL, &m_pixelshader);
	if (FAILED(t_result))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonlayout[2];
	polygonlayout[0].SemanticName = "POSITION";
	polygonlayout[0].SemanticIndex = 0;
	polygonlayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonlayout[0].InputSlot = 0;
	polygonlayout[0].AlignedByteOffset = 0;
	polygonlayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonlayout[0].InstanceDataStepRate = 0;

	polygonlayout[1].SemanticName = "COLOR";
	polygonlayout[1].SemanticIndex = 0;
	polygonlayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonlayout[1].InputSlot = 0;
	polygonlayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonlayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonlayout[1].InstanceDataStepRate = 0;

	unsigned int numberElements = 0;
	numberElements = sizeof(polygonlayout) / sizeof(polygonlayout[0]);

	t_result = device->CreateInputLayout(polygonlayout, numberElements, vertesShaderBuffer->GetBufferPointer(), vertesShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(t_result))
	{
		return false;
	}
	vertesShaderBuffer->Release();
	vertesShaderBuffer = nullptr;

	pixelShaderbuffer->Release();
	pixelShaderbuffer = nullptr;

	//Create a description for a buffer that will hold the data for the GPU's constant buffer(in this context)
	//Create a description for the buffer on the CPU's side.
	D3D11_BUFFER_DESC matrixbuffer_desc;
	matrixbuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrixbuffer_desc.ByteWidth = sizeof(Matrixes); //Create it with the size of the type of data that will be stored in it.
	matrixbuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixbuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixbuffer_desc.MiscFlags = 0;
	matrixbuffer_desc.StructureByteStride = 0;
	
	t_result = device->CreateBuffer(&matrixbuffer_desc, NULL, &m_matrixbuffer);
	if (FAILED(t_result))
	{
		std::cout << "Could not create Matrix buffer\n";
		return false;
	}
	return true;
}

void ColorShaderClass::ShutdownShaders()
{
	if (m_matrixbuffer)
	{
		m_matrixbuffer->Release();
		m_matrixbuffer = nullptr;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_pixelshader)
	{
		m_pixelshader->Release();
		m_pixelshader = nullptr;
	}
	if (m_vertexshader)
	{
		m_vertexshader->Release();
		m_vertexshader = nullptr;
	}
	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3DBlob* errormessage, HWND hwnd, WCHAR* shaderfilename)
{
	char* compileerrors;
	unsigned long long buffersize,i;
	std::ofstream f_out;

	compileerrors = (char*)(errormessage->GetBufferPointer());
	buffersize = errormessage->GetBufferSize();
	
	f_out.open("shader_error_log.txt");

	for (i = 0; i < buffersize; i++)
	{
		f_out << compileerrors[i];
	}

	f_out.close();

	errormessage->Release();
	errormessage = nullptr;

	MessageBox(hwnd, L"Error Compiling the Shader.\nPlease check shader error log file for information about the error", L"Shader Error", MB_OK);

	return;
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceconteext, DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection)
{

	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	//This structure describes the memory mapping of a buffer
	//Use this buffer to set the values of the buffer as it contains an actual pointer to the data of the buffer
	D3D11_MAPPED_SUBRESOURCE mappedresource;
	deviceconteext->Map(m_matrixbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedresource);

	//Create a pointer of the type you won't to fill into the shader
	Matrixes* dataptr;
	//Let it point to the mapped memory's pointer, which is literally the buffer
	dataptr = (Matrixes*)mappedresource.pData;

	//Since when creating the buffer we gave it the same size as that of what the pointer is to point to 
	//perform pointer arithmetic on the buffer is fine
	dataptr->world = world;
	dataptr->view = view;
	dataptr->projection = projection;

	deviceconteext->Unmap(m_matrixbuffer, 0);

	unsigned int buffernumber = 0;
	deviceconteext->VSSetConstantBuffers(buffernumber, 1, &m_matrixbuffer);

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* devicecontext, int indexcount)
{
	devicecontext->IASetInputLayout(m_layout);

	devicecontext->VSSetShader(m_vertexshader, NULL, 0);
	devicecontext->PSSetShader(m_pixelshader, NULL, 0);

	devicecontext->DrawIndexed(indexcount, 0, 0);
	return;
}

void ColorShaderClass::Shutdown()
{
	ShutdownShaders();
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* devicecontext, int indexcount, DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& projection)
{
	m_result = SetShaderParameters(devicecontext, world, view, projection);
	if (!m_result)
	{
		return false;
	}

	RenderShader(devicecontext, indexcount);
	return true;
}
