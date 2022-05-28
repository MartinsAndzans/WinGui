#pragma once

#include <ciso646>
#include <Windows.h>
#include <string>
#include <vector>

enum KnownColors {
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
}*LPGDI_TRIANGLE, *PTRGDI_TRIANGLE;

class Gdi {
public:

	static VOID DrawLine(HDC hdc, POINT begin, POINT end, UINT line_width = 2U, COLORREF line_color = KnownColors::Black) noexcept {

		HPEN LinePen = CreatePen(PS_SOLID, line_width, line_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, LinePen));

		MoveToEx(hdc, begin.x, begin.y, nullptr);
		LineTo(hdc, end.x, end.y);

		SelectObject(hdc, PreviousPen);
		DeleteObject(LinePen);

	}

	static VOID DrawRectangle(HDC hdc, RECT rect, UINT border_width = 2U, COLORREF border_color = KnownColors::Black) noexcept {

		HPEN RectanglePen = CreatePen(PS_SOLID, border_width, border_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, RectanglePen));

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
		DeleteObject(RectanglePen);


	}

	static VOID DrawCircle(HDC hdc, POINT center, UINT radius, UINT border_width = 2U, COLORREF border_color = KnownColors::Black) noexcept {

		HPEN CirclePen = CreatePen(PS_SOLID, border_width, border_color);
		HPEN PreviousPen = static_cast<HPEN>(SelectObject(hdc, CirclePen));

		MoveToEx(hdc, center.x + radius, center.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, center.x, center.y, radius, 0.0F, 360.0F); // # Draw Circle #

		SelectObject(hdc, PreviousPen);
		DeleteObject(CirclePen);

	}

	static VOID DrawTriangle(HDC hdc, GDI_TRIANGLE triangle, UINT border_width = 2U, COLORREF border_color = KnownColors::Black) noexcept {
		
		

	}

};
