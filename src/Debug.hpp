// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#pragma once
#include <glad/glad.h>
#include "Logger.hpp"

GLenum glCheckError_(const char*, int);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 