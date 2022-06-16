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

#include <thread>
#include <chrono>

enum ColorU : COLORREF {
	
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

	static void DrawLine(HDC hdc, POINT lineBegin, POINT lineEnd, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, lineBegin.x, lineBegin.y, nullptr); // # Move to starting point #
		LineTo(hdc, lineEnd.x, lineEnd.y); // # Draw line to ending point #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawRectangle(HDC hdc, const RECT &rect, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
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
	
	static void DrawRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U){
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
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

	static void DrawCircle(HDC hdc, POINT centerPoint, UINT32 radius, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
		HGDIOBJ PrevPen = SelectObject(hdc, StrokePen);

		MoveToEx(hdc, centerPoint.x + radius, centerPoint.y, nullptr); // # Move to starting point of a circle #
		AngleArc(hdc, centerPoint.x, centerPoint.y, radius, 0.0F, 360.0F); // # Draw circle #

		SelectObject(hdc, PrevPen);
		DeleteObject(StrokePen);

	}

	static void DrawTriangle(HDC hdc, const TRIANGLE &triangle, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {
		
		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
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

	static void DrawStar(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, COLORREF strokeColor = ColorU::DarkBlue, UINT32 strokeWidth = 1U) noexcept {

		HPEN StrokePen = CreatePen(PS_SOLID, strokeWidth, strokeColor);
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

		HBITMAP ImageBitmap = static_cast<HBITMAP>(LoadImageW(NULL, FilePath, IMAGE_BITMAP, Width, Height, LR_LOADFROMFILE | LR_VGACOLOR)); // # Load Image #
		if (ImageBitmap == NULL)
			return false;

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

	static void FillRectangle(HDC hdc, const RECT &rect, HBRUSH FillBrush) noexcept {

		FillRect(hdc, &rect, FillBrush);

	}

	static void FillRectangle(HDC hdc, const RECT &rect, COLORREF FillColor = ColorU::DarkBlue) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor);
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		FillRect(hdc, &rect, FillBrush);

		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	static void FillRectangle(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, COLORREF FillColor = ColorU::DarkBlue) noexcept {

		HBRUSH FillBrush = CreateSolidBrush(FillColor);
		HGDIOBJ PrevBrush = SelectObject(hdc, FillBrush);

		RECT rect = { X, Y, X + Width, Y + Height };
		FillRect(hdc, &rect, FillBrush);

		SelectObject(hdc, PrevBrush);
		DeleteObject(FillBrush);

	}

	template<size_t ArraySize>
	static void FillGradient(HDC hdc, INT32 X, INT32 Y, INT32 Width, INT32 Height, const std::array<COLORREF, ArraySize> &ColorCollection) noexcept {

		static_assert(ArraySize >= 2, "-/ERROR/- In \"ColorCollection\" Array Must Be Minimum 2 Colors");

		const INT32 GradientWidth = Width / (ArraySize - 1); // * Gradient Width Between 2 Colors *

		INT32 GradientBeginX = X; // * Gradient Starting Point *
		INT32 GradientEndX = X + GradientWidth; // * Gradient Ending Point *

		for (size_t i = 0U; i < ColorCollection.size() - 1U; i++) {
			
			TRIVERTEX Gradient[] = {
				#pragma warning(disable:4838)
				{ GradientBeginX, Y, GetRValue(ColorCollection[i]) << 8, GetGValue(ColorCollection[i]) << 8, GetBValue(ColorCollection[i]) << 8 },
				{ GradientEndX, Y + Height, GetRValue(ColorCollection[i + 1]) << 8, GetGValue(ColorCollection[i + 1]) << 8, GetBValue(ColorCollection[i + 1]) << 8 },
				#pragma warning(default:4838)
			};

			GRADIENT_RECT GradientRectangle[] = { { 0, 1 } };

			GradientFill(hdc, Gradient, ARRAYSIZE(Gradient), GradientRectangle, ARRAYSIZE(GradientRectangle), GRADIENT_FILL_RECT_H);

			GradientBeginX += GradientWidth;
			GradientEndX += GradientWidth;

		}

	}

	#pragma endregion

	struct DIALOG {
		DLGTEMPLATE DlgProp;
		WORD DlgMenu;
		WORD DlgClass;
		WCHAR Title[7];
		WORD TitleLength;
	};

	static INT_PTR ShowDialog(HWND ParentWindow, const std::string &Title) {

		HINSTANCE hInstance = GetModuleHandle(NULL);

		DIALOG dlg = { 0 };
		dlg.DlgProp.dwExtendedStyle = WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE;
		dlg.DlgProp.style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU | DS_3DLOOK | DS_CENTER | DS_MODALFRAME;
		dlg.DlgProp.cdit = 0;
		dlg.DlgProp.x = 0;
		dlg.DlgProp.y = 0;
		dlg.DlgProp.cx = 200;
		dlg.DlgProp.cy = 60;
		dlg.DlgMenu = 0x0000;
		dlg.DlgClass = 0x0000;
		wcscpy_s(dlg.Title, L"Dialog");
		dlg.TitleLength = 7;
		
		return DialogBoxIndirectW(hInstance, reinterpret_cast<LPDLGTEMPLATE>(&dlg), ParentWindow, DlgProc);

	}

	static LRESULT CALLBACK DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam) {
		switch (Msg) {
		case WM_INITDIALOG:
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);
			GdiPlus::DrawCircle(hdc, { 40, 40 }, 20);
			EndPaint(hDlg, &ps);
			return 0;
		}
		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return 0;
		}
		return 0;
	}

};

