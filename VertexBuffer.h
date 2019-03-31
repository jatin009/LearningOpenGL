#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer {

private:

	unsigned int m_RendererID;

public:
	
	VertexBuffer(const void* , unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};

#endif
