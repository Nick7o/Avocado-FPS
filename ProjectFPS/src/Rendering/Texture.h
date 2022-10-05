#pragma once

#include <string>
//#include "TextureFormat.h"
enum class TextureFormat;

class Texture
{
public:
	Texture();
	Texture(unsigned int width, unsigned int height, TextureFormat format);
	Texture(const std::string& path);
	~Texture();

	virtual void Load(std::string path);
	virtual void Bind();
	virtual void Unbind();

	virtual void SetWrapMode(int wrapMode);
	virtual void SetMinFilter(int minFilter);
	virtual void SetMaxFilter(int maxFilter);

	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }
	inline TextureFormat GetFormat() const { return format; }

protected:
	friend class Renderer;
	friend class Framebuffer;

	unsigned int ID = 0;
	unsigned int width, height;
	TextureFormat format;
};

