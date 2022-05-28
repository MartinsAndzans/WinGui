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

class functions {

	/// <summary>
	/// Converts Integer To Char Array
	/// <para>1234 % 10 = 4 | 1234 / 10 = 123 || 123 % 10 = 3 | 123 / 10 = 12 || 12 % 10 = 2 | 12 / 10 = 1 || 1 % 10 = 1 | 1 / 10 = 0</para>
	/// </summary>
	/// <param name="Value">Integer Type [int64_t] Value To Be Converted</param>
	/// <param name="Buffer">Char Buffer</param>
	/// <param name="BufferSize">Buffer Size</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL _itoa(int64_t Value, LPSTR Buffer, SIZE_T BufferSize) noexcept {

		BOOL Minus = FALSE;
		CONST SHORT ASCI_VALUE_ZERO = 48; // * Char Value 48 *

		std::string StringValue{};

		if (Value == INT64_MIN) {
			OutputDebugStringA("\'[Functions::_itoa] - Value \"OVERFLOW\"\'\r\n");
			return FALSE;
		}

		if (Value < 0) {
			Minus = TRUE;
			Value *= -1; // -Value *= -1
		}

		do {

			CHAR Character = static_cast<CHAR>(Value % 10 + ASCI_VALUE_ZERO); // * Get Last Number - Char Value 48 - 57 *
			Value = Value / 10; // Remove Last Number
			StringValue.insert(0, 1, Character); // Add Character To Begining of StringValue

		} while (Value != 0);
		
		if (Minus) {
			StringValue.insert(0, "-");
		}

		if (BufferSize >= StringValue.length() + 1) {
			strcpy_s(Buffer, BufferSize, StringValue.c_str());
		} else {
			OutputDebugStringA("\'[Functions::_itoa] - Buffer To Small\'\r\n");
			return FALSE;
		}

