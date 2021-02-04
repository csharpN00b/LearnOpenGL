#pragma once

namespace Logl
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int size);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const { return m_Count; }

	private:
		unsigned int m_Id;

	private:
		unsigned int m_Count;
	};
}