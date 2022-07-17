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
#include <Windows.h>
#include <string>
#include <array>
//====================//

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

	UINT32 get(void) const {
		return m_rgbColor;
	}

private:

	UINT32 m_rgbColor;

};

struct VERTEX {
	
	VERTEX()
		: x(0), y(0)
	{ /*...*/ }
	
	VERTEX(INT32 _x, INT32 _y)
		: x(_x), y(_y)
	{ /*...*/ }

	INT32 x;
	INT32 y;

};

struct TRIANGLE {

	TRIANGLE()
		: Vertex1({ }), Vertex2({ }), Vertex3({ })
	{ /*...*/
	}

	TRIANGLE(VERTEX _Vertex1, VERTEX _Vertex2, VERTEX _Vertex3)
		: Vertex1(_Vertex1), Vertex2(_Vertex2), Vertex3(_Vertex3)
	{ /*...*/ }

	VERTEX Vertex1;
	VERTEX Vertex2;
	VERTEX Vertex3;

};

struct GdiPlus {

	#pragma region DrawFigures

	static void DrawLine(HDC hdc, const VERTEX &lineBegin, const VERTEX &lineEnd, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr); // # Move to starting point #
		LineTo(hdc, lineEnd.x, lineEnd.y); // # Draw line to ending point #
		
		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawRectangle(HDC hdc, const RECT &rect, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT RectangleVertices[] = {
			{ rect.left, rect.top },
			{ rect.right, rect.top },
			{ rect.right, rect.bottom },
			{ rect.left, rect.bottom }
		};

		BYTE VerticesTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, RectangleVertices, VerticesTypes, ARRAYSIZE(RectangleVertices));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}
	
	static void DrawRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U){
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);
		
		POINT RectangleVertices[] = {
			{ X, Y },
			{ X + Width, Y },
			{ X + Width, Y + Height },
			{ X, Y + Height }
		};
		
		BYTE VerticesTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};
		
		PolyDraw(hdc, RectangleVertices, VerticesTypes, ARRAYSIZE(RectangleVertices));
		
		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);
		
	}

	static void DrawCircle(HDC hdc, const VERTEX &centerPoint, UINT32 radius, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, centerPoint.x + radius, centerPoint.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, centerPoint.x, centerPoint.y, radius, 0.0F, 360.0F); // # Draw circle #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawTriangle(HDC hdc, const TRIANGLE &triangle, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT TriangleVertices[] = {
			{ triangle.Vertex1.x, triangle.Vertex1.y },
			{ triangle.Vertex2.x, triangle.Vertex2.y },
			{ triangle.Vertex3.x, triangle.Vertex3.y }
		};

		BYTE VerticesTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, TriangleVertices, VerticesTypes, ARRAYSIZE(TriangleVertices));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawStar(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU strokeColor = ColorU::Enum::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT StarVertices[] = {
			{ X, Y + Height },
			{ X + Width / 2L, Y },
			{ X + Width, Y + Height },
			{ X, Y + Height / 3L},
			{ X + Width, Y + Height / 3L}
		};

		BYTE VerticesTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, StarVertices, VerticesTypes, ARRAYSIZE(StarVertices));

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	#pragma endregion

	#pragma region DrawImages

	// # This Function Draw Image From File With <.bmp> Extension #
	// ## Type 'f' - Load Image From File | Type 'r' - Load Image From Resource ##
	static bool DrawImage(HDC hdc, std::wstring_view FileName, CHAR Type, INT32 X, INT32 Y, INT32 Width, INT32 Height) noexcept {

		HANDLE ImageBitmap = NULL;
		
		if (Type == 'f') {
			ImageBitmap = LoadImageW(NULL, FileName.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_VGACOLOR); // # Load Image #
		} else if (Type == 'r') {
			HINSTANCE hInstance = GetModuleHandle(NULL);
			ImageBitmap = LoadImageW(hInstance, FileName.data(), IMAGE_BITMAP, 0, 0, LR_VGACOLOR); // # Load Image Resource #
		}

		if (ImageBitmap == NULL) {
			return false;
		}

		HDC BitmapDC = CreateCompatibleDC(hdc);

		HGDIOBJ PrevBitmap = SelectObject(BitmapDC, ImageBitmap);
		BitBlt(hdc, X, Y, Width, Height, BitmapDC, Width, Height, SRCCOPY); // # Draw Image #
		SelectObject(BitmapDC, PrevBitmap);

		DeleteDC(BitmapDC);
		DeleteObject(ImageBitmap);

		return true;

	}

	#pragma endregion

	
	#pragma region FillFigures

	static void FillRectangle(HDC hdc, const RECT &rect, ColorU FillColor = ColorU::Enum::DarkBlue) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor.get());
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		FillRect(hdc, &rect, FillBrush);
		
		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	static void FillRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU FillColor = ColorU::Enum::DarkBlue) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor.get());
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		RECT rect = { X, Y, X + Width, Y + Height };
		FillRect(hdc, &rect, FillBrush);

		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	template<size_t ArraySize>
	static void FillGradientH(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, const std::array<ColorU, ArraySize> &ColorCollection) noexcept {

		static_assert(ArraySize >= 2, "In \"ColorCollection\" Array Must Be Minimum TWO Colors");

		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Width, Height);

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientWidth = Width / (ArraySize - 1); // * Gradient Width Between TWO Colors *

		INT32 GradientBegin = 0; // * Gradient Starting Point *
		INT32 GradientEnd = GradientWidth; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {
			
			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)
				
				{
					GradientBegin, 0,                         // X, Y
					GetRValue(ColorCollection[i].get()) << 8, // Red
					GetGValue(ColorCollection[i].get()) << 8, // Green
					GetBValue(ColorCollection[i].get()) << 8  // Blue
				},

				{
					GradientEnd, Height,                      // X, Y
					GetRValue(ColorCollection[i + 1].get()) << 8, // Red
					GetGValue(ColorCollection[i + 1].get()) << 8, // Green
					GetBValue(ColorCollection[i + 1].get()) << 8  // Blue
				}

				#pragma warning(default:4838)
			};
			
			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_H);

			GradientBegin += GradientWidth;
			GradientEnd += GradientWidth;

		}

		BitBlt(hdc, X, Y, Width, Height, MemoryDC, Width, Height, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);
		
		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);

	}

	template<size_t ArraySize>
	static void FillGradientV(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, const std::array<ColorU, ArraySize> &ColorCollection) {

		static_assert(ArraySize >= 2, "In \"ColorCollection\" Array Must Be Minimum TWO Colors");

		HDC MemoryDC = CreateCompatibleDC(hdc);
		HBITMAP Bitmap = CreateCompatibleBitmap(hdc, Width, Height);

		HGDIOBJ PrevBitmap = SelectObject(MemoryDC, Bitmap);

		const INT32 GradientHeight = Height / (ArraySize - 1); // * Gradient Height Between TWO Colors *

		INT32 GradientBegin = 0; // * Gradient Starting Point *
		INT32 GradientEnd = GradientHeight; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {

			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)

				{
					0, GradientBegin,                         // X, Y
					GetRValue(ColorCollection[i].get()) << 8, // Red
					GetGValue(ColorCollection[i].get()) << 8, // Green
					GetBValue(ColorCollection[i].get()) << 8  // Blue
				},

				{
					Width, GradientEnd,                       // X, Y
					GetRValue(ColorCollection[i + 1].get()) << 8, // Red
					GetGValue(ColorCollection[i + 1].get()) << 8, // Green
					GetBValue(ColorCollection[i + 1].get()) << 8  // Blue
				}

				#pragma warning(default:4838)
			};

			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(MemoryDC, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_V);

			GradientBegin += GradientHeight;
			GradientEnd += GradientHeight;

		}

		BitBlt(hdc, X, Y, Width, Height, MemoryDC, Width, Height, SRCCOPY);

		SelectObject(MemoryDC, PrevBitmap);

		DeleteDC(MemoryDC);
		DeleteObject(Bitmap);

	}

	#pragma endregion

};
