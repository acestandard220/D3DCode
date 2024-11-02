#ifndef _D3D_CLASS_H_
#define _D3D_CLASS_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "comsuppw.lib")


#include <d3d11.h>
#include <directxmath.h>
#include <iostream>
#include <comdef.h>

#define yes true;
#define no  false

class D3DClass {
    public:
        D3DClass();
        ~D3DClass() {}
        D3DClass(const D3DClass& d){}


        bool Initialize(int, int, bool, HWND, bool, float, float);
        bool Shutdown();


        void BeginScene(float, float, float, float);
        void EndScene();

    public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

        void GetProjectionMatrix(DirectX::XMMATRIX&);
        void GetWorldMatrix(DirectX::XMMATRIX&);
        void GetOrthoMatrix(DirectX::XMMATRIX&);

        void GetVideoCardInfo(char*, int&);

        void SetBackbufferRenderTarget();
        void ResetViewPort();
    private:
		
        bool GetRefreshRate(int&,int&,unsigned int&, unsigned int&);
        bool CreateSwapChain(int&, int&,bool,unsigned int,unsigned int,HWND);
        bool SetUpViewPort(int&, int&);
        bool CreateDepthStencil(int&,int&);
        bool SetRasterState();
        bool SetMatrix(int&, int&,float&,float&);
    private:
        bool m_vsync_enabled;
        int  m_videocardmemory;
        char m_videocarddescription[128];

        IDXGISwapChain*          m_swapChain;
        ID3D11Device*            m_device;
        ID3D11DeviceContext*     m_deviceContext;

        ID3D11RenderTargetView*  m_RenderTargetView;
        ID3D11DepthStencilView*  m_depthStencilView;

        ID3D11Texture2D*         m_depthstencilbuffer;
       
        ID3D11DepthStencilState* m_depthStencilState;
        ID3D11RasterizerState*   m_RasterState;

        DirectX::XMMATRIX m_projectionMatrix;
        DirectX::XMMATRIX m_orthographicMatrix;
        DirectX::XMMATRIX m_WorldMatrix;

        
        D3D11_VIEWPORT m_viewport;
        HRESULT m_result;
        //TODO: Get _com_error to print out string interpretation of the HRESULT.FAILED
    
};


#endif // !_D3D_CLASS_H_
