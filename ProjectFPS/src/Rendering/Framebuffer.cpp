#include "pch.h"
#include "Framebuffer.h"

#include "GL/glew.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height, TextureFormat format, bool cubeTexture)
{
	glGenFramebuffers(1, &FBO);

	this->width = width;
	this->height = height;

	SetDepthAttachment(format, cubeTexture);
}

Framebuffer::~Framebuffer()
{
	Invalidate();
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//for (size_t i = 0; i < colorAttachments.size(); i++)
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i]->ID, 0);

	if (depthAttachment != nullptr)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->ID, 0);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Invalidate()
{
	if (FBO != 0)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AddColorAttachment(TextureFormat format, bool cubeTexture)
{
	colorAttachments.push_back(std::make_unique<Texture>(width, height, format));
}

void Framebuffer::SetDepthAttachment(TextureFormat format, bool cubeTexture)
{
	depthAttachment = std::make_unique<Texture>(width, height, format);
	depthAttachment->SetMinFilter(GL_LINEAR);
	depthAttachment->SetMaxFilter(GL_LINEAR);
	depthAttachment->SetWrapMode(GL_CLAMP_TO_BORDER);

	depthAttachment->Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	depthAttachment->Unbind();
}

//void Framebuffer::Resize(unsigned int width, unsigned int height)
//{
//	Invalidate();
//}
