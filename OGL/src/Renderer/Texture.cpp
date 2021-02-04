#include "Texture.h"

#include "glad/glad.h"
#include "stb/stb_image.h"

namespace Logl
{
	Texture2D::Texture2D(const char* path, int format)
	{
		m_Id = 0;

		int width{}, height{}, nrChannels{};
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glGenTextures(1, &m_Id);
			glBindTexture(GL_TEXTURE_2D, m_Id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else
			__debugbreak();
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Id);
	}

	void Texture2D::Bind(unsigned int index) const
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}
}