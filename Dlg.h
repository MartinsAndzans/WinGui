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

// # DIALOG STYLE #
typedef struct DLG {
	LPCWSTR Title;
	UINT16 Width;
	UINT16 Height;
	UINT16 FontHeight;
	LPCWSTR FontFamily;
}*LPDLG, *PDLG;

class DlgBox {
public:

	DlgBox() : m_hDlgTemplate(nullptr), m_DlgTemplateSize(0) {
		
		m_DlgTitle = L"Dialog";
		m_DlgSize = { 400, 200 };
		m_DlgFontHeight = 20;
		m_DlgFontWeight = 0;
		m_DlgFontItalic = FALSE;
		m_DlgFontFamily = L"Segoe UI";

	}

	void SetDialogTitle(LPCWSTR DlgTitle) {
		m_DlgTitle = DlgTitle;
	}

	void SetDialogSize(LONG Width, LONG Height) {
		m_DlgSize = {
			.cx = Width,
			.cy = Height
		};
	}

	void SetDialogFont() {

	}

	DlgBox(const DlgBox &Other) = default;

private:

	//=========== DIALOG ===========//
	std::wstring m_DlgTitle;
	SIZE m_DlgSize;
	WORD m_DlgFontHeight;
	WORD m_DlgFontWeight;
	BYTE m_DlgFontItalic;
	std::wstring m_DlgFontFamily;
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
