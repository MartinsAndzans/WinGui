#pragma once

#pragma comment(lib, "msimg32.lib")

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#include <Windows.h>
#include <string>
#include <array>

class ColorU {
public:

	enum Enum : UINT32 {

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
		Aquamarine = RGB(0, 255, 150)

	};

	// # Known Color #
	ColorU(Enum KnownColor) {
		m_rgbColor = KnownColor;
	}

	// # Color From Byte Values #
	ColorU(BYTE Red, BYTE Green, BYTE Blue) {
		m_rgbColor = RGB(Red, Green, Blue);
	}

	// # Color Format: 0xAABBGGRR #
	ColorU(UINT32 rgbColor) {
		m_rgbColor = rgbColor;
	}

	// # Color From Normalized Float Values #
	ColorU(FLOAT RedF, FLOAT GreenF, FLOAT BlueF) {
		
		BYTE Red, Green, Blue;

		RedF <= 0.0F ? Red = 0 : RedF >= 1.0F ? Red = 255 : Red = (BYTE)(RedF * 255);
		GreenF <= 0.0F ? Green = 0 : GreenF >= 1.0F ? Green = 255 : Green = (BYTE)(GreenF * 255);
		BlueF <= 0.0F ? Blue = 0 : BlueF >= 1.0F ? Blue = 255 : Blue = (BYTE)(BlueF * 255);
		
		m_rgbColor = RGB(Red, Green, Blue);
		
	}

	ColorU(const ColorU &Other) {
		m_rgbColor = Other.m_rgbColor;
	}

	UINT32 get(void) const {
		return m_rgbColor;
	}

private:

	UINT32 m_rgbColor;

};

typedef struct TRIANGLE {
	POINT Vertex1;
	POINT Vertex2;
	POINT Vertex3;
}*LPTRIANGLE;

typedef struct POINT3D {
	INT32 X;
	INT32 Y;
	INT32 Z;
}*LPPOINT3D;

struct GdiPlus {

	#pragma region DrawFigures

	static void DrawLine(HDC hdc, POINT lineBegin, POINT lineEnd, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr); // # Move to starting point #
		LineTo(hdc, lineEnd.x, lineEnd.y); // # Draw line to ending point #
		
		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawRectangle(HDC hdc, const RECT &rect, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) noexcept {

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
	
	static void DrawRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U){
		
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

	static void DrawCircle(HDC hdc, POINT centerPoint, UINT32 radius, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, centerPoint.x + radius, centerPoint.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, centerPoint.x, centerPoint.y, radius, 0.0F, 360.0F); // # Draw circle #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawTriangle(HDC hdc, const TRIANGLE &triangle, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) noexcept {
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT TriangleVertices[] = {
			triangle.Vertex1,
			triangle.Vertex2,
			triangle.Vertex3
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

	static void DrawStar(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) noexcept {

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

	// # This function draw image with this extensions ".bmp; .png" #
	static bool DrawImage(HDC hdc, LPCWSTR FilePath, INT32 X, INT32 Y, INT32 Width, INT32 Height) noexcept {

		HBITMAP ImageBitmap = (HBITMAP)(LoadImageW(NULL, FilePath, IMAGE_BITMAP, Width, Height, LR_LOADFROMFILE | LR_VGACOLOR)); // # Load Image #
		if (ImageBitmap == NULL) {
			return false;
		}

		HDC BitmapDC = CreateCompatibleDC(hdc);

		HGDIOBJ PrevBitmap = SelectObject(BitmapDC, ImageBitmap);
		BitBlt(hdc, X, Y, Width, Height, BitmapDC, 0, 0, SRCCOPY); // # Draw Image #
		SelectObject(BitmapDC, PrevBitmap);

		DeleteDC(BitmapDC);
		DeleteObject(ImageBitmap);

		return true;

	}

	#pragma endregion

	
	#pragma region FillFigures

	static void FillRectangle(HDC hdc, const RECT &rect, ColorU FillColor = ColorU(ColorU::Enum::DarkBlue)) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor.get());
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		FillRect(hdc, &rect, FillBrush);

		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	static void FillRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, ColorU FillColor = ColorU(ColorU::Enum::DarkBlue)) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor.get());
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		RECT rect = { X, Y, X + Width, Y + Height };
		FillRect(hdc, &rect, FillBrush);

		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	template<size_t ArraySize>
	static void FillGradient(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, const std::array<ColorU, ArraySize> &ColorCollection) noexcept {

		#define MAKECOLOR16(Byte) (COLOR16)(Byte) << 8

		static_assert(ArraySize >= 2, "[ERROR]: In \"ColorCollection\" Array Must Be Minimum 2 Colors");

		const INT32 GradientWidth = Width / (ArraySize - 1); // * Gradient Width Between 2 Colors *

		INT32 GradientBegin = X; // * Gradient Starting Point *
		INT32 GradientEnd = X + GradientWidth; // * Gradient Ending Point *

		for (size_t i = 0; i < ArraySize - 1; i++) {
			
			// COLOR16 0x0000 - # (COLOR16)(0xFF) << 8 : 0xFF00 #

			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)
				
				{
					GradientBegin, Y, 
					MAKECOLOR16(GetRValue(ColorCollection[i].get())),
					MAKECOLOR16(GetGValue(ColorCollection[i].get())),
					MAKECOLOR16(GetBValue(ColorCollection[i].get()))
				},

				{
					GradientEnd, Y + Height,
					MAKECOLOR16(GetRValue(ColorCollection[i + 1].get())),
					MAKECOLOR16(GetGValue(ColorCollection[i + 1].get())),
					MAKECOLOR16(GetBValue(ColorCollection[i + 1].get()))
				}

				#pragma warning(default:4838)
			};
			
			GRADIENT_RECT GradientRect[] = { { 0, 1 } };

			GradientFill(hdc, Gradient, ARRAYSIZE(Gradient), GradientRect, ARRAYSIZE(GradientRect), GRADIENT_FILL_RECT_H);

			GradientBegin += GradientWidth;
			GradientEnd += GradientWidth;

		}

		#undef MAKECOLOR16

	}

	#pragma endregion

	#pragma region 3DFigures

	static void DrawCube(HDC hdc, POINT3D centerPoint, UINT32 radius, UINT16 cameraAngle, ColorU strokeColor = ColorU(ColorU::Enum::DarkBlue), UINT32 strokeWidth = 1U) {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor.get());
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);



		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	#pragma endregion


};
