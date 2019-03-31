#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

class VertexLayout;

class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddLayout(VertexLayout& layout);

	void Bind() const;
	void UnBind() const;
};

#endif