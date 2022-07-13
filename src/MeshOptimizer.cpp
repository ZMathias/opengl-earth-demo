// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MeshOptimizer.hpp"

void optimizeMesh3D(const float vertices[], const long long len, std::vector<float>& outVertices, std::vector<unsigned int>& outIndices)
{
	std::unordered_map <std::tuple<float, float, float>, unsigned int, boost::hash<std::tuple<float, float, float>>> uniqueIndices;
	unsigned int uniqueIndex{};
	for (long long i = 0; i < len * 3; i += 3)
	{
		const std::tuple coord(vertices[i], vertices[i + 1], vertices[i + 2]);
		if (!uniqueIndices.contains(coord))
		{
			uniqueIndices.insert({coord, uniqueIndex});
			uniqueIndex++;
			auto [first, second, third] = coord;
			outVertices.emplace_back(first);
			outVertices.emplace_back(second);
			outVertices.emplace_back(third);
		}
	}

	outIndices.reserve(len * 3);
	for (long long i = 0; i < len * 3; i += 3)
	{
		const std::tuple coord(vertices[i], vertices[i + 1], vertices[i + 2]);
		outIndices.push_back(uniqueIndices[coord]);
	}
}
