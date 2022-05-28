#include "WinGui.h"

#pragma region WinGui

#pragma region Variables
HWND WinGui::hMainWindow = NULL; // * Main Window Handle *
RECT WinGui::WindowDimensions = { 0 }; // * Main Window Dimensions *

CONST COLORREF WinGui::BackgroundColor = RGB(0, 145, 255); // * Main Window Background Color *

CONST UINT WinGui::MainWindowWidth = 1000; // * Main Window Width *
CONST UINT WinGui::MainWindowHeight = 800; // * Main Window Height *

POINT WinGui::MousePosition; // * Mouse Position *
#pragma endregion

BOOL _fastcall WinGui::CreateMainWindow(LPCWSTR WindowTitle) noexcept {

	CONST HINSTANCE hInstance = GetModuleHandle(NULL);

	CONST INT SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
	CONST INT SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hbrBackground = CreateSolidBrush(BackgroundColor);
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MAINCURSOR));
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.lpszClassName = L"MAIN WINDOW CLASS";
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_GLOBALCLASS;

	if (!RegisterClassEx(&wcex)) {

		return FALSE;
	}

	hMainWindow = CreateWindowEx(NULL,
		L"MAIN WINDOW CLASS",
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		SCREEN_WIDTH / 2 - MainWindowWidth / 2,
		SCREEN_HEIGHT / 2 - MainWindowHeight / 2,
		MainWindowWidth,
		MainWindowHeight,
		HWND_DESKTOP,
		NULL,
		hInstance,
		nullptr);

	if (hMainWindow == NULL){

		return FALSE;
	}

	ShowWindow(hMainWindow, SW_SHOW);
	UpdateWindow(hMainWindow);

	return TRUE;

}

VOID _fastcall WinGui::BroadcastMessages(VOID) noexcept {
	
	MSG Msg = { 0 };

	while (GetMessage(&Msg, NULL, 0, 0) > 0) { // # Get Message From Message Queue #
		TranslateMessage(&Msg); // # Translate Keyboard Message #
		DispatchMessage(&Msg); // # Call Window Procedure #
	}

}

