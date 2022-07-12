#pragma once

//===== LIBRARYS =====//
#pragma comment(lib, "d2d1.lib") // DirectX 2D Library
//====================//

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#include <assert.h>
#include <stdexcept>
#include <Windows.h>
#include <vector>
#include <wrl.h> // Smart COM Pointer
#include <d2d1.h>
#include <string>
//====================//

#define dynamic_assert(condition, message) !(condition) ? (_wassert(#message), _CRT_WIDE(__FILE__), (UINT)(__LINE__)) : 

#define D2D1RGB(r, g, b) (UINT32)((((DWORD)(BYTE)(r)) << 16) | ((WORD)(BYTE)(g) << 8) | ((BYTE)(b)))
#define D2D1GetRValue(rgb) (BYTE)((rgb) >> 16)
#define D2D1GetGValue(rgb) (BYTE)((rgb) >> 8)
#define D2D1GetBValue(rgb) (BYTE)((rgb))

#define D2D1ScreenToLocal(p1, p2) p2.x -= p1.x; p1.x = 0; p2.y -= p1.y * (-1); p1.y = 0;

// # Geometry Vertex #
struct VERTEX {
	
	VERTEX()
		: x(0.0F), y(0.0F)
	{ /*...*/ }

	VERTEX(FLOAT _x, FLOAT _y)
		: x(_x), y(_y)
	{ /*...*/ }
	
	FLOAT x;
	FLOAT y;

};

struct D2D1Bitmap {
public:



private:

	Microsoft::WRL::ComPtr<IWICBitmapSource> m_wicBitmapSource;

};

// # Fill Modes # 
enum class MODE {
	DRAW = 0x05,
	FILL = 0x0F
};

class Direct2D1 {
public:

	explicit Direct2D1(D2D1_FACTORY_TYPE FactoryType);
	Direct2D1(const Direct2D1 &other) = delete;

	void BeginDraw(HDC DeviceContext, const RECT &DrawingSurface, D2D1_COLOR_F ClearColor);
	void EndDraw(void);

	bool DrawGeometry(const std::vector<VERTEX> &Vertecies, D2D1_COLOR_F Color, FLOAT strokeWidth = 1.0F,
		MODE Mode = MODE::DRAW, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	void DrawEllipse(D2D1_POINT_2F centerPoint, FLOAT RadiusX, FLOAT RadiusY, D2D1_COLOR_F Color, FLOAT strokeWidth = 1.0F,
		MODE Mode = MODE::DRAW, const D2D1_MATRIX_3X2_F *Transform = nullptr);
	
	void DrawBitmap(D2D1_RECT_F Rect, const D2D1Bitmap &Bitmap, INT32 Frame = 0, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	~Direct2D1(void) noexcept = default;

private:

	Microsoft::WRL::ComPtr<ID2D1Factory> m_d2d1Factory;
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> m_dcRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_dcSolidColorBrush;

};
