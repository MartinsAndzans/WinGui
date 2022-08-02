#pragma once

//===== LIBRARYS =====//
#pragma comment(lib, "msimg32.lib")
//====================//

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#include <memory> 
#include <Windows.h>
#include <string>
#include <array>
#include <vector>
//====================//

#undef RGB
// # 0x000000FF - Red / 0x0000FF00 - Green / 0x00FF0000 - Blue #
#define RGB(r, g, b) (UINT32)(((BYTE)(r))|((WORD)((BYTE)(g)) << 8)|((DWORD)((BYTE)(b)) << 16))

typedef PUINT64 LPUINT64;

class ColorU {
public:

	enum Enum : UINT32 {

		Aquamarine = RGB(0, 255, 150),

		White = RGB(255, 255, 255),
		Black = RGB(0, 0, 0),

		LightRed = RGB(255, 145, 145),
		Red = RGB(255, 0, 0),
		DarkRed = RGB(145, 0, 0),

		LightGreen = RGB(145, 255, 145),
		Green = RGB(0, 255, 0),
		DarkGreen = RGB(0, 145, 0),

		LightBlue = RGB(145, 145, 255),
		Blue = RGB(0, 0, 255),
		DarkBlue = RGB(0, 0, 145),

		Orange = RGB(255, 150, 0),
		Yellow = RGB(255, 255, 0),

	};

	// # Known Color #
	ColorU(Enum KnownColor) {
		m_rgbColor = KnownColor;
	}

	// # Color From Byte Values #
	explicit ColorU(BYTE Red, BYTE Green, BYTE Blue) {
		m_rgbColor = RGB(Red, Green, Blue);
	}

	ColorU(UINT32 rgbColor) {
		m_rgbColor = rgbColor;
	}

	// # Color From Normalized Float Values #
	explicit ColorU(FLOAT RedF, FLOAT GreenF, FLOAT BlueF) {
		
		BYTE Red, Green, Blue;

		RedF <= 0.0F ? Red = 0 : RedF >= 1.0F ? Red = 255 : Red = (BYTE)(RedF * 255);
		GreenF <= 0.0F ? Green = 0 : GreenF >= 1.0F ? Green = 255 : Green = (BYTE)(GreenF * 255);
		BlueF <= 0.0F ? Blue = 0 : BlueF >= 1.0F ? Blue = 255 : Blue = (BYTE)(BlueF * 255);
		
		m_rgbColor = RGB(Red, Green, Blue);
		
	}

	ColorU(const ColorU &other) {
		m_rgbColor = other.m_rgbColor;
	}

	UINT32 Get(void) const {
		return m_rgbColor;
	}

private:

	UINT32 m_rgbColor;

};

class GdiBrush {
public:

	// ===== PATTERN BRUSH =====
	explicit GdiBrush(HBITMAP hBitmap) {

		m_lpRefCount = new UINT64{};
		(*m_lpRefCount)++;
		m_hBrush = CreatePatternBrush(hBitmap);

	}

	// ===== SOLID BRUSH =====
	explicit GdiBrush(const ColorU &Color) {
		
		m_lpRefCount = new UINT64{};
		(*m_lpRefCount)++;
		m_hBrush = CreateSolidBrush(Color.Get());

	}

	GdiBrush(const GdiBrush &other) {

		m_lpRefCount = other.m_lpRefCount;
		(*m_lpRefCount)++;
		m_hBrush = other.m_hBrush;

	}

	HBRUSH Get(void) const {
		return m_hBrush;
	}

	~GdiBrush() noexcept {

		(*m_lpRefCount)--;
		
		if (m_lpRefCount == 0) {
			delete m_lpRefCount;
			DeleteObject(m_hBrush);
		}

	}
	

private:

	LPUINT64 m_lpRefCount;
	HBRUSH m_hBrush;

};

// # Geometry Vertex #
struct VERTEX_2U {
	
	VERTEX_2U()
		: x(0), y(0)
	{ /*...*/ }
	
	VERTEX_2U(INT32 _x, INT32 _y)
		: x(_x), y(_y)
	{ /*...*/ }

	VERTEX_2U(const VERTEX_2U &other) = default;

	INT32 x;
	INT32 y;

};

// # Triangle Geometry #
struct TRIANGLE_3U {

	TRIANGLE_3U() = default;

	TRIANGLE_3U(const VERTEX_2U &_Vertex1, const VERTEX_2U &_Vertex2, const VERTEX_2U &_Vertex3)
		: Vertex1(_Vertex1), Vertex2(_Vertex2), Vertex3(_Vertex3)
	{ /*...*/ }

	TRIANGLE_3U(const TRIANGLE_3U &other) = default;

	VERTEX_2U Vertex1;
	VERTEX_2U Vertex2;
	VERTEX_2U Vertex3;

};

// # Geometry Size #
struct SIZE_2U {

	SIZE_2U()
		: width(0), height(0)
	{ /*...*/ }

	SIZE_2U(INT32 _width, INT32 _height)
		: width(_width), height(_height)
	{ /*...*/ }

	SIZE_2U(const SIZE_2U &other) = default;

