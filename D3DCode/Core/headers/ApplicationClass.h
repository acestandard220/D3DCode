#pragma once
#include <windows.h>
#include <iostream>

#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "ModelClass.h"
#include "CameraClass.h"

const bool FULL_SCREEN = false;
const bool V_SYNC = true;
const float Near = 0.3f;
const float Far = 1000.0f;


class ApplicationClass {
    public:

		ApplicationClass();
		ApplicationClass(const ApplicationClass& a);
		~ApplicationClass() {};
	    
		bool Initialize(int& screenWidth, int& screenHeight, HWND& hwnd);
	    bool Shutdown();
	    bool Frame();
    
private:
	    bool Render();

    private:
		D3DClass* m_d3dclass;
		ColorShaderClass* m_colorshaderclass;
		ModelClass* m_modelclass;
		CameraClass* m_cameraclass;
		bool m_result;
};
