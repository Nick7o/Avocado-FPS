#include "pch.h"
#include "TextureFormat.h"

unsigned int ToGLFormat(const TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::R8:
		return GL_R8;
	case TextureFormat::R16:
		return GL_R16;
	case TextureFormat::D32:
		return GL_DEPTH_COMPONENT32;
	case TextureFormat::RGB8:
		return GL_RGB8;
	case TextureFormat::RGB16F:
		return GL_RGB16F;
	case TextureFormat::RGBA8:
		return GL_RGBA8;
	case TextureFormat::RGBA16F:
		return GL_RGBA16F;
	default:
		return 0;
	}
}

unsigned int ToBaseGLFormat(const TextureFormat& format)
{
	switch (format)
	{
	case TextureFormat::R8:
	case TextureFormat::R16:
		return GL_RED;
	case TextureFormat::D32:
		return GL_DEPTH_COMPONENT;
	case TextureFormat::RGB8:
	case TextureFormat::RGB16F:
		return GL_RGB;
	case TextureFormat::RGBA8:
	case TextureFormat::RGBA16F:
		return GL_RGBA;
	default:
		return 0;
	}
}
