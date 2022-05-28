#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "WinGui.h"

#pragma warning(disable:28251) // Warning C28251 Inconsistent annotation for 'wWinMain': this instance has no annotations

int32_t WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nShowCmd) {

	if (WinGui::CreateMainWindow(L"Window -Template-")) {
		WinGui::BroadcastMessages();
	}

	return 0;

}
