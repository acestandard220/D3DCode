#ifndef _COLOR_SHADER_CLASS_H_
#define _COLOR_SHADER_CLASS_H_

#include <DirectXMath.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <iostream>

class ColorShaderClass {

    public:
	    struct Matrixes 
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;   
	    };
		ColorShaderClass();
		ColorShaderClass(const ColorShaderClass& other);
		~ColorShaderClass();

		bool Initialize(ID3D11Device*, HWND);
		void Shutdown();
		bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&);

    private:
		bool InitializeShaders(ID3D11Device*, HWND, WCHAR*, WCHAR*);
		void ShutdownShaders();
		void OutputShaderErrorMessage(ID3DBlob*, HWND, WCHAR*);

		bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
		void RenderShader(ID3D11DeviceContext*, int);

   private:
	   ID3D11VertexShader* m_vertexshader;
	   ID3D11PixelShader* m_pixelshader;
	   ID3D11InputLayout* m_layout;
	   ID3D11Buffer* m_matrixbuffer;

	   bool m_result;


};
#endif // !_COLOR_SHADER_CLASS_H_