//class DlgBox {
//private:
//
//public:
//	bool Create(const std::wstring &Title, int32_t X, int32_t Y, int32_t Width, int32_t Height) {
//
//		HANDLE DlgTemplate = LocalAlloc(LMEM_ZEROINIT, 1024);
//		if (DlgTemplate == NULL)
//			return false;
//
//		void *lpDlgTemplate = LocalLock(DlgTemplate);
//
//		DLGTEMPLATE *Dlg = reinterpret_cast<DLGTEMPLATE*>(lpDlgTemplate);
//		
//
//
//	}
//};

class WaitAnimation {
private:

	HWND hUpdateWindow = nullptr; // * Handle to window *
	RECT rcCanvas = { 0 }; // * Where draw animation *

	enum class ObjectDirection { Right = 0x01, Left = 0x02 };
	ObjectDirection ObjectDirection = ObjectDirection::Right;
	
	POINT ptObject = { 0 };
	INT32 Radius = 0;
	UINT16 ObjectSpeed = 4U;

public:

	void Start(_In_ HWND _hUpdateWindow, _In_ const RECT &_rcCanvas) {

		hUpdateWindow = _hUpdateWindow; 
		rcCanvas = _rcCanvas;

		ptObject = { _rcCanvas.left + (_rcCanvas.right - _rcCanvas.left) / 4, _rcCanvas.top + (_rcCanvas.bottom - _rcCanvas.top) / 2 };
		Radius = (_rcCanvas.bottom - _rcCanvas.top) / 6;

		constexpr UINT16 FPS = 1000UL / 60UL; // * 60 FPS *
		SetTimer(hUpdateWindow, reinterpret_cast<UINT_PTR>(this), FPS, UpdateFrame);

	}

	void Stop() {
		KillTimer(hUpdateWindow, reinterpret_cast<UINT_PTR>(this));
	}

private:

	static void _stdcall UpdateFrame(HWND hUpdateWindow, UINT Msg, UINT_PTR nIDEvent, DWORD SystemTime) {

		WaitAnimation *obj = reinterpret_cast<WaitAnimation*>(nIDEvent);

		PAINTSTRUCT ps = { 0 };
		InvalidateRect(hUpdateWindow, &obj->rcCanvas, false);
		HDC WindowDC = BeginPaint(hUpdateWindow, &ps);

		GdiPlus::FillRectangle(WindowDC, obj->rcCanvas, ColorU::LightBlue);
		GdiPlus::DrawRectangle(WindowDC, obj->rcCanvas, ColorU::DarkBlue, 4U);

		GdiPlus::DrawCircle(WindowDC, obj->ptObject, obj->Radius, ColorU::DarkBlue, 2U);

		if (obj->ptObject.x - obj->Radius <= obj->rcCanvas.left) {
			obj->ObjectDirection = ObjectDirection::Right;
		} else if (obj->ptObject.x + obj->Radius >= obj->rcCanvas.right) {
			obj->ObjectDirection = ObjectDirection::Left;
		}

		if (obj->ObjectDirection == ObjectDirection::Right) {
			obj->ptObject.x += obj->ObjectSpeed;
			obj->ptObject.x += obj->ObjectSpeed;
		}

		if (obj->ObjectDirection == ObjectDirection::Left) {
			obj->ptObject.x -= obj->ObjectSpeed;
			obj->ptObject.x -= obj->ObjectSpeed;
		}

		EndPaint(hUpdateWindow, &ps);
		ValidateRect(hUpdateWindow, &obj->rcCanvas);

	}

};

/*uint64_t operator"" sqr(uint64_t value) {
	return value * value;
}*/
