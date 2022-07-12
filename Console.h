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

	enum ColorCodes : byte {

		F_Black = 0x00,
		F_Blue = 0x01,
		F_Green = 0x02,
		F_Aqua = 0x03,
		F_Red = 0x04,
		F_Purple = 0x05,
		F_Yellow = 0x06,
		F_White = 0x07,
		F_Gray = 0x08,
		F_LightBlue = 0x09,
		F_LightGreen = 0x0A,
		F_LightAqua = 0x0B,
		F_LightRed = 0x0C,
		F_LightPurple = 0x0D,
		F_LightYellow = 0x0E,
		F_BrightWhite = 0x0F,

		B_Black = 0x00,
		B_Blue = 0x10,
		B_Green = 0x20,
		B_Aqua = 0x30,
		B_Red = 0x40,
		B_Purple = 0x50,
		B_Yellow = 0x60,
		B_White = 0x70,
		B_Gray = 0x80,
		B_LightBlue = 0x90,
		B_LightGreen = 0xA0,
		B_LightAqua = 0xB0,
		B_LightRed = 0xC0,
		B_LightPurple = 0xD0,
		B_LightYellow = 0xE0,
		B_BrightWhite = 0xF0

	};

	//  ===== Set Console Title - only works on External-Console =====
	static bool _stdcall Title(std::string_view title) noexcept {
		
		bool Success = SetConsoleTitleA(title.data());
		return Success;

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
