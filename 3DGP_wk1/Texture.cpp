#include "Texture.h"

#include <stb_image.h>

#include <exception>

Texture::Texture(const std::string& _path)
	: m_TextureId(0)
	, m_width(0)
	, m_height(0)
	, m_dirty(false)
{
	unsigned char* data = stbi_load(_path.c_str(), &m_width, &m_height, NULL, 4);

	if (!data)
	{
		throw std::exception();
	}

	for (size_t i = 0; i < m_width * m_height * 4; ++i)
	{
		m_data.push_back(data[i]);
	}

	free(data);
	m_dirty = true;
}

Texture::~Texture()
{
	if (m_TextureId)
	{
		glDeleteTextures(1, &m_TextureId);
	}
}

GLuint Texture::TextureId()
{
	if (!m_TextureId)
	{
		glGenTextures(1, &m_TextureId);

		if (!m_TextureId)
		{
			throw std::exception();
		}
	}

	if (m_dirty)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_data.at(0));

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		m_dirty = false;
	}

	return m_TextureId;
}