#pragma once

#include "GL/glew.h"

enum class TextureFormat
{
	R8,
	R16,
	D32,
	RGB8,
	RGB16F,
	RGBA8,
	RGBA16F,
};

unsigned int ToGLFormat(const TextureFormat& format);
unsigned int ToBaseGLFormat(const TextureFormat& format);