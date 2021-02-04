#pragma once

#include <unordered_map>

namespace Logl
{
	static std::unordered_map<unsigned int, unsigned int> s_DataSize =
	{
		{GL_FLOAT, sizeof(float)},
	};


	struct BufferElement
	{
		unsigned int DataType;
		int Count;
		bool Normalized;
		int Offset;

		BufferElement(unsigned int dataType, int count, bool normalized = false)
			: DataType(dataType), Count(count), Normalized(normalized), Offset(0)
		{
		}
	};


	class BufferLayout
	{
	public:
		BufferLayout(){}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateLayout();
		}

		int GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		
		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateLayout()
		{
			m_Stride = 0;
			int i = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Count * s_DataSize[element.DataType];
			}
		}

	private:
		int m_Stride = 0;
		std::vector<BufferElement> m_Elements;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void SetBufferLayout(const BufferLayout& layout) { m_Layout = layout; m_Count /= layout.GetStride(); }
		const BufferLayout& GetBufferLayout() const { return m_Layout; }

		void Bind() const;
		void Unbind() const;

		unsigned int GetCount() const { return m_Count; }

	private:
		unsigned int m_Id;
		BufferLayout m_Layout;

	private:
		unsigned int m_Count;
	};
}