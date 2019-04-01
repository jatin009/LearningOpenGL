#include "GL/glew.h"
#include "Texture.h"
#include "stb_image.h"
#include <string>
#include <iostream>

Texture::Texture(const char* filepath, int wrapMode)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	//set the texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(filepath, &m_Width, &m_Height, &m_BPP, 0);

	if (m_LocalBuffer)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, (std::string(filepath).find(".png") != std::string::npos) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
	}
	else
	{
		std::cout << "Image file not loaded successfully - " << filepath << "." << std::endl;
	}
	stbi_image_free(m_LocalBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}