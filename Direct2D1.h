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
#include <stdexcept>
#include <Windows.h>
#include <vector>
#include <wrl.h> // Smart COM Pointer
#include <d2d1.h>
#include <string>
//====================//

#define D2D1RGB(r, g, b) (UINT32)((((DWORD)(BYTE)(r)) << 16) | ((WORD)(BYTE)(g) << 8) | ((BYTE)(b)))
#define D2D1GetRValue(rgb) (BYTE)((rgb) >> 16)
#define D2D1GetGValue(rgb) (BYTE)((rgb) >> 8)
#define D2D1GetBValue(rgb) (BYTE)((rgb))

// # Geometry Vertex #
struct VERTEX_2F {
	
	VERTEX_2F()
		: x(0.0F), y(0.0F)
	{ /*...*/ }

	VERTEX_2F(FLOAT _x, FLOAT _y)
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

class Direct2D1 {
public:

	explicit Direct2D1(D2D1_FACTORY_TYPE FactoryType);
	Direct2D1(const Direct2D1 &other) = delete;

	void BeginDraw(HDC hdc, const RECT &DrawRect, const D2D1_COLOR_F &ClearColor);
	void EndDraw(void);

	void DrawGeometry(const std::vector<VERTEX_2F> &VertexBuffer, const D2D1_COLOR_F &Color, FLOAT strokeWidth = 1.0F, const D2D1_MATRIX_3X2_F *Transform = nullptr);
	void FillGeometry(const std::vector<VERTEX_2F> &VertexBuffer, const D2D1_COLOR_F &Color, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	void DrawEllipse(const D2D1_POINT_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color, FLOAT strokeWidth = 1.0F, const D2D1_MATRIX_3X2_F *Transform = nullptr);
	void FillEllipse(const D2D1_POINT_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	void DrawBitmap(D2D1_RECT_F Rect, const D2D1Bitmap &Bitmap, INT32 Frame = 0, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	~Direct2D1(void) noexcept = default;

private:

	void CreatePathGeometry(const std::vector<VERTEX_2F> &VertexBuffer, ID2D1PathGeometry **PathGeometry);

private:

	Microsoft::WRL::ComPtr<ID2D1Factory> m_d2d1Factory;
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> m_dcRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_dcSolidColorBrush;

};
