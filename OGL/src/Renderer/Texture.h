#pragma once

namespace Logl
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		//virtual int GetWidth() const = 0;
		//virtual int GetHeight() const = 0;

		virtual void Bind(unsigned int index) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const char* path, int format);
		virtual ~Texture2D();

		virtual void Bind(unsigned int index) const override;

	private:
		unsigned int m_Id;
	};
}