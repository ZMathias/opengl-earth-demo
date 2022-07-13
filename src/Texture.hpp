// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include <stack>
#include <string_view>
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include "stb_image.h"
#include "debug.hpp"

class Texture
{
	unsigned int textureIds[15]{};

	
public:
	Texture()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	//automatically selects appropriate loading mode based on file extension
	void AddTexture(const std::string& path, unsigned int i)
	{

		std::stack<char> st;
		for (auto i = path.length() - 1; path[i] != '.'; i--) st.push(path[i]);
		std::string ext;
		ext.reserve(st.size());
		for (; !st.empty(); st.pop()) ext.push_back(st.top());

		int width{}, height{}, nrChannels{};
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
		if (data == nullptr) LogError(("Failed to open texture at specified path: " + path + "\n").c_str());
		glGenTextures(1, &textureIds[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		if (ext == "png") glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void BindTexture(unsigned int i) const
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
	}

	static void UnbindTexture(unsigned int i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};