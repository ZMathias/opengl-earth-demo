// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include <Windows.h>
#include <string_view>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <glad/glad.h>

const inline HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
void LogPerf(const std::string_view);
void LogInfo(const std::string_view);
void LogWarning(const std::string_view);
void LogError(const std::string_view);