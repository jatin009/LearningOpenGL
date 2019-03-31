#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer {

private:

	unsigned int m_RendererID;

public:

	IndexBuffer(const void* , unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
};

#endif
