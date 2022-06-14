#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "gui.h"
#include "Functions.h"
#include "resource.h"

#pragma warning(disable:28251)
INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {

	#if DEBUG == 1
	AllocConsole();
	FILE *OutputStream = nullptr, *InputStream = nullptr;
	freopen_s(&OutputStream, "CONOUT$", "w", stdout);
	freopen_s(&InputStream, "CONIN$", "r", stdin);
	SetConsoleTitleA("##Debug Console##");
	#endif

	HANDLE hMutex = CreateMutex(nullptr, true, L"{1SDF2-SDYGT-45YUT-89546}"); // * Prevent From 2 Instances of Application in the same time *
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(HWND_DESKTOP, L"Only One Instance Of That Application Can Exist!", L"-WARNING-", MB_ICONWARNING | MB_OK);
		return EXIT_FAILURE;
	}

	/*****************************************************************************/
	/*                              Initilaztion                                 */
	if (!WinGui::Init(L"##Window##", 600, 600, RGB(145, 145, 255), nCmdShow)) {
		Functions::ShowLastError(HWND_DESKTOP, "Initilization Error!");
		return EXIT_FAILURE;
	}
	/*                                                                           */

	/******************/
	/*    Main Loop   */
	WinGui::Run();
	/*                */

	/****************************/
	/*        Close Mutex       */
	if (hMutex != nullptr)
		CloseHandle(hMutex);
	/****************************/

	return EXIT_SUCCESS;

}
