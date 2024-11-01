#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_

#include <d3d11.h>
#include <directxmath.h>
#include <iostream>

class ModelClass {
    private:
	    struct VertexType {
		    DirectX::XMFLOAT3 position;
		    DirectX::XMFLOAT4 color;
	    };
    public:
	    ModelClass();
	    ModelClass(const ModelClass& other);
    	~ModelClass();

	    bool Initialize(ID3D11Device* device);
	    void Shutdown();
	    bool Render(ID3D11DeviceContext* devicecontext);

	    inline int GetIndexCount()const { return m_indexbuffercount; }
    private:
	    bool InitializeBuffers(ID3D11Device* device);
	    void ShutdownBuffer();
	    bool RenderBuffers(ID3D11DeviceContext* devicecontext);

    private:
	    ID3D11Buffer* m_vertexbuffer;
	    ID3D11Buffer* m_indexbuffer;
	    
	    int m_vertexbuffercount;
	    int m_indexbuffercount;
	    
	    bool m_result;
	    HRESULT t_result;
};
#endif // 
