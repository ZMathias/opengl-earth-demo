// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Logger.hpp"
#include <fstream>

class Shader
{
	unsigned int vertexShaderId{}, fragmentShaderId{}, controlShaderId{}, evaluationShaderId{}, programId{};
	std::string vertexShader, fragmentShader;

public:

	// only file reading is done at construction time to allow modification of shaderCode programatically if needed
	Shader(const char* vertexPath, const char* fragmentPath, const char* tcsPath = nullptr, const char* tesPath = nullptr) {
		// open both files and read their contents fully into vertexShader and fragmentShader in that order
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);
		std::ifstream controlFile;
		std::ifstream evaluationFile;
		if (tcsPath != nullptr && tesPath != nullptr)
		{
			controlFile.open(tcsPath);
			evaluationFile.open(tesPath);
			if (!(controlFile.is_open() && evaluationFile.is_open()))
			{
				LogError("Failed to open control and evaluation shader files");
				return;
			}
		}
		if (!(vertexFile.is_open() && fragmentFile.is_open()))
		{
			LogError("Failed to open shader files: " + std::string(vertexPath) + " and " + std::string(fragmentPath) + "\n");
		}
		else
		{
			vertexFile.seekg(0, std::ios::end);
			fragmentFile.seekg(0, std::ios::end);
			vertexShader.reserve(vertexFile.tellg());
			fragmentShader.reserve(fragmentFile.tellg());
			vertexFile.seekg(0, std::ios::beg);
			fragmentFile.seekg(0, std::ios::beg);
			vertexShader.assign(std::istreambuf_iterator<char>(vertexFile), std::istreambuf_iterator<char>());
			fragmentShader.assign(std::istreambuf_iterator<char>(fragmentFile), std::istreambuf_iterator<char>());
			vertexFile.close();
			fragmentFile.close();
		}
	}

	// tries to compile both shaders and links them into a program
	// BOTH SHADERS HAVE TO BE POPULATED BEFORE CALLING THIS FUNCTION
	bool CompileShaders() {
		// compile vertex shader first
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		const char* shaderCode = vertexShader.c_str();
		glShaderSource(vertexShaderId, 1, &shaderCode, nullptr);
		glCompileShader(vertexShaderId);
		int success;
		char infoLog[512]{};
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
			LogError(infoLog);
			return GL_FALSE;
		}

		//then compile fragment shader
		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		shaderCode = fragmentShader.c_str();
		glShaderSource(fragmentShaderId, 1, &shaderCode, nullptr);
		glCompileShader(fragmentShaderId);
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
			LogError(infoLog);
			return GL_FALSE;
		}

		return GL_TRUE;
	}

	bool CreateAndLinkProgram() {
		programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);
		int success;
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512]{};
			glGetProgramInfoLog(programId, 512, nullptr, infoLog);
			LogError(infoLog);
			return GL_FALSE;
		}
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		vertexShader.clear();
		fragmentShader.clear();
		return GL_TRUE;
	}

	// helper functions to set uniforms
	void SetBool(const std::string_view name, bool value) const {
		glUniform1i(glGetUniformLocation(programId, name.data()), (int)value);
	}

	void SetInt(const std::string_view name, int value) const {
		glUniform1i(glGetUniformLocation(programId, name.data()), value);
	}

	void SetFloat(const std::string_view name, float value) const {
		glUniform1f(glGetUniformLocation(programId, name.data()), value);
	}

	void SetVec2(const std::string_view name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(programId, name.data()), 1, &value[0]);
	}

	void SetVec2(const std::string_view name, const float x, const float y) const {
		glUniform2f(glGetUniformLocation(programId, name.data()), x, y);
	}

	void SetVec3(const std::string_view name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(programId, name.data()), 1, &value[0]);
	}

	void SetVec3(const std::string_view name, const float x, const float y, const float z) const {
		glUniform3f(glGetUniformLocation(programId, name.data()), x, y, z);
	}

	void SetVec4(const std::string_view name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(programId, name.data()), 1, &value[0]);
	}

	void SetVec4(const std::string_view name, const float x, const float y, const float z, const float w) const {
		glUniform4f(glGetUniformLocation(programId, name.data()), x, y, z, w);
	}

	void SetMat2(const std::string_view name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(programId, name.data()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat3(const std::string_view name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(programId, name.data()), 1, GL_FALSE, &mat[0][0]);
	}

	void SetMat4(const std::string_view name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(programId, name.data()), 1, GL_FALSE, &mat[0][0]);
	}

	void UseProgram() const {
		glUseProgram(programId);
	}
	static void UnbindShader() {
		glUseProgram(0);
	}

	[[nodiscard]] unsigned int GetProgramId() const {
		return programId;
	}
};

