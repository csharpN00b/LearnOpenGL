#include "glad/glad.h"

#include "Core/Base.h"
#include "VertexBuffer.h"

namespace Logl
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
		: m_Count(size)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const void* data, unsigned int size, std::initializer_list<BufferElement> elements)
		: m_Layout(elements)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		m_Count = size / m_Layout.GetStride();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void VertexBuffer::SetBufferLayout(const BufferLayout& layout) 
	{
		ASSERT(layout.GetElements().size());
		ASSERT(layout.GetStride());

		m_Layout = layout; 
		m_Count /= layout.GetStride(); 
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}