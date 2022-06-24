#pragma once

#include <Windows.h>
#include <array>

//====================================//
//                                    //
// Copyright (c) 2022 Martins Andzans //
//                                    //
//====================================//

//===========================//
// Dialog Template Structure //
//===========================//
// DWORD	Style            //
// DWORD	ExtendetStyle    //
// WORD		DialogItemCount  //
// SHORT	X                //
// SHORT	Y                //
// SHORT	Width            //
// SHORT	Height           //
// WCHAR[]	Menu             //
// WCHAR[]	Class            //
// WCHAR[]	Title            //
//===========================//

// Menu = 0x0000 # No Menu #
// Class = 0x0000 # Predefined Dialog Class #

//================================//
// Dialog Item Template Structure //
//================================//
// DWORD	Style                 //
// DWORD	ExtendetStyle         //
// SHORT	X                     //
// SHORT	Y                     //
// SHORT	Width                 //
// SHORT	Height                //
// DWORD	ItemID                //
// WCHAR[]	Class                 //
// WCHAR[]	Title                 //
// WORD		Reserved              //
//================================//

// * Dialog Item IDs *
enum Enum : SHORT { ID_OK = 0x000F, ID_CANCEL = 0x0010, ID_YES = 0x0011, ID_NO = 0x0012 };

enum ItemClass : WORD { BUTTON = 0x0080, STATIC = 0x0082 };

// # All Strings Must Be Null Terminated #
struct DIALOG {
	DWORD ExtendedStyle; // * Extended Window Styles *
	LPCWSTR Title; // * Dialog Title \ "NULL" - No Title *
	DWORD Style; // * Window Styles *
	SHORT X; // * X Position *
	SHORT Y; // * Y Position *
	SHORT Width; // * Dialog Width *
	SHORT Height; // * Dialog Height *
	LPCWSTR Menu; // * Dilog Menu \ "NULL" - No Menu *
};

// # All Strings Must Be Null Terminated #
struct ITEM {
	DWORD ExtendedStyle; // * Extended Window Styles *
	LPCWSTR Title; // * Dialog Item Title \ "NULL" - No Title *
	WORD Class; // * Dialog Item Class \ "0x0000" - Error *
	DWORD Style; // * Window Styles *
	SHORT X; // * X Position *
	SHORT Y; // * Y Position *
	SHORT Width; // * Dialog Item Width *
	SHORT Height; // * Dialog Item Height *
	DWORD Id; // * Dialog Item Id *
};

namespace Dlg {

	template<size_t ItemCount>
	/// <returns>Dialog Template Handle</returns>
	HANDLE CreateDialogTemplate(const DIALOG &Dlg, const std::array<ITEM, ItemCount> &DlgItems);

	/// <returns>Dialog Exit Code</returns>
	INT_PTR ShowDialog(HWND hWndParent, HANDLE hDlgTemplate, DLGPROC DlgProc, LPVOID lpParam);
	
	/// <summary># Delete Dialog Template #</summary>
	VOID DeleteDialogTemplate(HANDLE hDlgTemplate);

};
