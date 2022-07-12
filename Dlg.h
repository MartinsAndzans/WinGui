#pragma once

#include <Windows.h>
#include <string>

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

// # Dialog Template #
struct DialogTemplate {
	LPCWSTR Title;
	WORD Width;
	WORD Height;
	WORD FontHeight;
	WORD FontWeight;
	LPCWSTR FontFamily;
};

class Dialog {
public:

	Dialog() : m_hDlgTemplate(nullptr), m_DlgTemplateSize(0) {
		
		m_DialogTitle = L"Dialog";
		m_DiaogSize = { 400, 200 };
		m_FontHeight = 20;
		m_FontWeight = 0;
		m_FontItalic = FALSE;
		m_FontFamily = L"Segoe UI";

	}

	Dialog(const Dialog &other) {



	}

private:

	//=========== DIALOG ===========//
	std::wstring m_DialogTitle;
	SIZE m_DiaogSize;
	WORD m_FontHeight;
	WORD m_FontWeight;
	BYTE m_FontItalic;
	std::wstring m_FontFamily;
	//==============================//
	
	LPVOID m_hDlgTemplate; // * Handle To Dialog Template *
	SIZE_T m_DlgTemplateSize; // * Dialog Template Size In Bytes *

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
