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

	template<typename T>
	void SetUniform1(const std::string& name, T f0)
	{
		return;
	}
	template<>
	void SetUniform1<float>(const std::string& name, float f0)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform1f(location, f0);
		}
	}
	template<>
	void SetUniform1<int>(const std::string& name, int f0)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform1i(location, f0);
		}
	}
	void SetUniformF(const std::string& name, float f0, float f1, float f2, float f3);

private:
	std::stringstream ParseShader(const std::string& filepath);
	std::unordered_map<std::string, int> m_UniformLocs;
	unsigned int CreateShader(unsigned int type, const std::string& source);
};

#endif