#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include <vector>
#include "GL/glew.h"

struct LayoutElement
{
	int count;
	unsigned int type;
	unsigned int normalized;
	void * offset;
};

class VertexLayout
{
private:
	std::vector<LayoutElement> m_vLayouts;
	int m_Stride;

public:

	VertexLayout() : m_Stride(0) {}
	
	template <typename T>
	void Push(int count, void * offset)
	{
		static_assert(false);
	}

	template <>
	void Push<float>(int count, void* offset)
	{
		m_vLayouts.push_back({ count, GL_FLOAT, GL_FALSE, offset });
	}

	inline void SetStride(int stride)
	{
		m_Stride = stride;
	}

	inline int GetStride() const
	{
		return m_Stride;
	}

	inline std::vector<LayoutElement>& GetElements()
	{
		return m_vLayouts;
	}
};

#endif