#pragma once

//===== LIBRARYS =====//
#pragma comment(lib, "d2d1.lib") // DirectX 2D Library
//====================//

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#include <stdexcept>
#include <Windows.h>
#include <wrl.h> // Smart COM Pointer
#include <d2d1.h>
#include <string>
//====================//

#define D2D1RGB(r, g, b) (UINT32)((((DWORD)(BYTE)(r)) << 16) | ((WORD)(BYTE)(g) << 8) | ((BYTE)(b)))
#define D2D1GetRValue(rgb) (BYTE)((rgb) >> 16)
#define D2D1GetGValue(rgb) (BYTE)((rgb) >> 8)
#define D2D1GetBValue(rgb) (BYTE)((rgb))

class Direct2D1 {
public:

	Direct2D1(D2D1_FACTORY_TYPE FactoryType);
	Direct2D1(const Direct2D1 &other) = delete;

	void BeginDraw(HDC DeviceContext, const RECT &DrawingSurface, D2D1_COLOR_F ClearColor);
	void EndDraw(void);

	void DrawCircle(D2D1_POINT_2F centerPoint, FLOAT Radius, D2D1_COLOR_F Color, FLOAT strokeWidth = 1.0F, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	~Direct2D1(void) noexcept = default;

private:

	Microsoft::WRL::ComPtr<ID2D1Factory>(m_d2d1Factory);
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget>(m_dcRenderTarget);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>(m_dcSolidColorBrush);

};
