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
	unsigned int refCount{};
	
public:
	Texture()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	//automatically selects appropriate loading mode based on file extension
	void AddTexture(const std::string& path, unsigned int i, GLenum internalfmt = GL_RGBA8)
	{
		int width{}, height{}, nrChannels{};
		stbi_set_flip_vertically_on_load(false);

		unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 0);
		if (data == nullptr) LogError(("Failed to open texture at specified path: " + path + "\n").c_str());

		GLenum bpc{4};
		switch (nrChannels)
		{
		case 1:
			bpc = GL_RED;
			break;
		case 2:
			bpc = GL_RG;
			break;
		case 3:
			bpc = GL_RGB;
			break;
		case 4:
			bpc = GL_RGBA;
			break;
		}

		glGenTextures(1, &textureIds[i]);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, internalfmt, width, height, 0, bpc, GL_UNSIGNED_BYTE, data);
		
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
		refCount++;
	}

	void BindAllTextures() const
	{
		for (unsigned int i = 0; i < refCount; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		}
	}

	void BindTexture(unsigned int i) const
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
	}

	void UnbindAllTextures() const
	{
		for (unsigned int i = 0; i < refCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void UnbindTexture(unsigned int i) const
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};