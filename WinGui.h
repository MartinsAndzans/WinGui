#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <string>
#include "GdiPlus.h"

typedef void(*DISPLAY)(HDC, INT32, INT32); // $ Display Callback Signature $

class WinGui {
public:

	// # Create Main Window #
	static bool wCreateWindow(_In_ LPCWSTR WindowTitle, _In_ INT32 Width, _In_ INT32 Height, _In_ COLORREF BackgroundColor, _In_ INT nCmdShow) noexcept;
	
	// # Register Display Callback #
	static void wRegisterDisplayCallback(DISPLAY DisplayCallback);

	// # Main Loop #
	static void wRun(void) noexcept;

private:

	// # Main Window Procedure #
	static LRESULT CALLBACK WindowProc(HWND hWindow, UINT Msg, WPARAM wParam, LPARAM lParam);

private:

	//===== Main Window Properties =====//
	static HWND hMainWindow; // * Handle to Main Window *
	static SIZE MainWindowSize; // * Main Window Size *
	static COLORREF BackgroundColor; // * Main Window Background Color *
	//==================================//

	static HWND hEditBox_Code;
	static HWND hListBox_Controls;

};
