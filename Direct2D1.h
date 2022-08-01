#pragma once

//===== LIBRARYS =====//
#pragma comment(lib, "comsuppw.lib") // HRESULT Error Handling
#pragma comment(lib, "d2d1.lib") // DirectX 2D Library
//====================//

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#define WIN32_LEAN_AND_MEAN
#define COM_NO_WINDOWS_H

//===== HEADERS ======//
#pragma warning(disable:4005)
#include <stdexcept>
#include <Windows.h>
#include <Ole2.h>
#include <vector>
#include <wrl.h> // Smart COM Pointer
#include <d2d1.h>
#include <string>
#pragma warning(default:4005)
//====================//

// # 0x00FF0000 - Red / 0x0000FF00 - Green / 0x000000FF - Blue #
#define D2D1RGB(r, g, b) (UINT32)((((DWORD)(BYTE)(r)) << 16)|((WORD)(BYTE)(g) << 8)|((BYTE)(b)))
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

	VERTEX_2F(const VERTEX_2F &other) = default;
	
	FLOAT x;
	FLOAT y;

};

// # Image Rectangle #
struct SIZE_2F {
	
	SIZE_2F()
		: width(0.0F), height(0.0F)
	{ /*...*/ }

	SIZE_2F (FLOAT width_, FLOAT height_)
		: width(width_), height(height_)
	{ /*...*/ }

	SIZE_2F(const SIZE_2F &other) = default;

	FLOAT width;
	FLOAT height;

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

	void DrawEllipse(const VERTEX_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color, FLOAT strokeWidth = 1.0F, const D2D1_MATRIX_3X2_F *Transform = nullptr);
	void FillEllipse(const VERTEX_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	void DrawImage(const VERTEX_2F &Position, const SIZE_2F &Size, const D2D1Bitmap &Bitmap, INT32 Frame = 0, const D2D1_MATRIX_3X2_F *Transform = nullptr);

	~Direct2D1(void) noexcept = default;

private:

	void CreatePathGeometry(const std::vector<VERTEX_2F> &VertexBuffer, ID2D1PathGeometry **PathGeometry);

private:

	//===== CORE INTERFACES =====//
	Microsoft::WRL::ComPtr<ID2D1Factory> m_d2d1Factory;
	Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> m_dcRenderTarget;
	//===== SOLID COLOR BRUSH =====//
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_dcSolidColorBrush;

};
