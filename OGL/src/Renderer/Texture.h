#pragma once

namespace Logl
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const Texture&) = delete;
		virtual ~Texture() = default;
		
		Texture& operator=(const Texture&) = delete;

		//virtual int GetWidth() const = 0;
		//virtual int GetHeight() const = 0;

		virtual void Bind() const = 0;
		virtual void Bind(unsigned int index) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const char* path);
		
		virtual ~Texture2D();

		virtual void Bind() const override;
		virtual void Bind(unsigned int index) const override;

	private:
		unsigned int m_Id;
	};
}