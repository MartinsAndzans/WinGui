#ifndef _WIN_GUI_
#define _WIN_GUI_

#include <ciso646>
#include <d2d1.h>
#include <Windows.h>
#include <string>
#include <vector>

#include "resource.h"

#include "Functions.h"
#include "Gdi.h"
#include "Graphics.h"

class WinGui {
private:

	static HWND hMainWindow; // * Main Window Handle *
	static RECT WindowDimensions; // * Main Window Dimensions *

	static CONST COLORREF BackgroundColor; // * Main Window Background Color *

	static CONST UINT MainWindowWidth; // * Main Window Width *
	static CONST UINT MainWindowHeight; // * Main Window Height *

	static POINT MousePosition; // * Mouse Position *

public:

	// # Create Main Window #
	static BOOL _fastcall CreateMainWindow(LPCWSTR WindowTitle) noexcept;

	// # Main Message Loop #
	static VOID _fastcall BroadcastMessages(VOID) noexcept;

private:

	// # Main Window Procedure #
	static LRESULT CALLBACK WindowProcedure(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept;

};

#endif // _WIN_GUI_

struct listbox {

private:

	HWND _hListBox = NULL;
	uint32_t _ItemCount = 0;

protected:

	const uint32_t _MAX_ITEM_COUNT_ = 32767U;

	listbox(HWND parent_window, USHORT id) noexcept {
		_hListBox = CreateWindowExA(WS_EX_CLIENTEDGE, "listbox", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT,
			0, 0, 0, 0,
			parent_window,
			reinterpret_cast<HMENU>(id),
			GetModuleHandle(NULL),
			nullptr);
	}

	BOOL AddItem(const std::wstring string, uint32_t value = 0U) noexcept {
		if (_hListBox != NULL and _ItemCount < _MAX_ITEM_COUNT_) {
			LRESULT index = SendMessageW(_hListBox, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(string.c_str())); // Add Item String To ListBox
			SendMessageW(_hListBox, LB_SETITEMDATA, static_cast<WPARAM>(index), static_cast<LPARAM>(value)); // Add Value To Added ListBox Item
			_ItemCount++;
			return TRUE;
		}
		return FALSE;
	}

	BOOL DeleteItem(uint32_t index) noexcept {
		if (_hListBox != NULL and index < _ItemCount) {
			SendMessageW(_hListBox, LB_DELETESTRING, static_cast<WPARAM>(index), NULL); // Remove Item From ListBox
			_ItemCount--;
			return TRUE;
		}
		return FALSE;
	}

	LRESULT GetSelection(VOID) noexcept {
		if (_hListBox != NULL)
			return SendMessageW(_hListBox, LB_GETCURSEL, NULL, NULL);
	}

	BOOL SelectItem(uint32_t index) noexcept {
		if (_hListBox != NULL) {
			SendMessageW(_hListBox, LB_SETCURSEL, static_cast<WPARAM>(index), NULL);
			return TRUE;
		}
		return FALSE;
	}

#define ListBox_GetItemData(hwnd, index) SendMessage(hwnd, LB_GETITEMDATA, static_cast<WPARAM>(index), NULL) // Return Value is Value Associated With that Item
#define ListBox_FindString(hwnd, string) SendMessage(hwnd, LB_FINDSTRING, static_cast<WPARAM>(-1), static_cast<LPARAM>(string)) // Return Value is String Index
#define ListBox_SetTopItem(hwnd, index) SendMessage(hwnd, LB_SETTOPINDEX, static_cast<WPARAM>(index), NULL)

};
