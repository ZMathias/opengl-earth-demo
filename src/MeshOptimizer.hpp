// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include <unordered_map>
#include <boost/functional/hash.hpp>

void optimizeMesh3D(const float [], const int& len, std::vector<float>& outVertices, std::vector<unsigned int>& outIndices);