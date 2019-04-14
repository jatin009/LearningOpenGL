#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
	void SetUniform(const std::string& name, T f0)
	{
		return;
	}
	template<>
	void SetUniform<float>(const std::string& name, float f0)
	{
		if (m_UniformLocs.find(name) != m_UniformLocs.end())
		{
			glUniform1f(m_UniformLocs[name], f0);
			return;
		}

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			m_UniformLocs[name] = location;
			glUniform1f(location, f0);
		}
	}
	template<>
	void SetUniform<int>(const std::string& name, int f0)
	{
		if (m_UniformLocs.find(name) != m_UniformLocs.end())
		{
			glUniform1i(m_UniformLocs[name], f0);
			return;
		}

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			m_UniformLocs[name] = location;
			glUniform1i(location, f0);
		}
	}
	template <>
	void SetUniform<glm::mat4>(const std::string& name, glm::mat4 trans)
	{
		if (m_UniformLocs.find(name) != m_UniformLocs.end())
		{
			glUniformMatrix4fv(m_UniformLocs[name], 1, GL_FALSE, glm::value_ptr(trans));
			return;
		}

		int loc = glGetUniformLocation(m_RendererID, name.c_str());
		if (loc != -1)
		{
			m_UniformLocs[name] = loc;
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(trans));
		}
	}
	void SetUniformF(const std::string& name, float f0, float f1, float f2, float f3);

private:
	std::stringstream ParseShader(const std::string& filepath);
	std::unordered_map<std::string, int> m_UniformLocs;
	unsigned int CreateShader(unsigned int type, const std::string& source);
};

#endif