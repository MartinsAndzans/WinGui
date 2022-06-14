#pragma once

/************************************************
*                                               *
*         Copyright(c) Martins Andzans          *
*                                               *
************************************************/

#include <ciso646>
#include <Windows.h>
#include <string>
#include <fstream>

class Functions {
public:


	// # This function reverse number #
	static uint64_t ReverseNumber(uint64_t Number) noexcept {

		uint64_t ReversedNumber = 0;

		/* Reverse Number */
		while (Number != 0) {
			ReversedNumber *= 10; // # Add zero to reversed number #
			ReversedNumber += Number % 10; // # Get last digit from number and to reversed number #
			Number /= 10; // # Remove last digit from number #
		}

		return ReversedNumber;

	}

	// # This function converts all uppercase letters to lowercase letters #
	static void ToLower(LPSTR Text, size_t TextLength) noexcept {

		for (size_t i = 0; i < TextLength; i++){
			if (Text[i] >= 65 && Text[i] <= 90) {
				Text[i] += 32;
			}
		}

	}

	// # This function converts all lowercase letters to upercase letters #
	static void ToUpper(LPSTR Text, size_t TextLength) noexcept {

		for (size_t i = 0; i < TextLength; i++) {
			if (Text[i] >= 97 && Text[i] <= 122) {
				Text[i] -= 32;
			}
		}

	}

	// # This function encrypt text to ASCII value code #
	static std::string EncryptText(const std::string &TextToBeEncrypted) noexcept {

		std::string EncryptedText{};

		for (char8_t Character : TextToBeEncrypted) {
			USHORT ASCII_VALUE = static_cast<USHORT>(Character);
			EncryptedText += std::to_string(ASCII_VALUE) + ":";
		}

		return EncryptedText;

	}

	/// <summary>
	/// This Function Decrypt Text From ASCII Value Code
	/// </summary>
	/// <param name="EncryptedText">Text To Be Decrypted</param>
	/// <returns>Decrypted Text</returns>
	static std::string DecryptText(std::string EncryptedText) noexcept {

		std::string DecryptedText{};

		while (EncryptedText.length() != 0) {
			if (EncryptedText.ends_with(':')) {
				char8_t Character = static_cast<char8_t>(std::stoi(EncryptedText));
				DecryptedText += Character;
				EncryptedText.replace(0, EncryptedText.find(':') + 1, "");
			} else {
				return "Cannot decrypt text because text is corrupted!";
			}
		}

		return DecryptedText;

	}

	// # This function find character position in char array #
	// ## If character has been found function returns character position ##
	// ### If character has not been found function returns MAXSIZE_T value ###
	static size_t FindChar(LPCSTR Text, const char8_t Char, size_t TextLength) noexcept {
		
		for (size_t i = 0; i < TextLength; i++) {
			if (Text[i] == Char) {
				return i;
			}
		}

		return MAXSIZE_T;

	}

	// # This function find character position in wchar_t array #
	// ## If character has been found function returns character position ##
	// ### If character has not been found function returns MAXSIZE_T value ###
	static size_t FindChar(LPCWSTR UText, const wchar_t UChar, size_t UTextLength) noexcept {

		for (size_t i = 0; i < UTextLength; i++) {
			if (UText[i] == UChar) {
				return i;
			}
		}
		
		return MAXSIZE_T;

	}

