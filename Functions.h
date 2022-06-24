#pragma once

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>

class Functions {
public:

	// # This Function Reverse Number #
	static uint64_t ReverseNumber(_In_ uint64_t Number) noexcept {

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
	static uint64_t SumDigits(_In_ uint64_t Number) noexcept {

		uint64_t Sum = 0;

		/* Sum Digits */
		while (Number != 0) {
			Sum += Number % 10; // # Get last digit from "Number" and add to "Sum" #
			Number /= 10; // # Remove last digit from "Number" #
		}

		return Sum;

	}

	// # If Number is Even This Function returns TRUE #
	// ## If Number is Odd This Function returns FALSE ##
	static constexpr bool Even(_In_ uint64_t Number) noexcept {
		return Number % 2 == 0;
	}

	// # If Number is Odd This Function returns TRUE #
	// ## If Number is Even This Function returns FALSE ##
	static constexpr bool Odd(_In_ uint64_t Number) noexcept {
		return Number % 2 != 0;
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
	static std::string EncryptText(_In_ const std::string &Text) noexcept {

		std::string EncryptedText;

		for (const char8_t &Character : Text) {
			int32_t ASCII_VALUE = static_cast<int32_t>(Character);
			EncryptedText += std::to_string(ASCII_VALUE) + ":";
		}

		EncryptedText.pop_back();

		return EncryptedText;

	}

	// # This Function decrypt text from ASCII value code #
	static std::string DecryptText(_In_ const std::string &EncryptedText) noexcept {

		std::string DecryptedText;
		std::istringstream sstream(EncryptedText);

		while (!sstream.eof()) {
			std::string temp;
			std::getline(sstream, temp, ':');
			int32_t ASCII_VALUE = std::stoi(temp);
			DecryptedText += static_cast<char8_t>(ASCII_VALUE);
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

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool CopyTextToClipboard(_In_ HWND NewClipboardOwner, _In_ const std::string &Text) noexcept {

		if (Text.empty()) {
			return false;
		}

		HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, sizeof(char8_t) * (Text.length() + 1));
		if (hCopyData == NULL) {
			return false;
		}

		void *lpCopyData = GlobalLock(hCopyData);
		if (lpCopyData == nullptr) {
			GlobalFree(hCopyData);
			return false;
		}

		memcpy(lpCopyData, Text.c_str(), sizeof(char8_t) * (Text.length() + 1));

		GlobalUnlock(hCopyData);

		if (!OpenClipboard(NewClipboardOwner)) {
			GlobalFree(hCopyData);
			return false;
		}

		EmptyClipboard();
		SetClipboardData(CF_TEXT, hCopyData);
		CloseClipboard();

		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool CopyTextToClipboard(_In_ HWND NewClipboardOwner, _In_ const std::wstring &UText) noexcept {

		if (UText.empty()) {
			return false;
		}

		HGLOBAL hCopyData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_VALID_FLAGS, sizeof(wchar_t) * (UText.length() + 1));
		if (hCopyData == NULL) {
			return false;
		}

		void *lpCopyData = GlobalLock(hCopyData);
		if (lpCopyData == nullptr) {
			GlobalFree(hCopyData);
			return false;
		}

		memcpy(lpCopyData, UText.c_str(), sizeof(wchar_t) * (UText.length() + 1));

		GlobalUnlock(hCopyData);

		if (!OpenClipboard(NewClipboardOwner)) {
			GlobalFree(hCopyData);
			return false;
		}

		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hCopyData);
		CloseClipboard();

		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool GetTextFromClipboard(_In_ HWND NewClipboardOwner, _Inout_ std::string &Buffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_TEXT);
		if (hClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		void *lpClipboardData = GlobalLock(hClipboardData);
		if (lpClipboardData == nullptr) {
			CloseClipboard();
			return false;
		}

		Buffer += reinterpret_cast<char*>(lpClipboardData);
		GlobalUnlock(hClipboardData);
		CloseClipboard();

		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool GetTextFromClipboard(_In_ HWND NewClipboardOwner, _Inout_ std::wstring &UBuffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HGLOBAL hClipboardData = GetClipboardData(CF_UNICODETEXT);
		if (hClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		void *lpClipboardData = GlobalLock(hClipboardData);
		if (lpClipboardData == nullptr) {
			CloseClipboard();
			return false;
		}

		UBuffer += reinterpret_cast<wchar_t*>(lpClipboardData);
		GlobalUnlock(hClipboardData);
		CloseClipboard();

		return true;

	}

	static void OutputMultiplicationTable(_Inout_ std::ostream &out) noexcept {

		for (int32_t FirstNumber = 1; FirstNumber <= 10; FirstNumber++) {
			for (int32_t SecondNumber = 1; SecondNumber <= 10; SecondNumber++) {
				out << "|\t" << FirstNumber << "\t*\t" << SecondNumber << "\t=\t" << FirstNumber * SecondNumber << "\t|\n";
			}
			out << "\n";
		}

	}

	// # This Function show last error in -/MessageBox/- #
	static void ShowLastError(HWND hWndParent, const std::string &AdditionalErrorMessage) noexcept {
		std::string ErrorMessage = "[ERROR]: " + std::to_string(GetLastError()) + " - " + AdditionalErrorMessage;
		MessageBoxA(hWndParent, ErrorMessage.c_str(), "-ERROR-", MB_OK | MB_ICONERROR);
	}

	// # This Function show last error in -/MessageBox/- #
	static void ShowLastError(HWND hWndParent, const std::wstring &AdditionalErrorMessage) noexcept {
		std::wstring ErrorMessage = L"[ERROR]: " + std::to_wstring(GetLastError()) + L" - " + AdditionalErrorMessage;
		MessageBoxW(hWndParent, ErrorMessage.c_str(), L"-ERROR-", MB_OK | MB_ICONERROR);
	}

	/// <param name="FilePath">File path with ".bmp" extension</param>
	/// <param name="BitmapSize">Bitmap size in pixels</param>
	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool SaveBitmapToFile(_In_ HBITMAP hBitmap, _In_ LPCSTR FilePath, _In_ SIZE BitmapSize) noexcept {

		std::ofstream image;

		constexpr WORD BM = 0x4D42; // * ASCII 'B' = 0x42 / 'M' = 0x4D *
		const DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // * Bitmap Size [cx x cy] *

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM;
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORREF) * BitmapSizeCXxCY; // # File size #
		bmfheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // # Offset to color bits #
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

		if (!lpBitmapBytes) {
			return false;
		}

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		HDC ScreenDC = GetDC(HWND_DESKTOP);
		GetDIBits(ScreenDC, hBitmap, 0, BitmapSize.cy, lpBitmapBytes.get(), &bminfo, DIB_RGB_COLORS);
		ReleaseDC(HWND_DESKTOP, ScreenDC);

		image.open(FilePath, std::ios::binary);

		if (!image.is_open()) {
			return false;
		}

		image.write(reinterpret_cast<char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // # BITMAP FILE HEADER #
		image.write(reinterpret_cast<char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // # BITMAP INFO HEADER #
		image.write(reinterpret_cast<char*>(lpBitmapBytes.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // # BITMAP BYTE ARRAY #
		
		image.close();

		return true;

	}

};

//class MP3Player {
//private:
//	MCIDEVICEID _DeviceId = 0U;
//	BOOL Repeat = FALSE;
//public:
//
//	// # MCI Error Codes #
//	enum ErrorCodes {
//		MCI_OK = 0, // * MCI Success Code *
//	};
//
//	/// <param name="FilePath">Music FilePath -/ *.wav; *.wma; *.mp3 /-</param>
//	MCIERROR Load(LPCWSTR FilePath) noexcept {
//
//		MCI_LOAD_PARMS Load = { 0 };
//		Load.lpfilename = FilePath;
//
//		return mciSendCommand(_DeviceId, MCI_LOAD, MCI_WAIT | MCI_LOAD_FILE, reinterpret_cast<DWORD_PTR>(&Load));
//
//	}
//
//	/// <summary>
//	/// This Function Obtains Playback Status
//	/// <para>MCI_STATUS_POSITION - Obtains Current Playback Position</para>
//	/// <para>MCI_STATUS_LENGTH - Obtains Total Media Length</para>
//	/// <para>MCI_STATUS_MODE - Obtains Current Mode of The Device</para>
//	/// <para>MCI_STATUS_TIME_FORMAT - Obtains Current Time Format of The Device</para>
//	/// </summary>
//	MCIERROR GetPlaybackStatus(DWORD StatusCode, DWORD *PlaybackStatus) noexcept {
//
//		MCI_STATUS_PARMS status = { 0 };
//		status.dwItem = StatusCode; // Status Code
//		status.dwReturn = NULL; // Return Value
//
//		MCIERROR Error = mciSendCommand(_DeviceId, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&status));
//		if (PlaybackStatus != nullptr)
//			*PlaybackStatus = static_cast<DWORD>(status.dwReturn);
//		return Error;
//
//	}
//
//	/// <summary>
//	/// This Function Plays Music From The Beginning
//	/// <para>If NOTIFY is TRUE MCI Sends To Callback Window [MM_MCINOTIFY] Message 'LOWORD(lParam) - MCIDeviceID'</para>
//	/// </summary>
//	MCIERROR Play(HWND CallbackWindow, BOOL NOTIFY = FALSE) noexcept {
//
//		MCI_PLAY_PARMS play = { 0 };
//		play.dwCallback = reinterpret_cast<DWORD_PTR>(CallbackWindow);
//		play.dwFrom = 0;
//		
//		if (NOTIFY) {
//			return mciSendCommandW(_DeviceId, MCI_PLAY, MCI_NOTIFY | MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
//		} else {
//			return mciSendCommandW(_DeviceId, MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
//		}
//
//	}
//
//	/// <summary>
//	/// This Function Seek To Specified Point of Playback
//	/// <para>MCI_SEEK_TO_START - Seek To Beginning of Playback</para>
//	/// <para>MCI_SEEK_TO_END - Seek To End of Playback</para>
//	/// </summary>
//	MCIERROR Seek(DWORD SeekTo) noexcept {
//
//		MCI_SEEK_PARMS seek = { 0 };
//		seek.dwTo = SeekTo;
//
//		if (SeekTo == MCI_SEEK_TO_START) {
//			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_START, NULL);
//		} else if (SeekTo == MCI_SEEK_TO_END) {
//			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_END, NULL);
//		} else {
//			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_TO, reinterpret_cast<DWORD_PTR>(&seek));
//		}
//		
//	}
//
//	/// <summary>
//	/// This Function Pauses Playbeck
//	/// </summary>
//	MCIERROR Pause(VOID) noexcept {
//		return mciSendCommandW(_DeviceId, MCI_PAUSE, MCI_WAIT, NULL);
//	}
//
//	/// <summary>
//	/// This Function Resumes Playbeck
//	/// </summary>
//	MCIERROR Resume(VOID) noexcept {
//		return mciSendCommandW(_DeviceId, MCI_RESUME, MCI_WAIT, NULL);
//	}
//
//	/// <summary>
//	/// This Function Stops Playbeck
//	/// </summary>
//	MCIERROR Stop(VOID) noexcept {
//		return mciSendCommandW(_DeviceId, MCI_STOP, MCI_WAIT, NULL);
//	}
//
//	/// <summary>
//	/// This Function Closes MCI Device
//	/// </summary>
//	MCIERROR Close(VOID) noexcept {
//		return mciSendCommandW(_DeviceId, MCI_CLOSE, MCI_WAIT, NULL);
//	}
//
//};
