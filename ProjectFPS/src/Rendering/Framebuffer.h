#pragma once

#include <Rendering/Texture.h>

enum class TextureFormat;

class Framebuffer
{
public:
	Framebuffer(unsigned int width, unsigned int height, TextureFormat format, bool cubeTexture);
	~Framebuffer();

	void Bind();
	void Unbind();
	void Invalidate();

	void AddColorAttachment(TextureFormat format, bool cubeTexture);
	void SetDepthAttachment(TextureFormat format, bool cubeTexture);
	//void Resize(unsigned int width, unsigned int height);

private:
	friend class Renderer;

	unsigned int FBO = 0;
	unsigned int width = 0, height = 0;

	std::vector<std::unique_ptr<Texture>> colorAttachments;
	std::unique_ptr<Texture> depthAttachment;
};

