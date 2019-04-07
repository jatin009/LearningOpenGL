#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(void* figure, unsigned int size):m_FigureData(figure), m_Size(size)
{
	glGenBuffers(1, &m_RendererID);	
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Size, m_FigureData, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::ShiftXY(float x, float y)
{
	float* floatData = reinterpret_cast<float*>(m_FigureData);
	if (floatData)
	{
		*(floatData + 0) += x;  *(floatData + 1) += y;
		*(floatData + 7) += x;	*(floatData + 8) += y;
		*(floatData + 14) += x;	*(floatData + 15) += y;
		*(floatData + 21) += x;	*(floatData + 22) += y;

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, m_Size, floatData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}