#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Logl
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		~VertexArray();

		VertexArray& operator=(const VertexArray&) = delete;

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const VertexBuffer& vertexBuffer);
		void SetIndexBuffer(const IndexBuffer& indexBuffer);

		unsigned int GetCount() const { return m_Count; }
		bool IsUsingIndex() const { return m_bUseIndex; }

	private:
		unsigned int m_Id;

	private:
		unsigned int m_Count = 0;
		bool m_bUseIndex = false;
	};
}