	INT32 width;
	INT32 height;

};

// # Image Load Properties #
enum class MODE : UINT16 {
	LOAD_FROM_FILE,
	LOAD_FROM_FILE_MONOCHROME,
	LOAD_FROM_RESOURCE,
	LOAD_FROM_RESOURCE_MONOCHROME
};

#include <winrt/Windows.Devices.Input.h>

struct GdiPlus {

	static void DrawRectangle(HDC hdc, const VERTEX_2U &Position, const SIZE_2U &Size,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.Get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT PointBuffer[] = {
			{ Position.x, Position.y },
			{ Position.x + Size.width, Position.y },
			{ Position.x + Size.width, Position.y + Size.height },
			{ Position.x, Position.y + Size.height }
		};

		BYTE PointTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawEllipse(HDC hdc, const VERTEX_2U &CenterPoint, UINT32 RadiusX, UINT32 RadiusY,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.Get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		ArcTo(hdc, CenterPoint.x - RadiusX, CenterPoint.y - RadiusY, CenterPoint.x + RadiusX, CenterPoint.y + RadiusY,
			CenterPoint.x + RadiusX, CenterPoint.y, CenterPoint.x + RadiusX, CenterPoint.y);

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawTriangle(HDC hdc, const TRIANGLE_3U &Triangle,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.Get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT PointBuffer[] = {
			{ Triangle.Vertex1.x, Triangle.Vertex1.y },
			{ Triangle.Vertex2.x, Triangle.Vertex2.y },
			{ Triangle.Vertex3.x, Triangle.Vertex3.y }
		};

		BYTE PointTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	template<size_t ArraySize>
	static void DrawGeometry(HDC hdc, const std::array<VERTEX_2U, ArraySize> &VertexBuffer,
		const ColorU &strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {
		
		static_assert(ArraySize >= 2, "At Least TWO Vertices is Required");

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.Get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT PointBuffer[ArraySize] = { 0 };
		BYTE PointTypes[ArraySize] = { 0 };

		for (size_t i = 0; i < ArraySize; i++) {

			PointBuffer[i].x = VertexBuffer[i].x;
			PointBuffer[i].y = VertexBuffer[i].y;

			if (i == 0) {
				PointTypes[i] = PT_MOVETO;
			} else if (i == ArraySize - 1) {
				PointTypes[i] = PT_LINETO | PT_CLOSEFIGURE;
			} else {
				PointTypes[i] = PT_LINETO;
			}

		}

		PolyDraw(hdc, PointBuffer, PointTypes, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void FillRectangle(HDC hdc, const VERTEX_2U &Position, const SIZE_2U &Size, const GdiBrush &dcBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, dcBrush.Get());

		POINT PointBuffer[] = {
			{ Position.x, Position.y },
			{ Position.x + Size.width, Position.y },
			{ Position.x + Size.width, Position.y + Size.height },
			{ Position.x, Position.y + Size.height }
		};

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));
		
		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);
		
	}

	static void FillEllipse(HDC hdc, const VERTEX_2U &CenterPoint, UINT32 RadiusX, UINT32 RadiusY, const GdiBrush &dcBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, dcBrush.Get());

		Ellipse(hdc, CenterPoint.x - RadiusX, CenterPoint.y - RadiusY, CenterPoint.x + RadiusX, CenterPoint.y + RadiusY);

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	static void FillTriangle(HDC hdc, const TRIANGLE_3U &Triangle, const GdiBrush &dcBrush) noexcept {

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, dcBrush.Get());

		POINT PointBuffer[] = {
			{ Triangle.Vertex1.x, Triangle.Vertex1.y },
			{ Triangle.Vertex2.x, Triangle.Vertex2.y },
			{ Triangle.Vertex3.x, Triangle.Vertex3.y }
		};

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	template<size_t ArraySize>
	static void FillGeometry(HDC hdc, const std::array<VERTEX_2U, ArraySize> &VertexBuffer, const GdiBrush &dcBrush) noexcept {

		static_assert(ArraySize >= 2, "At Least TWO Vertices is Required");

		HPEN InvisiblePen = (HPEN)(GetStockObject(NULL_PEN));

		HGDIOBJ PrevPen = SelectObject(hdc, InvisiblePen);
		HGDIOBJ PrevBrush = SelectObject(hdc, dcBrush.Get());
		
		POINT PointBuffer[ArraySize] = { 0 };
		
		for (size_t i = 0; i < ArraySize; i++) {
			PointBuffer[i].x = VertexBuffer[i].x;
			PointBuffer[i].y = VertexBuffer[i].y;
		}

		Polygon(hdc, PointBuffer, ARRAYSIZE(PointBuffer));

		SelectObject(hdc, PrevPen);
		SelectObject(hdc, PrevBrush);

	}

	/// <summary>This Function Draw Image From File with ".bmp" Extension or Application Resource</summary>
	/// <param name="FileName">More Information is Located in "Mode" Parameter Anotation</param>
	/// <param name="Mode"><para>LOAD_FROM_FILE - Load Image From File with ".bmp" Extension</para>
	/// <para>LOAD_FROM_RESOURCE - Load Image From Resource - Use "MAKEINTRESOURCE" Macro</para>
	/// <para>MONOCHROME - Load Monochrome Image (Black - White)</para></param>
	/// <returns><para>If Succeded Returns GDI_OK</para><para>If Failed Returns GDI Error Code</para></returns>
	static _Success_(return == true) bool DrawImage( _In_ HDC hdc, _In_z_ LPCTSTR FileName, _In_ MODE Mode,
		_In_ const VERTEX_2U &Position, _In_ const SIZE_2U &Size) noexcept {

		HANDLE ImageBitmap = NULL;
		HINSTANCE hInstance = GetModuleHandle(NULL);
		
		switch (Mode) {
		//--------------------------------------------------------------------------------------------------
		// Load Image From File
		case MODE::LOAD_FROM_FILE:
			ImageBitmap = LoadImage(NULL, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_LOADFROMFILE | LR_VGACOLOR);
			break;
		case MODE::LOAD_FROM_FILE_MONOCHROME:
			ImageBitmap = LoadImage(NULL, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_LOADFROMFILE | LR_MONOCHROME);
			break;
		//--------------------------------------------------------------------------------------------------
		// Load Image From Resource
		case MODE::LOAD_FROM_RESOURCE:
			ImageBitmap = LoadImage(hInstance, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_VGACOLOR);
			break;
		case MODE::LOAD_FROM_RESOURCE_MONOCHROME:
			ImageBitmap = LoadImage(hInstance, FileName, IMAGE_BITMAP, Size.width, Size.height, LR_MONOCHROME);
			break;
		//--------------------------------------------------------------------------------------------------
		}

		if (ImageBitmap == NULL) {
			return false;
		}

		HDC BitmapDC = CreateCompatibleDC(hdc);

		//--------------------------------------------------------------------------------------------------
		// Draw Image
		HGDIOBJ PrevBitmap = SelectObject(BitmapDC, ImageBitmap);
		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, BitmapDC, 0, 0, SRCCOPY);
		SelectObject(BitmapDC, PrevBitmap);
		//--------------------------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------------------------
		// Cleanup Resources
		DeleteDC(BitmapDC);
		DeleteObject(ImageBitmap);
		//--------------------------------------------------------------------------------------------------

		return true;

	}

	template<size_t ArraySize>
	static void FillGradientH(HDC hdc, const VERTEX_2U &Position, const SIZE_2U &Size, const std::array<ColorU, ArraySize> &ColorCollection) noexcept {

		static_assert(ArraySize >= 2, "At Least TWO Colors is Required");

		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Size.width, Size.height);

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientWidth = Size.width / (ArraySize - 1); // * Gradient Width Between TWO Colors *

		INT32 GradientBeginX = 0; // * Gradient Starting Point *
		INT32 GradientEndX = GradientWidth; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {
			
			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)
				
				{
					GradientBeginX, 0,                        // X, Y
					GetRValue(ColorCollection[i].Get()) << 8, // Red
					GetGValue(ColorCollection[i].Get()) << 8, // Green
					GetBValue(ColorCollection[i].Get()) << 8  // Blue
				},

				{
					GradientEndX, Size.height,                    // X, Y
					GetRValue(ColorCollection[i + 1].Get()) << 8, // Red
					GetGValue(ColorCollection[i + 1].Get()) << 8, // Green
					GetBValue(ColorCollection[i + 1].Get()) << 8  // Blue
				}

				#pragma warning(default:4838)
			};
			
			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_H);

			GradientBeginX += GradientWidth;
			GradientEndX += GradientWidth;

		}

		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);
		
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);

	}

	template<size_t ArraySize>
	static void FillGradientV(HDC hdc, const VERTEX_2U &Position, const SIZE_2U &Size, const std::array<ColorU, ArraySize> &ColorCollection) {

		static_assert(ArraySize >= 2, "At Least TWO Colors is Required");

		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Size.width, Size.height);

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientHeight = Size.height / (ArraySize - 1); // * Gradient Height Between TWO Colors *

		INT32 GradientBeginY = 0; // * Gradient Starting Point *
		INT32 GradientEndY = GradientHeight; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {

			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)

				{
					0, GradientBeginY,                        // X, Y
					GetRValue(ColorCollection[i].Get()) << 8, // Red
					GetGValue(ColorCollection[i].Get()) << 8, // Green
					GetBValue(ColorCollection[i].Get()) << 8  // Blue
				},

				{
					Size.width, GradientEndY,                     // X, Y
					GetRValue(ColorCollection[i + 1].Get()) << 8, // Red
					GetGValue(ColorCollection[i + 1].Get()) << 8, // Green
					GetBValue(ColorCollection[i + 1].Get()) << 8  // Blue
				}

				#pragma warning(default:4838)
			};

			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_V);

			GradientBeginY += GradientHeight;
			GradientEndY += GradientHeight;

		}

		BitBlt(hdc, Position.x, Position.y, Size.width, Size.height, MemoryDC, 0, 0, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);

		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);

	}

};
