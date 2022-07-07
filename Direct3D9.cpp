#include "Direct3D9.h"

template<typename Interface>
void SafeRelease(Interface **D3DResource) {
	if (*D3DResource != nullptr) {
		(*D3DResource)->Release();
		*D3DResource = nullptr;
	}
}

Direct3D9::Direct3D9(_In_ HWND hRenderWindow, _In_ BOOL Windowed)
	: m_hRenderWindow(NULL), m_Direct3d(nullptr), m_Direct3dDevice(nullptr) {
	
	//===== Create Direct3D Interface =====//
	m_Direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_Direct3d == nullptr) {
		throw(std::runtime_error("[ERROR]: DirectX3D Interface Initilization Failed!"));
	}

	m_hRenderWindow = hRenderWindow;

	D3DPRESENT_PARAMETERS Direct3dDevParms = { 0 };
	Direct3dDevParms.hDeviceWindow = hRenderWindow;
	Direct3dDevParms.Windowed = Windowed;
	Direct3dDevParms.BackBufferCount = 1;
	Direct3dDevParms.BackBufferFormat = D3DFMT_A8R8G8B8;
	Direct3dDevParms.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//===== Create Direct3D Device =====//
	HRESULT Direct3dDeviceError = m_Direct3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hRenderWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&Direct3dDevParms, &m_Direct3dDevice
	);

	if (Direct3dDeviceError != D3D_OK) {
		SafeRelease(&m_Direct3d);
		throw(std::runtime_error("[ERROR]: DirectX3D Device Initilization Failed!\nError Code: /" +
			std::to_string(Direct3dDeviceError) + "/"));
	}

}

void Direct3D9::BeginRender(D3DCOLOR ClearColor) noexcept {

	m_Direct3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, ClearColor, 1.0F, 0);
	m_Direct3dDevice->BeginScene();

}

void Direct3D9::EndRender(void) noexcept {

	m_Direct3dDevice->EndScene();
	m_Direct3dDevice->Present(NULL, NULL, NULL, NULL);

}

void Direct3D9::SetVirtualCamera(const D3DXVECTOR3 &VirtualCameraPos, const D3DXVECTOR3 &VirtualCameraLookAt, FLOAT FieldOfView, FLOAT ZNearPlane, FLOAT ZFarPlane) {

	RECT ClientRect = { 0 };
	GetClientRect(m_hRenderWindow, &ClientRect);

	//===== Camera Angle =====//
	D3DXMATRIX ViewMatrix;
	ZeroMemory(ViewMatrix, sizeof(D3DXMATRIX));
	D3DXVECTOR3 VirtualCameraUp = { 0.0F, 1.0F, 0.0F };
	D3DXMatrixLookAtLH(&ViewMatrix, &VirtualCameraPos, &VirtualCameraLookAt, &VirtualCameraUp);
	m_Direct3dDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);

	//===== Camera View Projection On Screen =====//
	D3DXMATRIX ProjectionMatrix;
	ZeroMemory(ProjectionMatrix, sizeof(D3DXMATRIX));
	float_t FOV = D3DXToRadian(FieldOfView), AspectRatio = (FLOAT)(ClientRect.right) / (FLOAT)(ClientRect.bottom);
	D3DXMatrixPerspectiveLH(&ProjectionMatrix, FOV, AspectRatio, ZNearPlane, ZFarPlane);
	m_Direct3dDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);

}

IDirect3DDevice9* Direct3D9::GetDevice(void) const {
	return m_Direct3dDevice;
}

Direct3D9::~Direct3D9(void) noexcept {

	m_hRenderWindow = NULL;
	SafeRelease(&m_Direct3d);
	SafeRelease(&m_Direct3dDevice);

}
