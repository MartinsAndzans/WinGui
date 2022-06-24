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

std::ostream& operator<<(std::ostream &out, char8_t Char) {
	return out << static_cast<char>(Char);
}

struct Console {

	static bool _stdcall SetTitle(const std::string &Title) noexcept {
		return SetConsoleTitleA(Title.c_str());
	}

	// #	%i - Integer	#	%d - Double	#
	// #	%f - Float	#	%c - Char	#
	// #	%s - String	#
	static void _stdcall Print(const std::string &message, byte Color = 0x0F, char8_t end = '\n') {
		
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, Color);

		std::cout << message.c_str() << end;

	}

	static std::string _stdcall Input(const std::string &message = "> ", byte Color = 0x0F) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
		
		std::string Buffer;
		std::cout << message.c_str();
		std::getline(std::cin, Buffer);

		return Buffer;

	}

	static char8_t _stdcall Pause(const std::string &message = "Press any key to continue . . .", byte Color = 0x0F) noexcept {
		
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);
		
		std::cout << message.c_str();

		fflush(stdin);
		char8_t key = _getch();
		std::cout << std::endl;

		return key;

	}

	static void _stdcall DrawRectangleInConsole(USHORT Width = 40, USHORT Height = 20, const char8_t Symbol = '#', byte Color = 0x0F) noexcept {

		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, FOREGROUND_INTENSITY | Color);

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
