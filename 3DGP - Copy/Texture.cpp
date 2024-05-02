#include "Texture.h"

#include <stb_image.h>
#include <exception>

Texture::Texture(const std::string& _path)
	:m_Id(0)
	,m_Width(0)
	,m_Height(0)
	,m_Dirty(false)
{

	unsigned char* data = stbi_load(_path.c_str(), &m_Width, &m_Height, NULL, 4);

	if (!data)
	{
		throw std::exception();
	}

	for (size_t i = 0; i < m_Width * m_Height * 4; i++)
	{
		m_Data.push_back(data[i]);
	}

	m_Dirty = true;
	free(data);
}

GLuint Texture::getId()
{
	if (!m_Id)
	{
		glGenTextures(1, &m_Id);

		if (!m_Id)
		{
			throw std::exception();
		}
	}
	
	if (m_Dirty)
	{
		glBindTexture(GL_TEXTURE_2D, m_Id);

		//upload image data to the bound texture unit in the gpu
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, &m_Data.at(0));

		//generated mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		//unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D,0);

		m_Dirty = false;
	}

	return m_Id;
}