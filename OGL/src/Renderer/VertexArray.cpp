#include "glad/glad.h"

#include "VertexArray.h"


namespace Logl
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_Id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_Id);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer)
	{
		if (vertexBuffer.GetBufferLayout().GetElements().size() == 0)
		{
			// TODO: 
			__debugbreak();
		}

		glBindVertexArray(m_Id);
		vertexBuffer.Bind();

		auto& bufferLayout = vertexBuffer.GetBufferLayout();
		auto stride = bufferLayout.GetStride();
		int i = 0;
		for (auto& element : bufferLayout)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.Count, element.DataType, element.Normalized ? GL_TRUE : GL_FALSE, stride, (const void*)element.Offset);
			i++;
		}

		if (!m_bUseIndex)
			m_Count = vertexBuffer.GetCount();
	}

	void VertexArray::SetIndexBuffer(const IndexBuffer& indexBuffer)
	{
		glBindVertexArray(m_Id);
		indexBuffer.Bind();

		m_bUseIndex = true;
		m_Count = indexBuffer.GetCount();
	}
}