	static std::string RoundDoubleString(std::string DoubleString) noexcept {

		if (DoubleString.find('.') == std::string::npos) {
			return DoubleString;
		}

		while (DoubleString.ends_with('0') or DoubleString.ends_with('.')) {

			if (DoubleString.ends_with('.')) {
				DoubleString.pop_back();
				return DoubleString;
			}

			DoubleString.pop_back();

		}

		return DoubleString;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool CopyTextToClipboard(_In_ HWND NewClipboardOwner, _In_ const std::string &Text) noexcept {

		if (Text.empty()) {
			return false;
		}

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		EmptyClipboard();
		HLOCAL CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(CHAR) * (Text.length() + 1));

		if (CopyData == NULL) {
			CloseClipboard();
			return false;
		}

		void *lpCopyData = LocalLock(CopyData);

		if (lpCopyData == nullptr) {
			LocalFree(CopyData);
			CloseClipboard();
			return false;
		}

		memcpy(lpCopyData, static_cast<const void*>(Text.c_str()), sizeof(CHAR) * (Text.length() + 1));
		SetClipboardData(CF_TEXT, CopyData);
		LocalUnlock(CopyData);
		LocalFree(CopyData);
		CloseClipboard();
		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool CopyTextToClipboard(_In_ HWND NewClipboardOwner, _In_ const std::wstring &UText) noexcept {

		if (UText.empty()) {
			return false;
		}

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		EmptyClipboard();
		HLOCAL CopyData = LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT | LMEM_VALID_FLAGS, sizeof(WCHAR) * (UText.length() + 1));

		if (CopyData == NULL) {
			CloseClipboard();
			return false;
		}

		void *lpCopyData = LocalLock(CopyData);

		if (lpCopyData == nullptr) {
			LocalFree(CopyData);
			CloseClipboard();
			return false;
		}

		memcpy(lpCopyData, static_cast<const void*>(UText.c_str()), sizeof(WCHAR) * (UText.length() + 1));
		SetClipboardData(CF_UNICODETEXT, CopyData);
		LocalUnlock(CopyData);
		LocalFree(CopyData);
		CloseClipboard();
		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool GetTextFromClipboard(_In_ HWND NewClipboardOwner, _Inout_ std::string &Buffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HLOCAL ClipboardData = GetClipboardData(CF_TEXT);

		if (ClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		char *lpClipboardData = reinterpret_cast<char*>(LocalLock(ClipboardData));

		if (lpClipboardData == nullptr) {
			LocalFree(ClipboardData);
			CloseClipboard();
			return false;
		}

		Buffer += lpClipboardData;
		LocalUnlock(ClipboardData);
		LocalFree(ClipboardData);
		CloseClipboard();
		return true;

	}

	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool GetTextFromClipboard(_In_ HWND NewClipboardOwner, _Inout_ std::wstring &UBuffer) noexcept {

		if (!OpenClipboard(NewClipboardOwner)) {
			return false;
		}

		HLOCAL ClipboardData = GetClipboardData(CF_UNICODETEXT);

		if (ClipboardData == NULL) {
			CloseClipboard();
			return false;
		}

		wchar_t *lpClipboardData = reinterpret_cast<wchar_t*>(LocalLock(ClipboardData));

		if (lpClipboardData == nullptr) {
			LocalFree(ClipboardData);
			CloseClipboard();
			return false;
		}

		UBuffer += lpClipboardData;
		LocalUnlock(ClipboardData);
		LocalFree(ClipboardData);
		CloseClipboard();
		return true;

	}

	static void OutputMultiplicationTable(_In_ std::ostream &out) noexcept {

		for (int32_t FirstNumber = 1; FirstNumber <= 10; FirstNumber++) {
			for (int32_t SecondNumber = 1; SecondNumber <= 10; SecondNumber++) {
				out << "|\t" << FirstNumber << "\t*\t" << SecondNumber << "\t=\t" << FirstNumber * SecondNumber << "\t|\n";
			}
			out << "\n";
		}

	}

	// # This function show last error in -/MessageBox/- #
	static void ShowLastError(HWND OwnerWindow = HWND_DESKTOP, const std::string &AdditionalErrorMessage = "Additional Error Message") noexcept {
		std::string ErrorMessage = "-/ERROR/- " + std::to_string(GetLastError()) + " - " + AdditionalErrorMessage;
		MessageBoxA(OwnerWindow, ErrorMessage.c_str(), "-ERROR-", MB_OK | MB_ICONERROR);
	}

	// # This function show last error in -/MessageBox/- #
	static void ShowLastError(HWND OwnerWindow = HWND_DESKTOP, const std::wstring &AdditionalErrorMessage = L"Additional Error Message") noexcept {
		std::wstring ErrorMessage = L"-/ERROR/- " + std::to_wstring(GetLastError()) + L" - " + AdditionalErrorMessage;
		MessageBoxW(OwnerWindow, ErrorMessage.c_str(), L"-ERROR-", MB_OK | MB_ICONERROR);
	}

	/// <param name="FilePath">File path with ".bmp" extension</param>
	/// <param name="BitmapSize">Bitmap size in pixels</param>
	/// <returns>If succeeded returns true, but if not returns false</returns>
	static bool SaveBitmapToFile(_In_ HBITMAP Bitmap, _In_ LPCSTR FilePath, _In_ SIZE BitmapSize) noexcept {

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
		GetDIBits(ScreenDC, Bitmap, 0, BitmapSize.cy, lpBitmapBytes.get(), &bminfo, DIB_RGB_COLORS);
		ReleaseDC(HWND_DESKTOP, ScreenDC);

		image.open(FilePath, std::ios::binary);

		if (!image.is_open()) {
			return false;
		}

		image.write(reinterpret_cast<const char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // # BITMAP FILE HEADER #
		image.write(reinterpret_cast<const char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // # BITMAP INFO HEADER #
		image.write(reinterpret_cast<const char*>(lpBitmapBytes.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // # BITMAP BYTE ARRAY #

		image.close();

		return true;

	}

};

class MP3Player {
private:
	MCIDEVICEID _DeviceId = 0U;
	BOOL Repeat = FALSE;
public:

	// # MCI Error Codes #
	enum ErrorCodes {
		MCI_OK = 0, // * MCI Success Code *
		MCIERROR_BASE = 256,
		MCIERROR_INVALID_DEVICE_ID = 257,
		MCIERROR_UNRECOGNIZED_KEYWORD = 259,
		MCIERROR_UNRECOGNIZED_COMMAND = 261,
		MCIERROR_HARDWARE = 262,
		MCIERROR_INVALID_DEVICE_NAME = 263,
		MCIERROR_OUT_OF_MEMORY = 264,
		MCIERROR_DEVICE_OPEN = 265,
		MCIERROR_CANNOT_LOAD_DRIVER = 266,
		MCIERROR_MISSING_COMMAND_STRING = 267,
		MCIERROR_PARAM_OVERFLOW = 268,
		MCIERROR_MISSING_STRING_ARGUMENT = 269,
		MCIERROR_BAD_INTEGER = 270,
		MCIERROR_PARSER_INTERNAL = 271,
		MCIERROR_DRIVER_INTERNAL = 272,
		MCIERROR_MISSING_PARAMETER = 273,
		MCIERROR_UNSUPPORTED_FUNCTION = 274,
		MCIERROR_FILE_NOT_FOUND = 275,
		MCIERROR_DEVICE_NOT_READY = 276,
		MCIERROR_INTERNAL = 277,
		MCIERROR_DRIVER = 278,
		MCIERROR_CANNOT_USE_ALL = 279,
		MCIERROR_MULTIPLE = 280,
		MCIERROR_EXTENSION_NOT_FOUND = 281,
		MCIERROR_OUTOFRANGE = 282,
		MCIERROR_FLAGS_NOT_COMPATIBLE = 283,
		MCIERROR_FILE_NOT_SAVED = 286,
		MCIERROR_DEVICE_TYPE_REQUIRED = 287,
		MCIERROR_DEVICE_LOCKED = 288,
		MCIERROR_DUPLICATE_ALIAS = 289,
		MCIERROR_BAD_CONSTANT = 290,
		MCIERROR_MUST_USE_SHAREABLE = 291,
		MCIERROR_MISSING_DEVICE_NAME = 292,
		MCIERROR_BAD_TIME_FORMAT = 293,
		MCIERROR_NO_CLOSING_QUOTE = 294,
		MCIERROR_DUPLICATE_FLAGS = 295,
		MCIERROR_INVALID_FILE = 296,
		MCIERROR_NULL_PARAMETER_BLOCK = 297,
		MCIERROR_UNNAMED_RESOURCE = 298,
		MCIERROR_NEW_REQUIRES_ALIAS = 299,
		MCIERROR_NOTIFY_ON_AUTO_OPEN = 300,
		MCIERROR_NO_ELEMENT_ALLOWED = 301,
		MCIERROR_NONAPPLICABLE_FUNCTION = 302,
		MCIERROR_ILLEGAL_FOR_AUTO_OPEN = 303,
		MCIERROR_FILENAME_REQUIRED = 304,
		MCIERROR_EXTRA_CHARACTERS = 305,
		MCIERROR_DEVICE_NOT_INSTALLED = 306,
		MCIERROR_GET_CD = 307,
		MCIERROR_SET_CD = 308,
		MCIERROR_SET_DRIVE = 309,
		MCIERROR_DEVICE_LENGTH = 310,
		MCIERROR_DEVICE_ORD_LENGTH = 311,
		MCIERROR_NO_INTEGER = 312,
		MCIERROR_WAVE_OUTPUTSINUSE = 320,
		MCIERROR_WAVE_SETOUTPUTINUSE = 321,
		MCIERROR_WAVE_INPUTSINUSE = 322,
		MCIERROR_WAVE_SETINPUTINUSE = 323,
		MCIERROR_WAVE_OUTPUTUNSPECIFIED = 324,
		MCIERROR_WAVE_INPUTUNSPECIFIED = 325,
		MCIERROR_WAVE_OUTPUTSUNSUITABLE = 326,
		MCIERROR_WAVE_SETOUTPUTUNSUITABLE = 327,
		MCIERROR_WAVE_INPUTSUNSUITABLE = 328,
		MCIERROR_WAVE_SETINPUTUNSUITABLE = 329,
		MCIERROR_SEQ_DIV_INCOMPATIBLE = 336,
		MCIERROR_SEQ_PORT_INUSE = 337,
		MCIERROR_SEQ_PORT_NONEXISTENT = 338,
		MCIERROR_SEQ_PORT_MAPNODEVICE = 339,
		MCIERROR_SEQ_PORT_MISCERROR = 340,
		MCIERROR_SEQ_TIMER = 341,
		MCIERROR_SEQ_PORTUNSPECIFIED = 342,
		MCIERROR_SEQ_NOMIDIPRESENT = 343,
		MCIERROR_NO_WINDOW = 346,
		MCIERROR_CREATEWINDOW = 347,
		MCIERROR_FILE_READ = 348,
		MCIERROR_FILE_WRITE = 349,
		MCIERROR_CUSTOM_DRIVER_BASE = 512
	};

	MP3Player() {

		MCI_OPEN_PARMS Open = { 0 };
		Open.lpstrDeviceType = MAKEINTRESOURCE(MCI_DEVTYPE_CD_AUDIO);
		
		MCIERROR Error = mciSendCommand(NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID, reinterpret_cast<DWORD_PTR>(&Open));



	}

	bool IsOpen() {



	}

	/// <param name="FilePath">Music FilePath -/ *.wav; *.wma; *.mp3 /-</param>
	MCIERROR Load(LPCWSTR FilePath) noexcept {

		MCI_LOAD_PARMS Load = { 0 };
		Load.lpfilename = FilePath;

		return mciSendCommand(_DeviceId, MCI_LOAD, MCI_WAIT | MCI_LOAD_FILE, reinterpret_cast<DWORD_PTR>(&Load));

	}

	/// <summary>
	/// This Function Obtains Playback Status
	/// <para>MCI_STATUS_POSITION - Obtains Current Playback Position</para>
	/// <para>MCI_STATUS_LENGTH - Obtains Total Media Length</para>
	/// <para>MCI_STATUS_MODE - Obtains Current Mode of The Device</para>
	/// <para>MCI_STATUS_TIME_FORMAT - Obtains Current Time Format of The Device</para>
	/// </summary>
	MCIERROR GetPlaybackStatus(DWORD StatusCode, DWORD *PlaybackStatus) noexcept {

		MCI_STATUS_PARMS status = { 0 };
		status.dwItem = StatusCode; // Status Code
		status.dwReturn = NULL; // Return Value

		MCIERROR Error = mciSendCommand(_DeviceId, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&status));
		if (PlaybackStatus != nullptr)
			*PlaybackStatus = static_cast<DWORD>(status.dwReturn);
		return Error;

	}

	/// <summary>
	/// This Function Plays Music From The Beginning
	/// <para>If NOTIFY is TRUE MCI Sends To Callback Window [MM_MCINOTIFY] Message 'LOWORD(lParam) - MCIDeviceID'</para>
	/// </summary>
	MCIERROR Play(HWND CallbackWindow, BOOL NOTIFY = FALSE) noexcept {

		MCI_PLAY_PARMS play = { 0 };
		play.dwCallback = reinterpret_cast<DWORD_PTR>(CallbackWindow);
		play.dwFrom = 0;
		
		if (NOTIFY) {
			return mciSendCommandW(_DeviceId, MCI_PLAY, MCI_NOTIFY | MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
		} else {
			return mciSendCommandW(_DeviceId, MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
		}

	}

	/// <summary>
	/// This Function Seek To Specified Point of Playback
	/// <para>MCI_SEEK_TO_START - Seek To Beginning of Playback</para>
	/// <para>MCI_SEEK_TO_END - Seek To End of Playback</para>
	/// </summary>
	MCIERROR Seek(DWORD SeekTo) noexcept {

		MCI_SEEK_PARMS seek = { 0 };
		seek.dwTo = SeekTo;

		if (SeekTo == MCI_SEEK_TO_START) {
			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_START, NULL);
		} else if (SeekTo == MCI_SEEK_TO_END) {
			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_END, NULL);
		} else {
			return mciSendCommandW(_DeviceId, MCI_SEEK, MCI_WAIT | MCI_TO, reinterpret_cast<DWORD_PTR>(&seek));
		}
		
	}

	/// <summary>
	/// This Function Pauses Playbeck
	/// </summary>
	MCIERROR Pause(VOID) noexcept {
		return mciSendCommandW(_DeviceId, MCI_PAUSE, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Resumes Playbeck
	/// </summary>
	MCIERROR Resume(VOID) noexcept {
		return mciSendCommandW(_DeviceId, MCI_RESUME, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Stops Playbeck
	/// </summary>
	MCIERROR Stop(VOID) noexcept {
		return mciSendCommandW(_DeviceId, MCI_STOP, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Closes MCI Device
	/// </summary>
	MCIERROR Close(VOID) noexcept {
		return mciSendCommandW(_DeviceId, MCI_CLOSE, MCI_WAIT, NULL);
	}

};
