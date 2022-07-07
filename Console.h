#pragma once

/************************************
*                                   *
* Copyright(c) 2022 Martins Andzans *
*                                   *
************************************/

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>

std::ostream& operator<<(std::ostream &out, char8_t Char) {
	return out << static_cast<char>(Char);
}

struct Console {

	enum Colors : byte {

		fBlack = 0x00,
		fBlue = 0x01,
		fGreen = 0x02,
		fAqua = 0x03,
		fRed = 0x04,
		fPurple = 0x05,
		fYellow = 0x06,
		fWhite = 0x07,
		fGray = 0x08,
		fLightBlue = 0x09,
		fLightGreen = 0x0A,
		fLightAqua = 0x0B,
		fLightRed = 0x0C,
		fLightPurple = 0x0D,
		fLightYellow = 0x0E,
		fBrightWhite = 0x0F,

		bBlack = 0x00,
		bBlue = 0x10,
		bGreen = 0x20,
		bAqua = 0x30,
		bRed = 0x40,
		bPurple = 0x50,
		bYellow = 0x60,
		bWhite = 0x70,
		bGray = 0x80,
		bLightBlue = 0x90,
		bLightGreen = 0xA0,
		bLightAqua = 0xB0,
		bLightRed = 0xC0,
		bLightPurple = 0xD0,
		bLightYellow = 0xE0,
		bBrightWhite = 0xF0

	};

	#undef SetConsoleTitle
	// # Set Console Title Text #
	static bool _stdcall SetConsoleTitle(const std::string &title) noexcept {

		bool Error = SetConsoleTitleA(title.c_str());
		return Error;
	
	}

	// # Set Console Text Color Format: [0xBF] B - Background F - Foreground #
	static bool _stdcall SetConsoleTextColor(byte color) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		bool Error = SetConsoleTextAttribute(hOutput, color);
		return Error;
	
	}

	// Print Message to Console
	static void _stdcall Print(const std::string &message, char8_t end = '\n') noexcept {

		std::cout << message << end;

	}

	static std::string _stdcall Input(const std::string &message = "> ") noexcept {
		
		fflush(stdin);
		std::string Buffer;
		std::cout << message;
		std::getline(std::cin, Buffer);

		return Buffer;

	}

	static char8_t _stdcall Pause(const std::string &message = "Press any key to continue . . .") noexcept {

		fflush(stdin);
		std::cout << message;
		char8_t key = _getch();
		std::cout << "\n";

		return key;

	}

	static void _stdcall DrawRectangleInConsole(USHORT Width = 40, USHORT Height = 20, char8_t Symbol = '#') noexcept {

		for (USHORT Y = 1; Y <= Height; Y++) {
			for (USHORT X = 1; X <= Width; X++) {
				if (Y == 1 || Y == Height) {
					std::cout << Symbol;
				} else {
					if (X == 1 || X == Width) {
						std::cout << Symbol;
					} else {
						std::cout << ' ';
					}
				}
			}
			std::cout << "\n";
		}

	}

	static bool _stdcall GetConsoleCursorPosition(_Out_ COORD &ConsoleCursorPosition) noexcept {
		
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		
		CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		if (!GetConsoleScreenBufferInfo(hOutput, &csbi)) {
			ConsoleCursorPosition = { 0, 0 };
			return false;
		}
		
		ConsoleCursorPosition = { csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y };
		
		return true;
	
	}

	static bool _stdcall SetConsoleCursorPosition(SHORT X, SHORT Y) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		return ::SetConsoleCursorPosition(hOutput, { X, Y });

	}

	static bool _stdcall ClearConsole(void) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		if (!GetConsoleScreenBufferInfo(hOutput, &csbi))
			return false;

		std::cout.flush();

		if (!::SetConsoleCursorPosition(hOutput, { 0, 0 }))
			return false;

		DWORD Length = csbi.dwSize.X * csbi.dwSize.Y, Written = 0;
		if (!FillConsoleOutputCharacter(hOutput, TEXT(' '), Length, { 0, 0 }, &Written))
			return false;

		return true;

	}

	static bool _stdcall SetConsoleCursorVisible(bool CusrorVisible = true) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO cursor_info = { 0 };
		if (!GetConsoleCursorInfo(hOutput, &cursor_info))
			return false;

		cursor_info.bVisible = CusrorVisible;
		if (!SetConsoleCursorInfo(hOutput, &cursor_info))
			return false;

		return true;

	}

	static bool _stdcall SetConsoleCursorSize(uint32_t CursorSize = 20U) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO cursor_info = { 0 };
		if (!GetConsoleCursorInfo(hOutput, &cursor_info))
			return false;

		if (CursorSize <= 100U) {
			cursor_info.dwSize = CursorSize;
			if (!SetConsoleCursorInfo(hOutput, &cursor_info))
				return false;
		} else {
			cursor_info.dwSize = 20U;
			if (!SetConsoleCursorInfo(hOutput, &cursor_info))
				return false;
		}

		return true;

	}

	static bool _stdcall GetConsoleSize(_Out_ SIZE &ConsoleSize) noexcept {
		
		RECT ConsoleRect = { 0 };
		if (!GetClientRect(GetConsoleWindow(), &ConsoleRect)) {
			ConsoleSize = { 0, 0 };
			return false;
		}
		
		ConsoleSize = { ConsoleRect.right, ConsoleRect.bottom };
		
		return true;
	
	}

	static bool _stdcall SetConsoleSize(INT32 Width, INT32 Height) noexcept {
		return SetWindowPos(GetConsoleWindow(), nullptr, 0, 0, Width, Height, SWP_SHOWWINDOW | SWP_NOMOVE);
	}

};
