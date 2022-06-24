#include "WinGui.h"
#include "Console.h"

#pragma warning(disable:28251)
INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {

	#if DEBUG == 1
	/***** ***** Show Debug Console ***** *****/
	AllocConsole();
	FILE *OutputStream = nullptr, *InputStream = nullptr, *ErrorStream = nullptr;
	freopen_s(&OutputStream, "CONOUT$", "w", stdout);
	freopen_s(&InputStream, "CONIN$", "r", stdin);
	freopen_s(&ErrorStream, "CONOUT$", "w", stderr);
	Console::SetTitle("Debug Console");
	/***** ***** **** ****** ****** ***** *****/
	#endif

	if (!WinGui::wCreateWindow(L"Window", 1000, 800, RGB(145, 135, 255), nCmdShow)) {
		return EXIT_FAILURE;
	}
	
	WinGui::wRun();

	return EXIT_SUCCESS;

}
