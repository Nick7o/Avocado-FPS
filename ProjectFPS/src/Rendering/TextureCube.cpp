#include "pch.h"
#include "TextureCube.h"

#include <stb_image.h>
#include <GL/glew.h>
#include <Rendering/TextureFormat.h>

void TextureCube::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void TextureCube::Unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::SetWrapMode(int wrapMode)
{
	Bind();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);

	Unbind();
}

void TextureCube::SetMinFilter(int minFilter)
{
	Bind();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);

	Unbind();
}

void TextureCube::SetMaxFilter(int maxFilter)
{
	Bind();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, maxFilter);

	Unbind();
}

void TextureCube::Load(std::string path)
{
	Bind();

	static std::vector<std::string> faces { "px", "nx", "py", "ny", "pz", "nz" };

	size_t lastDotIndex = path.find_last_of(".");
	std::string basePath = path.substr(0, lastDotIndex);
	std::string extension = path.substr(lastDotIndex, path.length());

	int faceId = 0;
	for (auto face : faces)
	{
		LoadFace(basePath + face + extension, faceId++);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	SetMaxFilter(GL_LINEAR);

	Unbind();
}

void TextureCube::LoadFace(const std::string& path, int faceId)
{
	int width, height, numberOfChannels;

	bool isHDR = stbi_is_hdr(path.c_str());
	float* data = stbi_loadf(path.c_str(), &width, &height, &numberOfChannels, 0);

	if (data)
	{
		Bind();

		this->width = width;
		this->height = height;

		int glDataType = GL_FLOAT;

		if (numberOfChannels == 1)
			this->format = TextureFormat::R8;
		else if (numberOfChannels == 3)
			this->format = TextureFormat::RGB16F;
		else if (numberOfChannels == 4)
			this->format = TextureFormat::RGBA16F;
		else
			Debug::LogError("Texture Loading", "Unsupported number of channels.");

		int glFormat = ToBaseGLFormat(this->format);
		int glInternalFormat = ToGLFormat(this->format);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceId,
			0, glInternalFormat, width, height, 0, glFormat, glDataType, data);
	}
	else
	{
		Debug::LogError("TextureCube::LoadFace", "Failed to load a texture.");
	}

	stbi_image_free(data);
}