LRESULT CALLBACK WinGui::WindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept {

	switch (Msg) {
	case WM_CREATE:
	{
		LPCREATESTRUCT lpwindow = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetTimer(hMainWindow, 0xFF00FF00, 1000 / 10, NULL);
		return 0;
	}
	case WM_SIZE:
	{
		WindowDimensions.right = LOWORD(lParam);
		WindowDimensions.bottom = HIWORD(lParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		MousePosition.x = LOWORD(lParam);
		MousePosition.y = HIWORD(lParam);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps = { 0 };
		HDC WindowDC = BeginPaint(hMainWindow, &ps);

		D2D1_RENDER_TARGET_PROPERTIES d2d1rtp = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), 0.0F, 0.0F,
			D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE, D2D1_FEATURE_LEVEL_DEFAULT);

		ID2D1Factory *Factory = nullptr; // * DirectX2D Factory *
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);

		ID2D1DCRenderTarget *RenderTarget = nullptr;
		if (SUCCEEDED(Factory->CreateDCRenderTarget(&d2d1rtp, &RenderTarget))) {

			RenderTarget->BindDC(WindowDC, &WindowDimensions);

			D2D1_RECT_F rect = {
				WindowDimensions.right / 2.0F - 240.0F,
				WindowDimensions.bottom / 2.0F - 240.0F,
				WindowDimensions.right / 2.0F + 240.0F,
				WindowDimensions.bottom / 2.0F + 240.0F
			};

			D2D1_POINT_2F center = {
				WindowDimensions.right / 2.0F,
				WindowDimensions.bottom / 2.0F
			};

			ID2D1SolidColorBrush *Brush = nullptr;
			if (SUCCEEDED(RenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &Brush))) {

				std::vector<COLORREF> ColorCollection = {
					D2D1::ColorF::LightGreen,
					D2D1::ColorF::Wheat,
					D2D1::ColorF::BurlyWood,
					D2D1::ColorF::White
				};

				RenderTarget->BeginDraw();

				D2D1_POINT_2F line_up = { rect.left, rect.top };
				D2D1_POINT_2F line_down = { rect.left, rect.bottom };
				for (size_t I = 0; I <= rect.right - rect.left; I++) {

					Brush->SetColor(D2D1::ColorF(ColorCollection[I % ColorCollection.size()]));

					RenderTarget->DrawLine(center, line_up, Brush);
					RenderTarget->DrawLine(center, line_down, Brush);
					line_up.x += 1.0F;
					line_down.x += 1.0F;

				}
				D2D1_POINT_2F line_left = { rect.left, rect.top };
				D2D1_POINT_2F line_right = { rect.right, rect.top };
				for (size_t I = 0; I <= rect.bottom - rect.top; I++) {

					Brush->SetColor(D2D1::ColorF(ColorCollection[I % ColorCollection.size()]));

					RenderTarget->DrawLine(center, line_left, Brush);
					RenderTarget->DrawLine(center, line_right, Brush);
					line_left.y += 1.0F;
					line_right.y += 1.0F;

				}

				Brush->SetColor(D2D1::ColorF(D2D1::ColorF::ForestGreen));

				CONST FLOAT rows = 10.0F;
				CONST FLOAT collumns = 10.0F;

				D2D1_POINT_2F position = { static_cast<FLOAT>(WindowDimensions.left + 10L), static_cast<FLOAT>(WindowDimensions.top + 10L) };
				D2D1_SIZE_F size = { 600.0F, 600.0F };

				D2D1_POINT_2F V1 = { position.x, position.y };
				D2D1_POINT_2F V2 = { position.x, position.y + size.height };

				FLOAT CollumnWidth = size.width / collumns;

				for (FLOAT I = 1.0F; I <= collumns + 1.0F; I++) {
					RenderTarget->DrawLine(V1, V2, Brush, 2.0F);
					V1.x += CollumnWidth;
					V2.x += CollumnWidth;
				}

				FLOAT CollumnHeight = size.height / rows;

				V1 = { position.x, position.y };
				V2 = { position.x + size.width, position.y };

				for (FLOAT I = 1.0F; I <= rows + 1.0F; I++) {
					RenderTarget->DrawLine(V1, V2, Brush, 2.0F);
					V1.y += CollumnHeight;
					V2.y += CollumnHeight;
				}
				
				RenderTarget->EndDraw();
				Brush->Release();

			}

			RenderTarget->Release();

		}

		Factory->Release();

		SetBkMode(WindowDC, TRANSPARENT);
		SetTextColor(WindowDC, RGB(255, 255, 255));
		SelectObject(WindowDC, GetStockObject(SYSTEM_FONT));

		SIZE TextSizePx = { 0 };
		std::string LabelText = "Hello World!";
		GetTextExtentPoint32A(WindowDC, LabelText.c_str(), static_cast<INT>(LabelText.length()), &TextSizePx);
		TextOutA(WindowDC, WindowDimensions.right / 2 - TextSizePx.cx / 2, WindowDimensions.bottom / 2 - TextSizePx.cy / 2, LabelText.c_str(), static_cast<INT>(LabelText.length()));

		CONST LONG Width = 200L;
		CONST LONG Height = 140L;

		GDI_TRIANGLE triangle = {
			{ WindowDimensions.left + Width / 2, WindowDimensions.top },
			{ WindowDimensions.left, WindowDimensions.top + Height },
			{ WindowDimensions.left + Width, WindowDimensions.top + Height }
		};

		GdiPlus::DrawCircle(WindowDC, { WindowDimensions.right / 2, WindowDimensions.bottom / 2 }, 200U, 26U);
		
		EndPaint(hMainWindow, &ps);
		return 0;
	}
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpminmax = reinterpret_cast<LPMINMAXINFO>(lParam);
		lpminmax->ptMinTrackSize = { MainWindowWidth, MainWindowHeight };
		return 0;
	}
	case WM_CLOSE:
	{
		if (MessageBoxA(hMainWindow, "Are You Sure!", "-INFORMATION-", MB_ICONINFORMATION | MB_YESNO | MB_DEFBUTTON2) == IDYES) {
			KillTimer(hMainWindow, 0xFF00FF00);
			DestroyWindow(hMainWindow);
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProc(hMainWindow, Msg, wParam, lParam);

}

#pragma endregion
