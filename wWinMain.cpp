#include "WinGui.h"
#include "Console.h"

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow) {

	#if defined(_DEBUG)
	/***** ***** Show Debug Console ***** *****/
	AllocConsole();
	FILE *OutputStream = nullptr, *InputStream = nullptr, *ErrorStream = nullptr;
	freopen_s(&OutputStream, "CONOUT$", "w", stdout);
	freopen_s(&InputStream, "CONIN$", "r", stdin);
	freopen_s(&ErrorStream, "CONOUT$", "w", stderr);
	Console::Title("Debug Console");
	/***** ***** **** ****** ****** ***** *****/
	#endif

	if (!WinGui::wCreateMainWindow(L"Window -SandBox-", 800, 600, nCmdShow)) {
		MessageBoxA(HWND_DESKTOP, Algoritms::GetWINAPIErrorMessage().c_str(), "ERROR", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	INT nExitCode = WinGui::wMainLoop();

	return nExitCode;

}
