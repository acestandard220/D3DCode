#include "../headers/ModelClass.h"


ModelClass::ModelClass()
{
	m_indexbuffer       = nullptr;
	m_vertexbuffer      = nullptr;
	m_indexbuffercount  = 0;
	m_vertexbuffercount = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device)
{
	m_result = InitializeBuffers(device);
	if (!m_result)
	{
		return false;
	}
	return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffer();
	return;
}

bool ModelClass::Render(ID3D11DeviceContext* devicecontext)
{
	m_result = RenderBuffers(devicecontext);
	if (!m_result)
	{
		return false;
	}
	return true;
}

//This function only creates and fills the buffers but does not Set them active 
// Refer to RenderBuffers() for setting them active
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	m_vertexbuffercount = 3;
	m_indexbuffercount  = 3;

	//Create a vertices type for the three positions and colors
	VertexType* vertices = new VertexType[m_vertexbuffercount];

	if (!vertices)
	{
		std::cout << "Could not create vertex array\n";
		return false;
	}

	unsigned long* indices = new unsigned long[m_indexbuffercount];
	if (!indices)
	{
		std::cout << "Could not create indices array\n";
		return false;
	}

	//Assign the values
	//Note that this is all on the CPUs side and they will be released when they get sent to the GPU.
	vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color    = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color    = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color    = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	//Create a description for our buffer as usual
	D3D11_BUFFER_DESC vertexbuffer_desc;

	vertexbuffer_desc.ByteWidth           = sizeof(VertexType) * m_vertexbuffercount; //Set the size of the buffer to the size of vertices* the number
	vertexbuffer_desc.Usage               = D3D11_USAGE_DEFAULT;
	vertexbuffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER; 
	vertexbuffer_desc.CPUAccessFlags      = 0; //Specify what kind of access the CPU has on the buffer
	vertexbuffer_desc.MiscFlags           = 0;
	vertexbuffer_desc.StructureByteStride = 0;

	//This is a sub resource data to point to the data on the CPU side
	//This is going to be use when pushing to the GPU
	D3D11_SUBRESOURCE_DATA  vertexdata;
	vertexdata.pSysMem          = vertices;
	vertexdata.SysMemPitch      = 0;
	vertexdata.SysMemSlicePitch = 0;

	//Create a buffer with its _desc, sub_resource and buffer.
	t_result = device->CreateBuffer(&vertexbuffer_desc, &vertexdata, &m_vertexbuffer);
	if (FAILED(t_result))
	{
		std::cout << "Could not create vertex buffer\n";
		//return false;
	}


	D3D11_BUFFER_DESC indexbuffer_desc;
	indexbuffer_desc.ByteWidth           = sizeof(unsigned) *m_indexbuffercount;
	indexbuffer_desc.Usage               = D3D11_USAGE_DEFAULT;
	indexbuffer_desc.BindFlags           = D3D11_BIND_INDEX_BUFFER;
	indexbuffer_desc.CPUAccessFlags      = 0;
	indexbuffer_desc.MiscFlags           = 0;
	indexbuffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA  indexdata;
	indexdata.pSysMem          = indices;
	indexdata.SysMemPitch      = 0;
	indexdata.SysMemSlicePitch = 0;

	t_result = device->CreateBuffer(&indexbuffer_desc,&indexdata,&m_indexbuffer);
	if (FAILED(t_result))
	{
		std::cout << "Could not create index buffer\n";
		return false;
	}

	//delete the CPU side memory buffers to prevent memory leaks
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
	return true;

}

void ModelClass::ShutdownBuffer()
{
	if (m_indexbuffer)
	{
		m_indexbuffer->Release();
		m_indexbuffer = nullptr;
	}
	if (m_vertexbuffer)
	{
		m_vertexbuffer->Release();
		m_vertexbuffer = nullptr;
	}
}

bool ModelClass::RenderBuffers(ID3D11DeviceContext* devicecontext)
{
	unsigned int offset;
	unsigned int stride;

	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer as the active vertex buffer to be used at this point
	devicecontext->IASetVertexBuffers(0, 1, &m_vertexbuffer, &stride, &offset);
	
	//Set the index buffer as the active vertex buffer to be used at this point
	devicecontext->IASetIndexBuffer(m_indexbuffer, DXGI_FORMAT_R32_UINT, 0);

	//Tell the GPU how it should draw the data in the bound buffer;
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
