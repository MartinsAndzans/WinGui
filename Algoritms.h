#pragma once

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <memory>
#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>

struct Algoritms {

	// # This Function Reverse Number #
	static constexpr uint64_t ReverseNumber(_In_ uint64_t Number) noexcept {

		uint64_t ReversedNumber = 0;

		/* Reverse Number */
		while (Number != 0) {
			ReversedNumber *= 10; // # Add zero to "ReversedNumber" #
			ReversedNumber += Number % 10; // # Get last digit from "Number" and add to "ReversedNumber" #
			Number /= 10; // # Remove last digit from "Number" #
		}

		return ReversedNumber;

	}

	// # This Function Sum all "Number" digits #
	static constexpr uint64_t SumDigits(_In_ uint64_t Number) noexcept {

		uint64_t Sum = 0;

		/* Sum Digits */
		while (Number != 0) {
			Sum += Number % 10; // # Get last digit from "Number" and add to "Sum" #
			Number /= 10; // # Remove last digit from "Number" #
		}

		return Sum;

	}

	// # This Function converts all uppercase letters to lowercase letters #
	static void ToLower(_Inout_ LPSTR Text, _In_ size_t TextLength) noexcept {

		/* 65 - Start of Upper Case Letters
		   90 - End of Upper Case Letters
		   32 - Difference Between Upper and Lower Case Letters */

		for (size_t i = 0; i < TextLength; i++){
			if (Text[i] >= 65 && Text[i] <= 90) {
				Text[i] += 32;
			}
		}

	}

	// # This Function converts all lowercase letters to upercase letters #
	static void ToUpper(_Inout_ LPSTR Text, _In_ size_t TextLength) noexcept {

		/* 97 - Start of Lower Case Letters
		   122 - End of Lower Case Letters
		   32 - Difference Between Lower and Upper Case Letters */

		for (size_t i = 0; i < TextLength; i++) {
			if (Text[i] >= 97 && Text[i] <= 122) {
				Text[i] -= 32;
			}
		}

	}

	// # This Function encrypt text to ASCII value code #
	static std::string EncryptText(_In_ std::string_view Text) noexcept {

		std::string EncryptedText;

		for (const char &Character : Text) {
			int32_t ascii_value = static_cast<int32_t>(Character);
			EncryptedText += std::to_string(ascii_value) + ':';
		}

		EncryptedText.pop_back();

		return EncryptedText;

	}

	// # This Function decrypt text from ASCII value code #
	static std::string DecryptText(_In_ std::string_view EncryptedText) noexcept {

		std::string DecryptedText;
		std::istringstream sstream(EncryptedText.data());

		while (!sstream.eof()) {
			std::string temp;
			std::getline(sstream, temp, ':');
			int32_t ascii_value = std::stoi(temp);
			DecryptedText += static_cast<char>(ascii_value);
		}
		
		return DecryptedText;

	}

	// # This Function find character position in char array #
	// ## If character has been found Function returns character position ##
	// ### If character has not been found Function returns MAXSIZE_T value ###
	static size_t FindChar(_In_ LPCSTR Text, _In_ const char8_t Char, _In_ size_t TextLength) noexcept {
		
		for (size_t i = 0; i < TextLength; i++) {
			if (Text[i] == Char) {
				return i;
			}
		}

		return MAXSIZE_T;

	}

	// # This Function find unicode character position in wchar_t array #
	// ## If unicode character has been found Function returns unicode character position ##
	// ### If unicode character has not been found Function returns MAXSIZE_T value ###
	static size_t FindChar(_In_ LPCWSTR UText, _In_ const wchar_t UChar, _In_ size_t UTextLength) noexcept {

		for (size_t i = 0; i < UTextLength; i++) {
			if (UText[i] == UChar) {
				return i;
			}
		}
		
		return MAXSIZE_T;

	}

	// This Function Round Double String : Example "20.22440000" - "20.2244"
	static bool RoundDoubleString(_Inout_ std::string &DoubleString) noexcept {

		if (DoubleString.find('.') == std::string::npos)
			return false;

		while (DoubleString.ends_with('0') || DoubleString.ends_with('.')) {

			if (DoubleString.ends_with('.')) {
				DoubleString.pop_back();
				return true;
			}

			DoubleString.pop_back();

		}

		return true;

	}

