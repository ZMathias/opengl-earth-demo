// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Logger.hpp"

void LogPerf(const std::string_view str)
{
	// get console attributes
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	// set console attributes
	constexpr char INFO[] = "PERF";
	WriteConsoleA(hConsole, "[", 1, nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	WriteConsoleA(hConsole, INFO, __builtin_strlen(INFO), nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, bufferInfo.wAttributes);
	WriteConsoleA(hConsole, "] ", 2, nullptr, nullptr);
	WriteConsoleA(hConsole, str.data(), static_cast<DWORD>(str.size()), nullptr, nullptr);
}

void LogInfo(const std::string_view str)
{
	// get console attributes
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	// set console attributes
	constexpr char INFO[] = "INFO";
	WriteConsoleA(hConsole, "[", 1, nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	WriteConsoleA(hConsole, INFO, __builtin_strlen(INFO), nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, bufferInfo.wAttributes);
	WriteConsoleA(hConsole, "] ", 2, nullptr, nullptr);
	WriteConsoleA(hConsole, str.data(), static_cast<DWORD>(str.size()), nullptr, nullptr);
}

void LogWarning(const std::string_view str)
{
	// get console attributes
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	// set console attributes
	constexpr char WARN[] = "WARN";
	WriteConsoleA(hConsole, "[", 1, nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, 14 | FOREGROUND_INTENSITY);
	WriteConsoleA(hConsole, WARN, __builtin_strlen(WARN), nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, bufferInfo.wAttributes);
	WriteConsoleA(hConsole, "] ", 2, nullptr, nullptr);
	WriteConsoleA(hConsole, str.data(), static_cast<DWORD>(str.size()), nullptr, nullptr);
}

void LogError(const std::string_view str)
{
	// get console attributes
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	// set console attributes
	constexpr char ERR[] = "ERROR";
	WriteConsoleA(hConsole, "[", 1, nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	WriteConsoleA(hConsole, ERR, __builtin_strlen(ERR), nullptr, nullptr);
	SetConsoleTextAttribute(hConsole, bufferInfo.wAttributes);
	WriteConsoleA(hConsole, "] ", 2, nullptr, nullptr);
	WriteConsoleA(hConsole, str.data(), static_cast<DWORD>(str.size()), nullptr, nullptr);
}
