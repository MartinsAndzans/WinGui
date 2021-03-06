#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <string>

#include "GdiPlus.h"
#include "Direct2D1.h"
#include "Algoritms.h"

#include "resource.h"

class WinGui {
public:

	// # Create Main Window #
	static bool wCreateMainWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ INT nCmdShow) noexcept;

	// # Main Loop #
	static INT wMainLoop(void) noexcept;

private:

	// # Main Window Procedure #
	static LRESULT CALLBACK WindowProc(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam);

private:

	//===== Main Window Properties =====//
	static HWND hMainWindow; // * Handle to Main Window *
	static SIZE MainWindowSize; // * Main Window Size *
	//==================================//

	static std::unique_ptr<Direct2D1> GraphicsDevice;

};
