#include "WinGui.h"

//===== Main Window Properties =====//
HWND WinGui::hMainWindow = NULL; // * Handle to Main Window *
SIZE WinGui::MainWindowSize = { 0 }; // * Main Window Size *
//==================================//

Direct2D1 *WinGui::GraphicsDevice = nullptr;

bool WinGui::wCreateMainWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ INT nCmdShow) noexcept {
	
	HINSTANCE hInstance = GetModuleHandle(NULL); // * Instance of Application *
	constexpr LPCWSTR ClassName = L"MainClass"; // * Main Window Class *

	WinGui::MainWindowSize = { Width, Height };

	//===== Create Main Window Class =====//
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.hbrBackground = NULL;
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MAINCURSOR));
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProc;
	wcex.lpszClassName = ClassName;
	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	//====================================//

	//==== Register Main Window Class ====//
	if (!RegisterClassExW(&wcex)) {
		return false;
	}
	//====================================//

	INT32 X = GetSystemMetrics(SM_CXSCREEN) / 2 - WinGui::MainWindowSize.cx / 2; // * Window X Position *
	INT32 Y = GetSystemMetrics(SM_CYSCREEN) / 2 - WinGui::MainWindowSize.cy / 2; // * Window Y Position *

	//======== Create Main Window ========//
	WinGui::hMainWindow = CreateWindowExW(
		WS_EX_APPWINDOW,
		ClassName,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		X, Y, WinGui::MainWindowSize.cx, WinGui::MainWindowSize.cy,
		HWND_DESKTOP,
		NULL,
		hInstance,
		nullptr
	);
	//====================================//

	//===== Check For Window Creation Error =====//
	if (WinGui::hMainWindow == NULL) {
		return false;
	}
	//===========================================//

	ShowWindow(WinGui::hMainWindow, nCmdShow); // # Show Main Window #

	return true;

}

INT WinGui::wMainLoop(void) noexcept {

	MSG Msg = { 0 }; // * Message Structure *
	
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return static_cast<INT>(Msg.wParam);

}

struct POINTVERTEX {
	FLOAT X, Y, Z;
	DWORD Color;
};

enum class Fade { out, in };
Fade state = Fade::out;
COLORREF Color = D2D1RGB(0, 0, 255);

D2D1_POINT_2F circle_point = { 0 };

