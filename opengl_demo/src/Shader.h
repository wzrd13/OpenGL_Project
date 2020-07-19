#pragma once

#include <string>
#include <unordered_map>


class Shader
{
private:
	std::string m_FilePathFS;
	std::string m_FilePathVS;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocactionCache;

public:
	Shader(const std::string vs_path, const std::string& fs_path);
	~Shader(); 
	
	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4f(const std::string name, float v0, float v1, float f2, float f3);

private:
	unsigned int GetUniformLocation(const std::string& name);
	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(const unsigned int type, const std::string& source);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
