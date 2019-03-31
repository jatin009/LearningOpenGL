#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

class Shader
{
private:
	//program id
	unsigned int m_RendererID;

public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	void Use() const;
	void UnUse() const;

	void SetUniformF(const std::string& name, float f0);
	void SetUniformF(const std::string& name, float f0, float f1, float f2, float f3);

private:
	std::stringstream ParseShader(const std::string& filepath);
	std::unordered_map<std::string, int> m_UniformLocs;
	unsigned int CreateShader(unsigned int type, const std::string& source);
};

#endif