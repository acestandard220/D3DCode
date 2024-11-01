#ifndef _CAMERA_CLASS_H_
#define _CAMERA_CLASS_H_

#include <DirectXMath.h>

class CameraClass {
    public:
        CameraClass();
        CameraClass(const CameraClass& other);
        ~CameraClass();

        void SetPosition(float, float, float);
        void SetRotation(float, float, float);

        DirectX::XMFLOAT3 GetPosition() const;
        DirectX::XMFLOAT3 GetRotation() const;

        void Render();
        void GetViewMatrix(DirectX::XMMATRIX& view);
    private:
        float m_position_x, m_position_y, m_position_z;
        float m_rotation_x, m_rotation_y, m_rotation_z;

        DirectX::XMMATRIX m_viewmatrix;
};

#endif // !_CAMERA_CLASS_H_