		return TRUE;

	}

	/// <summary>
	/// This Function Encrypt Text To ASCII Value Code
	/// </summary>
	/// <param name="TextToBeEncrypted">Text To Be Encrypted</param>
	/// <returns>Encrypted Text</returns>
	static std::string EncryptText(const std::string &TextToBeEncrypted) noexcept {

		std::string EncryptedText{};

		for (CHAR CHARACTER : TextToBeEncrypted) {
			USHORT ASCII_VALUE = static_cast<USHORT>(CHARACTER);
			EncryptedText += std::to_string(ASCII_VALUE) + ":";
		}

		return EncryptedText;

	}

	/// <summary>
	/// This Function Encrypt Unicode Text To ASCII Value Code
	/// </summary>
	/// <param name="UTextToBeEncrypted">Unicode Text To Be Encrypted</param>
	/// <returns>Encrypted Unicode Text</returns>
	static std::wstring EncryptText(const std::wstring &UTextToBeEncrypted) noexcept {

		std::wstring EncryptedUText{};

		for (WCHAR UCHARACTER : UTextToBeEncrypted) {
			USHORT ASCII_VALUE = static_cast<USHORT>(UCHARACTER);
			EncryptedUText += std::to_wstring(ASCII_VALUE) + L":";
		}

		return EncryptedUText;

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
				CHAR CHARACTER = static_cast<CHAR>(std::stoi(EncryptedText));
				DecryptedText += CHARACTER;
				EncryptedText.replace(0, EncryptedText.find(':') + 1, "");
			} else {
				return "Cannot Decrypt Text Because Text Is Corrupted!";
			}
		}

		return DecryptedText;

	}

	/// <summary>
	/// This Function Decrypt Unicode Text From ASCII Value Code
	/// </summary>
	/// <param name="UEncryptedText">Unicode Text To Be Decrypted</param>
	/// <returns>Decrypted Unicode Text</returns>
	static std::wstring DecryptText(std::wstring UEncryptedText) noexcept {

		std::wstring UDecryptedText{};

		while (UEncryptedText.length() != 0) {
			if (UEncryptedText.ends_with(L':')) {
				WCHAR UCHARACTER = static_cast<WCHAR>(std::stoi(UEncryptedText));
				UDecryptedText += UCHARACTER;
				UEncryptedText.replace(0, UEncryptedText.find(L':') + 1, L"");
			} else {
				return L"Cannot Decrypt Unicode Text Because Unicode Text Is Corrupted!";
			}
		}

		return UDecryptedText;

	}

	/// <summary>
	/// This Function Finds Character Position Into String
	/// </summary>
	/// <param name="Text">Text</param>
	/// <param name="Char">Character To Find</param>
	/// <param name="TextLength">Text Length In Characters</param>
	/// <returns>
	/// If Character has been found Function Returns Character Position,
	/// <para>but If Character not found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static SIZE_T FindChar(LPCSTR Text, CONST CHAR Char, SIZE_T TextLength) noexcept {
		
		for (SIZE_T I = 0; I < TextLength; I++) {
			if (Text[I] == Char) {
				return I;
			}
		}

		return static_cast<SIZE_T>(-1);

	}

	/// <summary>
	/// This Function Finds Unicode Character Position Into Unicode String
	/// </summary>
	/// <param name="UText">Unicode Text</param>
	/// <param name="UChar">Unicode Character To Find</param>
	/// <param name="UTextLength">Unicode Text Length In Characters</param>
	/// <returns>
	/// If Unicode Character has been found Function Returns Unicode Character Position,
	/// <para>but If Character not found Function Returns MAXSIZE_T Value</para>
	/// </returns>
	static SIZE_T FindChar(LPCWSTR UText, CONST WCHAR UChar, SIZE_T UTextLength) noexcept {

		for (SIZE_T I = 0; I < UTextLength; I++) {
			if (UText[I] == UChar) {
				return I;
			}
		}

		return static_cast<SIZE_T>(-1);

	}

	/// <summary>
	/// This Function Rounds Double String
	/// </summary>
	/// <param name="DoubleString">Double String</param>
	/// <returns>Rounded Double String</returns>
	static std::string RoundDoubleString(std::string DoubleString) noexcept {

		if (DoubleString.find('.') != std::string::npos) {

			while (DoubleString.ends_with('0') or DoubleString.ends_with('.')) {

				if (DoubleString.ends_with('.')) {
					DoubleString.pop_back();
					return DoubleString;
				}

				DoubleString.pop_back();

			}

			return DoubleString;

		}

		return DoubleString;

	}

	/// <summary>
	/// This Function Copy Text To Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">New Clipboard Owner</param>
	/// <param name="Text">Text To Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND NewClipboardOwner, const std::string &Text) noexcept {

		if (Text.length() != 0) {

			if (OpenClipboard(NewClipboardOwner)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LPTR | LMEM_VALID_FLAGS, sizeof(CHAR) * (Text.length() + 1));
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				void *CopyDataPtr = LocalLock(CopyData);
				if (CopyDataPtr == nullptr) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(CopyDataPtr, static_cast<const void*>(Text.c_str()), sizeof(CHAR) * (Text.length() + 1));
				SetClipboardData(CF_TEXT, CopyData);
				LocalUnlock(CopyData);
				LocalFree(CopyData);
				CloseClipboard();
				return TRUE;

			}

		}

		return FALSE;

	}

	/// <summary>
	/// This Function Copy Unicode Text To Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">New Clipboard Owner</param>
	/// <param name="UText">Unicode Text To Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL CopyTextToClipboard(HWND NewClipboardOwner, const std::wstring &UText) noexcept {

		if (UText.length() != 0) {
			if (OpenClipboard(NewClipboardOwner)) {
				EmptyClipboard();
				HLOCAL CopyData = LocalAlloc(LPTR | LMEM_VALID_FLAGS, sizeof(WCHAR) * (UText.length() + 1));
				if (CopyData == NULL) {
					CloseClipboard();
					return FALSE;
				}
				void *CopyDataPtr = LocalLock(CopyData);
				if (CopyDataPtr == nullptr) {
					LocalFree(CopyData);
					CloseClipboard();
					return FALSE;
				}
				memcpy(CopyDataPtr, static_cast<const void*>(UText.c_str()), sizeof(WCHAR) * (UText.length() + 1));
				SetClipboardData(CF_UNICODETEXT, CopyData);
				LocalUnlock(CopyData);
				LocalFree(CopyData);
				CloseClipboard();
				return TRUE;

			}

		}

		return FALSE;

	}

	/// <summary>
	/// This Function Gets Text From Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">New Clipboard Owner</param>
	/// <param name="Buffer">Text From Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL GetTextFromClipboard(HWND NewClipboardOwner, std::string &Buffer) noexcept {

		if (OpenClipboard(NewClipboardOwner)) {
			HLOCAL ClipboardData = GetClipboardData(CF_TEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return FALSE;
			}
			void *ClipboardDataPtr = LocalLock(ClipboardData);
			if (ClipboardDataPtr == nullptr) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return FALSE;
			}
			Buffer += static_cast<char*>(ClipboardDataPtr);
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return TRUE;
		}

		return FALSE;

	}

	/// <summary>
	/// This Function Gets Unicode Text From Clipboard
	/// </summary>
	/// <param name="NewClipboardOwner">New Clipboard Owner</param>
	/// <param name="UBuffer">Unicode Text From Clipboard</param>
	/// <returns>If Function Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL GetTextFromClipboard(HWND NewClipboardOwner, std::wstring &UBuffer) noexcept {

		if (OpenClipboard(NewClipboardOwner)) {
			HLOCAL ClipboardData = GetClipboardData(CF_UNICODETEXT);
			if (ClipboardData == NULL) {
				CloseClipboard();
				return FALSE;
			}
			void *ClipboardDataPtr = LocalLock(ClipboardData);
			if (ClipboardDataPtr == nullptr) {
				LocalFree(ClipboardData);
				CloseClipboard();
				return FALSE;
			}
			UBuffer += static_cast<wchar_t*>(ClipboardDataPtr);
			LocalUnlock(ClipboardData);
			LocalFree(ClipboardData);
			CloseClipboard();
			return TRUE;
		}

		return FALSE;

	}

	/// <summary>
	/// Shows Last Error Message
	/// </summary>
	/// <param name="ParentWindow">Parent Window</param>
	/// <param name="AdditionalErroMessage">Additional Error Message</param>
	static VOID ShowLastError(HWND ParentWindow = HWND_DESKTOP, std::string AdditionalErrorMessage = " - Additional Error Message") noexcept {
		std::string ErrorMessage = "ERROR " + std::to_string(GetLastError()) + AdditionalErrorMessage;
		MessageBoxA(ParentWindow, ErrorMessage.c_str(), "-ERROR-", MB_OK | MB_ICONERROR);
	}

	/// <summary>
	/// Saves Bitmap To File
	/// </summary>
	/// <param name="Bitmap">Bitmap</param>
	/// <param name="FilePath">File Path With ".bmp" Extension</param>
	/// <param name="BitmapSize">Bitmap Size In Pixels</param>
	/// <returns>If Succeeded Returns TRUE, but If not Returns FALSE</returns>
	static BOOL SaveBitmapToFile(HBITMAP Bitmap, LPCSTR FilePath, CONST SIZE &BitmapSize) noexcept {

		std::ofstream image;

		CONST WORD BM = 0x4D42; // ASCII 'B' = 66 / 0x42 | 'M' = 77 / 0x4D
		CONST DWORD BitmapSizeCXxCY = BitmapSize.cx * BitmapSize.cy; // Bitmap Size [cx * cy]

		BITMAPFILEHEADER bmfheader = { 0 };
		bmfheader.bfType = BM;
		bmfheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORREF) * BitmapSizeCXxCY; // File Size
		bmfheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // Offset To Color Bits
		bmfheader.bfReserved1 = NULL;
		bmfheader.bfReserved2 = NULL;

		BITMAPINFOHEADER bmiheader = { 0 };
		bmiheader.biSize = sizeof(BITMAPINFOHEADER);
		bmiheader.biWidth = BitmapSize.cx; // Bitmap Width In Pixels
		bmiheader.biHeight = BitmapSize.cy; // Bitmap Height In Pixels
		bmiheader.biPlanes = 0x01; // 1 - Bitmap has a one plane
		bmiheader.biBitCount = 0x20; // 32 - Bitmap has a maximum of 2^32 colors
		bmiheader.biCompression = BI_RGB;
		bmiheader.biSizeImage = BitmapSizeCXxCY;
		bmiheader.biXPelsPerMeter = BitmapSize.cx;
		bmiheader.biYPelsPerMeter = BitmapSize.cy;
		bmiheader.biClrUsed = NULL;
		bmiheader.biClrImportant = NULL;

		std::unique_ptr<COLORREF[]> BitmapBytesPtr = std::make_unique<COLORREF[]>(BitmapSizeCXxCY);

		if (!BitmapBytesPtr) {
			return FALSE;
		}

		BITMAPINFO bminfo = { 0 };
		bminfo.bmiHeader = bmiheader;

		HDC ScreenDC = GetDC(HWND_DESKTOP);
		GetDIBits(ScreenDC, Bitmap, 0, BitmapSize.cy, BitmapBytesPtr.get(), &bminfo, DIB_RGB_COLORS);
		ReleaseDC(HWND_DESKTOP, ScreenDC);

		image.open(FilePath, std::ios::binary); // Open File

		if (!image.is_open()) {
			return FALSE;
		}

		image.write(reinterpret_cast<const char*>(&bmfheader), sizeof(BITMAPFILEHEADER)); // BITMAP FILE HEADER
		image.write(reinterpret_cast<const char*>(&bmiheader), sizeof(BITMAPINFOHEADER)); // BITMAP INFO HEADER
		image.write(reinterpret_cast<const char*>(BitmapBytesPtr.get()), sizeof(COLORREF) * BitmapSizeCXxCY); // COLOR BYTE ARRAY

		image.close(); // Close File

		return TRUE;

	}

};

