#pragma once

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#include <Windows.h>
#include <string>
#include <array>

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

typedef struct TRIANGLE {
	POINT Vertex1;
	POINT Vertex2;
	POINT Vertex3;
}*LPTRIANGLE;

class GdiPlus {
public:

	#pragma region DrawFigures

	static void DrawLine(HDC hdc, POINT lineBegin, POINT lineEnd, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr); // # Move to starting point #
		LineTo(hdc, lineEnd.x, lineEnd.y); // # Draw line to ending point #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawRectangle(HDC hdc, RECT rect, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
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
	
	static void DrawRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U){
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
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

	static void DrawCircle(HDC hdc, POINT centerPoint, UINT32 radius, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, centerPoint.x + radius, centerPoint.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, centerPoint.x, centerPoint.y, radius, 0.0F, 360.0F); // # Draw circle #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawTriangle(HDC hdc, TRIANGLE triangle, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
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

	static void DrawStar(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, COLORREF color = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, color);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		POINT StarVertices[] = {
			{ X, Y + Width },
			{ X + Width / 2L, Y },
			{ X + Width, Y + Height },
			{ X, Y + Width / 3L},
			{ X + Width, Y + Width / 3L}
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
	
	#pragma region FillFigures
	
	template<int32_t ArraySize>
	static void FillGradient(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, std::array<COLORREF, ArraySize> ColorCollection) noexcept {
			 
		static_assert(ArraySize < 2, "-/ERROR/- In \"ColorCollection\" Array Must Be Minimum 2 Colors");

		INT32 GradientWidth = Width / ArraySize;
			 
		for (size_t i = 0U; i < ColorCollection.size() - 1U; i++) {
			 
			TRIVERTEX Gradient[] = {
				{  },
				{  },
			};
			 
			GRADIENT_RECT GradientRectangle = { 0, 1 };

			//GradientFill(hdc, )

		}

		//TRIVERTEX ColorCollection[] = {
		//	{ position.x, position.y, 0xFF00, 0x0000, 0x0000, 0x0000 }, // Red Color
		//	{ position.x, position.y, 0xFF00, 0xFF00, 0x0000, 0x0000 }, // Yellow Color
		//	{ position.x, position.y, 0x0000, 0xFF00, 0x0000, 0x0000 }, // Green Color
		//	{ position.x, position.y, 0x0000, 0xFF00, 0xFF00, 0x0000 }, // Light Blue Color
		//	{ position.x, position.y, 0x0000, 0x0000, 0xFF00, 0x0000 }, // Blue Color
		//	{ position.x, position.y, 0xFF00, 0x0000, 0xFF00, 0x0000 }, // Purple Color
		//	{ position.x, position.y, 0xFF00, 0x0000, 0x0000, 0x0000 } // Red Color
		//};

	}
	
	#pragma endregion

};

/*uint64_t operator"" sqr(uint64_t value) {
	return value * value;
}*/
