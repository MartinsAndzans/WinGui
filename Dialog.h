#pragma once

#define WIN32_LEAN_AND_MEAN

//===== HEADERS ======//
#include <memory>
#include <Windows.h>
#include <vector>
#include <string>
//====================//

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
struct DialogStyle {
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
struct DialogItemStyle {
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

class Dialog {
public:

	Dialog()
		: m_Dlg({
			
		})
	{ /*...*/ }

	Dialog(const Dialog &other) {



	}

private:

	DialogStyle m_Dlg;
	std::vector<DialogItemStyle> m_DlgItems;

};
