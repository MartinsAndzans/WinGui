#pragma once

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#include <ciso646>
#include <Windows.h>
#include <string>
#include <vector>

#define DIRECTION_UP 0x01
#define DIRECTION_DOWN 0x02
#define DIRECTION_LEFT 0x03
#define DIRECTION_RIGHT 0x04

enum ColorU {
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
	DarkBlue = RGB(0, 0, 145)
};

typedef struct GDI_TRIANGLE {
	POINT point1;
	POINT point2;
	POINT point3;
}*LPGDI_TRIANGLE, *PGDI_TRIANGLE;

class GdiPlus {
public:

	static VOID DrawLine(HDC hdc, POINT begin, POINT end, uint32_t line_width = 1U, COLORREF line_color = ColorU::Red) noexcept {

		HPEN LinePen = CreatePen(PS_SOLID, line_width, line_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, LinePen));

		MoveToEx(hdc, begin.x, begin.y, nullptr); // # Move to starting point #
		LineTo(hdc, end.x, end.y); // # Draw line to ending point #

		SelectObject(hdc, PreviousPen);
		DeleteObject(LinePen);

	}

	static VOID DrawRectangle(HDC hdc, RECT rect, uint32_t line_width = 1U, COLORREF border_color = ColorU::Red) noexcept {

		HPEN LinePen = CreatePen(PS_SOLID, line_width, border_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, LinePen));

		// TOP
		MoveToEx(hdc, rect.left, rect.top, nullptr);
		LineTo(hdc, rect.right, rect.top);
		// BOTTOM
		MoveToEx(hdc, rect.left, rect.bottom, nullptr);
		LineTo(hdc, rect.right, rect.bottom);
		// LEFT
		MoveToEx(hdc, rect.left, rect.top, nullptr);
		LineTo(hdc, rect.left, rect.bottom);
		// RIGHT
		MoveToEx(hdc, rect.right, rect.top, nullptr);
		LineTo(hdc, rect.right, rect.bottom);

		SelectObject(hdc, PreviousPen);
		DeleteObject(LinePen);


	}

	static VOID DrawCircle(HDC hdc, POINT center, uint32_t radius, uint32_t line_width = 1U, COLORREF border_color = ColorU::Red) noexcept {

		HPEN LinePen = CreatePen(PS_SOLID, line_width, border_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, LinePen));

		MoveToEx(hdc, center.x + radius, center.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, center.x, center.y, radius, 0.0F, 360.0F); // # Draw Circle #

		SelectObject(hdc, PreviousPen);
		DeleteObject(LinePen);

	}

	static VOID DrawTriangle(HDC hdc, GDI_TRIANGLE triangle, uint32_t line_width = 1U, COLORREF border_color = ColorU::Red) noexcept {
		
		HPEN LinePen = CreatePen(PS_SOLID, line_width, border_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, LinePen));

		POINT TrianglePoints[] = {
			triangle.point1,
			triangle.point2,
			triangle.point3
		};

		BYTE PointTypes[] = {
			PT_MOVETO,
			PT_LINETO,
			PT_LINETO | PT_CLOSEFIGURE
		};

		PolyDraw(hdc, TrianglePoints, PointTypes, ARRAYSIZE(TrianglePoints));

		SelectObject(hdc, PreviousPen);
		DeleteObject(LinePen);

	}

	static void DrawStar(HDC hdc, RECT rect, uint32_t strokeWidth = 1U, COLORREF color = ColorU::Red) {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
		HGDIOBJ PreviousPen = SelectObject(hdc, StrokePen);

		POINT StarPoints[] = {
			{ rect.left, rect.bottom },
			{  },
			{  },
			{  },
			{  }
		};

	}

};
