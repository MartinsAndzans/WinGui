#include "WinGui.h"
#include "Console.h"

#pragma warning(disable:28251)
INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {

	#if defined(_DEBUG)
	/***** ***** Show Debug Console ***** *****/
	AllocConsole();
	FILE *OutputStream = nullptr, *InputStream = nullptr, *ErrorStream = nullptr;
	freopen_s(&OutputStream, "CONOUT$", "w", stdout);
	freopen_s(&InputStream, "CONIN$", "r", stdin);
	freopen_s(&ErrorStream, "CONOUT$", "w", stderr);
	Console::SetConsoleTitle("Debug Console");
	/***** ***** **** ****** ****** ***** *****/
	#endif

	if (!WinGui::wCreateMainWindow(L"Window -SandBox-", 1440, 900, nCmdShow)) {
		return EXIT_FAILURE;
	}
	
	INT nExitCode = WinGui::wMainLoop();

	return nExitCode;

}
