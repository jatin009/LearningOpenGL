#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "VertexLayout.h"

VertexArray::VertexArray(): m_RendererID(0)
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddLayout(VertexLayout& layout)
{
	std::vector<LayoutElement> vLayouts = layout.GetElements();

	for (int i = 0; i < vLayouts.size(); i++)
	{
		glVertexAttribPointer(i, vLayouts[i].count, vLayouts[i].type, vLayouts[i].normalized, layout.GetStride(), vLayouts[i].offset);
		glEnableVertexAttribArray(i);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}