class Sound {
private:

	MCIDEVICEID device_id = 0U;

public:

	typedef DWORD MCISTATUSCODE; // * MCISTATUSCODE [DWORD] is a 64-bit unsigned integer *
	typedef DWORD MCISTATUS; // * MCISTATUS [DWORD] is a 64-bit unsigned integer *

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

	/// <param name="FilePath">Music FilePath -/ *.wav; *.wma; *.mp3 /-</param>
	MCIERROR Open(LPCWSTR FilePath) noexcept {

		MCI_OPEN_PARMS open = { 0 };
		open.lpstrElementName = FilePath;
		open.wDeviceID = 0U;

		MCIERROR open_error = mciSendCommandW(0U, MCI_OPEN, MCI_WAIT | MCI_OPEN_ELEMENT, reinterpret_cast<DWORD_PTR>(&open));
		device_id = open.wDeviceID;
		
		return open_error;

	}

	/// <summary>
	/// This Function Obtains Playback Status
	/// <para>MCI_STATUS_POSITION - Obtains Current Playback Position</para>
	/// <para>MCI_STATUS_LENGTH - Obtains Total Media Length</para>
	/// <para>MCI_STATUS_MODE - Obtains Current Mode of The Device</para>
	/// <para>MCI_STATUS_TIME_FORMAT - Obtains Current Time Format of The Device</para>
	/// </summary>
	MCIERROR GetPlaybackStatus(MCISTATUSCODE StatusCode, MCISTATUS *PlaybackStatus) noexcept {

		MCI_STATUS_PARMS status = { 0 };
		status.dwItem = StatusCode; // Status Code
		status.dwReturn = NULL; // Return Value

		MCIERROR Error = mciSendCommandW(device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, reinterpret_cast<DWORD_PTR>(&status));
		if (PlaybackStatus != nullptr)
			*PlaybackStatus = static_cast<MCISTATUS>(status.dwReturn);
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
			return mciSendCommandW(device_id, MCI_PLAY, MCI_NOTIFY | MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
		} else {
			return mciSendCommandW(device_id, MCI_PLAY, MCI_FROM, reinterpret_cast<DWORD_PTR>(&play));
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
			return mciSendCommandW(device_id, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_START, NULL);
		} else if (SeekTo == MCI_SEEK_TO_END) {
			return mciSendCommandW(device_id, MCI_SEEK, MCI_WAIT | MCI_SEEK_TO_END, NULL);
		} else {
			return mciSendCommandW(device_id, MCI_SEEK, MCI_WAIT | MCI_TO, reinterpret_cast<DWORD_PTR>(&seek));
		}
		
	}

	/// <summary>
	/// This Function Pauses Playbeck
	/// </summary>
	MCIERROR Pause(VOID) noexcept {
		return mciSendCommandW(device_id, MCI_PAUSE, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Resumes Playbeck
	/// </summary>
	MCIERROR Resume(VOID) noexcept {
		return mciSendCommandW(device_id, MCI_RESUME, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Stops Playbeck
	/// </summary>
	MCIERROR Stop(VOID) noexcept {
		return mciSendCommandW(device_id, MCI_STOP, MCI_WAIT, NULL);
	}

	/// <summary>
	/// This Function Closes MCI Device
	/// </summary>
	MCIERROR Close(VOID) noexcept {
		return mciSendCommandW(device_id, MCI_CLOSE, MCI_WAIT, NULL);
	}

};