	/// <returns>If Succeeded Function Returns TRUE, but if Failed Returns FALSE</returns>
	static bool SetClipboardText(_In_ HWND NewClipboardOwner, _In_ std::string_view Text) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		if (Text.empty()) {

			constexpr char EmptyString[] = "<Empty String>";
			
			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, ARRAYSIZE(EmptyString) * sizeof(CHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return false;
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, EmptyString, ARRAYSIZE(EmptyString) * sizeof(CHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			SetClipboardData(CF_TEXT, hCopyData);
			CloseClipboard();

			return true;

		} else {

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, (Text.length() + 1) * sizeof(CHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return false;
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, Text.data(), (Text.length() + 1) * sizeof(CHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			SetClipboardData(CF_TEXT, hCopyData);
			CloseClipboard();

			return true;
		}

	}

	/// <returns>If Succeeded Function Returns TRUE, but if Failed Returns FALSE</returns>
	static bool SetClipboardText(_In_ HWND NewClipboardOwner, _In_ std::wstring_view UText) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		if (UText.empty()) {

			constexpr wchar_t EmptyUnicodeString[] = L"<Empty Unicode String>";

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, ARRAYSIZE(EmptyUnicodeString) * sizeof(WCHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return false;
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, EmptyUnicodeString, ARRAYSIZE(EmptyUnicodeString) * sizeof(WCHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hCopyData);
			CloseClipboard();

			return true;

		} else {

			HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, (UText.length() + 1) * sizeof(WCHAR));
			if (hCopyData == NULL) {
				CloseClipboard();
				return false;
			}

			LPVOID lpCopyData = nullptr;
			while (lpCopyData == nullptr)
				lpCopyData = GlobalLock(hCopyData);
			memcpy(lpCopyData, UText.data(), (UText.length() + 1) * sizeof(WCHAR));
			GlobalUnlock(hCopyData);

			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hCopyData);
			CloseClipboard();

			return true;
		}

	}

	/// <returns>If Succeeded Function Returns TRUE, but if Failed Returns FALSE</returns>
	static bool GetClipboardText(_In_ HWND NewClipboardOwner, _Inout_ std::string &Buffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);
		if (hClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		LPVOID lpClipboardData = nullptr;
		while (lpClipboardData == nullptr)
			lpClipboardData = GlobalLock(hClipboardData);
		Buffer += reinterpret_cast<LPCSTR>(lpClipboardData);
		GlobalUnlock(hClipboardData);

		CloseClipboard();

		return true;

	}

	/// <returns>If Succeeded Function Returns TRUE, but if Failed Returns FALSE</returns>
	static bool GetClipboardText(_In_ HWND NewClipboardOwner, _Inout_ std::wstring &UBuffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (hClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		LPVOID lpClipboardData = nullptr;
		while (lpClipboardData == nullptr)
			lpClipboardData = GlobalLock(hClipboardData);
		UBuffer += reinterpret_cast<LPCWSTR>(lpClipboardData);
		GlobalUnlock(hClipboardData);

		CloseClipboard();

		return true;

	}

	// # This Function Show Last Error in -/MessageBox/- #
	static void ShowLastError(_In_opt_ HWND hWndParent) noexcept {

		DWORD LastError = GetLastError(); // # Last Error #

		//========== ERROR MESSAGE BUFFER ==========//
		constexpr size_t MAX_CHAR_STRING = 256U;
		CHAR LastErrorMessage[MAX_CHAR_STRING] = { ' ', '-', ' ' };
		//==========================================//

		//========== GET LAST ERROR MESSAGE ==========//
		DWORD Length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr, LastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			LastErrorMessage + 3, MAX_CHAR_STRING - 4, nullptr);
		//============================================//

		std::string MessageBoxTitle = "ERROR " + std::to_string(LastError);
		std::string ErrorMessage = "Error Code: " + std::to_string(LastError);

		if (Length == 0) {
			ErrorMessage += " - Unknown Error";
		} else {
			ErrorMessage += LastErrorMessage;
		}

		MessageBoxA(hWndParent, ErrorMessage.c_str(), MessageBoxTitle.c_str(), MB_OK | MB_ICONERROR);

	}

	/// <param name="FilePath">File path with ".bmp" extension</param>
	/// <param name="BitmapSize">Bitmap size in pixels</param>
	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool SaveBitmapToFile(_In_ HBITMAP hBitmap, _In_ LPCSTR FilePath, _In_ SIZE BitmapSize, _Inout_ std::string *ErrorString = nullptr) noexcept {

		std::ofstream image;

		constexpr WORD BM = 0x4D42; // * ASCII 'B' = 0x42 / 'M' = 0x4D *
		const DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // * Bitmap Size [cx x cy] *

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM;
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORREF) * BitmapSizeCXxCY; // # File Size #
		bmfheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // # Offset To Color Bits #
		bmfheader.bfReserved1 = 0x0000;
		bmfheader.bfReserved2 = 0x0000;

		BITMAPINFOHEADER bmiheader = { 0 };
		bmiheader.biSize = sizeof(BITMAPINFOHEADER);
		bmiheader.biWidth = BitmapSize.cx; // # Bitmap width in pixels #
		bmiheader.biHeight = BitmapSize.cy; // # Bitmap height in pixels #
		bmiheader.biPlanes = 0x01; // # Bitmap has a one plane #
		bmiheader.biBitCount = 0x20; // # Bitmap has a maximum of 2^32 colors #
		bmiheader.biCompression = BI_RGB;
		bmiheader.biSizeImage = BitmapSizeCXxCY;
		bmiheader.biXPelsPerMeter = BitmapSize.cx;
		bmiheader.biYPelsPerMeter = BitmapSize.cy;
		bmiheader.biClrUsed = 0x00000000;
		bmiheader.biClrImportant = 0x00000000;

		std::unique_ptr<COLORREF[]> lpBitmapBytes = std::make_unique<COLORREF[]>(BitmapSizeCXxCY);
		if (!lpBitmapBytes){
			if (ErrorString != nullptr)
				*ErrorString = "Out of Memory";
			return false;
		}

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		HDC hScreenDC = GetDC(HWND_DESKTOP);
		HGDIOBJ hPrevBitmap = SelectObject(hScreenDC, hBitmap);
		GetDIBits(hScreenDC, hBitmap, 0, BitmapSize.cy, lpBitmapBytes.get(), &bminfo, DIB_RGB_COLORS);
		SelectObject(hScreenDC, hPrevBitmap);
		ReleaseDC(HWND_DESKTOP, hScreenDC);

		image.open(FilePath, std::ios::binary);

		if (!image.is_open()) {
			if (ErrorString != nullptr)
				*ErrorString = "Cannot Create File";
			return false;
		}

		image.write(reinterpret_cast<char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // # BITMAP FILE HEADER #
		image.write(reinterpret_cast<char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // # BITMAP INFO HEADER #
		image.write(reinterpret_cast<char*>(lpBitmapBytes.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // # BITMAP BYTE ARRAY #
		
		image.close();

		if (ErrorString != nullptr) {
			*ErrorString = "Operation Completed Successfully";
		}

		return true;

	}

};
