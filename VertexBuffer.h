#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer {

private:

	unsigned int m_RendererID;
	void* m_FigureData;
	unsigned int m_Size;

public:
	
	VertexBuffer(void* , unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void ShiftXY(float x, float y);
};

#endif