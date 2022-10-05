#include "pch.h"
#include "Texture.h"

#include "GL/glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Rendering/TextureFormat.h>

Texture::Texture()
{
	glGenTextures(1, &ID);

	SetWrapMode(GL_REPEAT);
	SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	SetMaxFilter(GL_LINEAR);

	Bind();
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	Unbind();
}

Texture::Texture(unsigned int width, unsigned int height, TextureFormat format) : Texture()
{
	this->width = width;
	this->height = height;

	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, ToBaseGLFormat(format), width, height, 0, ToBaseGLFormat(format), GL_FLOAT, nullptr);
	Unbind();
}

Texture::Texture(const std::string& path) : Texture()
{
	Load(path);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::Load(std::string path)
{
	int width, height, numberOfChannels;

	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numberOfChannels, 0);

	if (data)
	{
		Bind();

		this->width = width;
		this->height = height;

		int glFormat = 0;
		int glInternalFormat = 0;
		int glDataType = GL_UNSIGNED_BYTE;

		if (numberOfChannels == 1)
		{
			this->format = TextureFormat::R8;
			glFormat = GL_RED;
			glInternalFormat = GL_R8;
		}
		else if (numberOfChannels == 3)
		{
			this->format = TextureFormat::RGB8;
			glFormat = GL_RGB;
			glInternalFormat = GL_RGB8;
		}
		else if (numberOfChannels == 4)
		{
			this->format = TextureFormat::RGBA8;
			glFormat = GL_RGBA;
			glInternalFormat = GL_RGBA8;
		}
		else
		{
			Debug::LogError("Texture Loading", "Unsupported number of channels.");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glDataType, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Debug::LogError("Texture", "Failed to load a texture.");
	}

	stbi_image_free(data);
	Unbind();
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetWrapMode(int wrapMode)
{
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrapMode);

	Unbind();
}

void Texture::SetMinFilter(int minFilter)
{
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	Unbind();
}

void Texture::SetMaxFilter(int maxFilter)
{
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

	Unbind();
}
