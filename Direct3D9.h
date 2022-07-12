#pragma once

//===== LIBRARYS =====//
#pragma comment(lib, "d3d9.lib") // # DirectX 3D Version 9.0 #
#pragma comment(lib, "d3dx9.lib") // # DirectX 3D SDK Version 9.0 #
//====================//

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#include <stdexcept>
#include <Windows.h>
#include <d3d9.h> // # DirectX 3D Version 9.0 #
#include <d3dx9math.h> // # DirectX 3D SDK Version 9.0 #
#include <string>
//====================//

class Direct3D9 {
public:

	explicit Direct3D9(_In_ HWND hRenderWindow, _In_ BOOL Windowed);
	Direct3D9(const Direct3D9 &other) = delete;

	void BeginRender(D3DCOLOR ClearColor) noexcept;
	void EndRender(void) noexcept;

	void SetVirtualCamera(const D3DXVECTOR3 &VirtualCameraPos, const D3DXVECTOR3 &VirtualCameraLookAt, FLOAT FOV, FLOAT ZNearPlane, FLOAT ZFarPlane);
	IDirect3DDevice9* GetDevice(void) const;

	~Direct3D9(void) noexcept;

private:

	HWND m_hRenderWindow;
	IDirect3D9 *m_Direct3d;
	IDirect3DDevice9 *m_Direct3dDevice;

};