LRESULT CALLBACK WinGui::WindowProc(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_CREATE:
	{

		try {
			GraphicsDevice = new Direct2D1(D2D1_FACTORY_TYPE_SINGLE_THREADED);
		} catch (std::runtime_error &e) {
			MessageBoxA(hMainWindow, e.what(), "DirectX 2D Error", MB_ICONERROR);
			return -1;
		}

		INT16 FrameRate = (INT16)(1000.0F / 60.0F);
		SetTimer(hMainWindow, 0xFF, FrameRate, NULL);

		return 0;
		
	}
	case WM_TIMER:
	{

		if (Color == D2D1RGB(0, 0, 0)) {
			state = Fade::in;
		} else if (Color == D2D1RGB(0, 0, 255)) {
			state = Fade::out;
		}

		InvalidateRect(hMainWindow, NULL, FALSE);

		return 0;
	}
	case WM_PAINT:
	{

		RECT rect = { 0 };
		PAINTSTRUCT ps = { 0 };

		HDC WindowDC = BeginPaint(hMainWindow, &ps);
		GetClientRect(hMainWindow, &rect);

		SetBkMode(WindowDC, TRANSPARENT);

		if (circle_point.x == 0 && circle_point.y == 0) {
			circle_point = { rect.right / 2.0F, rect.bottom / 2.0F };
		} else {
			circle_point = { rect.right / 2.0F, rect.bottom / 2.0F };
		}

		if (state == Fade::out) {
			Color = D2D1RGB(D2D1GetRValue(Color), D2D1GetGValue(Color), D2D1GetBValue(Color) - 0x05);
		} else if (state == Fade::in) {
			Color = D2D1RGB(D2D1GetRValue(Color), D2D1GetGValue(Color), D2D1GetBValue(Color) + 0x05);
		}

		GraphicsDevice->BeginDraw(WindowDC, rect, D2D1::ColorF(Color));

		GraphicsDevice->DrawCircle(circle_point, 100.0F, D2D1::ColorF((rand() % 256) / 255.0F, (rand() % 256) / 255.0F, (rand() % 256) / 255.0F), 4.0F);
		


		GraphicsDevice->EndDraw();

		/* constexpr float_t X = 0.0F, Y = 0.0F, Z = 0.0F, CubeWidth = 10.0F, CubeHeight = 10.0F, CubeDepth = 10.0F;

		POINTVERTEX CubeVerticies[] = {
			// # Center Cube #
			{ X - CubeWidth / 2.0F, Y + CubeHeight / 2.0F, Z - CubeDepth / 2.0F, D3DCOLOR_XRGB(0, 255, 0) }, // - + - 0
			{ X + CubeWidth / 2.0F, Y + CubeHeight / 2.0F, Z - CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // + + - 1
			{ X - CubeWidth / 2.0F, Y - CubeHeight / 2.0F, Z - CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // - - - 2
			{ X + CubeWidth / 2.0F, Y - CubeHeight / 2.0F, Z - CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // + - - 3
			{ X - CubeWidth / 2.0F, Y + CubeHeight / 2.0F, Z + CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // - + + 4
			{ X + CubeWidth / 2.0F, Y + CubeHeight / 2.0F, Z + CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // + + + 5
			{ X - CubeWidth / 2.0F, Y - CubeHeight / 2.0F, Z + CubeDepth / 2.0F, D3DCOLOR_XRGB(255, 180, 0) }, // - - + 6
			{ X + CubeWidth / 2.0F, Y - CubeHeight / 2.0F, Z + CubeDepth / 2.0F, D3DCOLOR_XRGB(0, 255, 0) }, // + - + 7
		};

		SHORT CubeIndeces[] = {
			// Front
			0, 1, 3,
			3, 2, 0,
			// Left
			0, 4, 6,
			6, 2, 0,
			// Back
			4, 5, 7,
			7, 6, 4,
			// Right
			1, 5, 7,
			7, 3, 1,
			// Up
			0, 4, 5,
			5, 1, 0,
			// Down
			3, 7, 6,
			6, 2, 3
		};

		if (state == Fade::out) {
			Color = D3DCOLOR_XRGB((BYTE)(Color) >> 16, (BYTE)(Color) >> 8, (BYTE)(Color) - 0x05);
		} else if (state == Fade::in) {
			Color = D3DCOLOR_XRGB((BYTE)(Color) >> 16, (BYTE)(Color) >> 8, (BYTE)(Color) + 0x05);
		}

		constexpr DWORD CUSTOMFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

		IDirect3DDevice9 *Device = GraphicsDevice->GetDevice();

		IDirect3DVertexBuffer9 *Vertex = nullptr;
		Device->CreateVertexBuffer(ARRAYSIZE(CubeVerticies) * sizeof(POINTVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED, &Vertex, nullptr);

		LPVOID lpVertex = nullptr;
		Vertex->Lock(0, 0, &lpVertex, 0);
		CopyMemory(lpVertex, &CubeVerticies, ARRAYSIZE(CubeVerticies) * sizeof(POINTVERTEX));
		Vertex->Unlock();

		IDirect3DIndexBuffer9 *Index= nullptr;
		Device->CreateIndexBuffer(ARRAYSIZE(CubeIndeces) * sizeof(SHORT), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &Index, nullptr);

		LPVOID lpIndex = nullptr;
		Index->Lock(0, 0, &lpIndex, 0);
		CopyMemory(lpIndex, &CubeIndeces, ARRAYSIZE(CubeIndeces) * sizeof(SHORT));
		Index->Unlock();

		GraphicsDevice->BeginRender(Color);
		GraphicsDevice->SetVirtualCamera({ 0.0F, 20.0F, -20.0F }, { 0.0F, 0.0F, 0.0F }, 60, 1.0F, 200.0F);

		Device->SetFVF(CUSTOMFVF);
		Device->SetRenderState(D3DRS_LIGHTING, FALSE);
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		Device->SetStreamSource(0, Vertex, 0, sizeof(POINTVERTEX));
		Device->SetIndices(Index);
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		GraphicsDevice->EndRender(); */

		EndPaint(hMainWindow, &ps);
		
		return 0;
	
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO minmax = reinterpret_cast<LPMINMAXINFO>(lParam);
		minmax->ptMinTrackSize = { MainWindowSize.cx, MainWindowSize.cy };
		return 0;
	}
	case WM_CLOSE:
		if (MessageBox(hMainWindow, L"You want to exit?", L"-INFORMATION-", MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
			DestroyWindow(hMainWindow);
		return 0;
	case WM_DESTROY:
		delete GraphicsDevice;
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hMainWindow, Msg, wParam, lParam);
	}

}
