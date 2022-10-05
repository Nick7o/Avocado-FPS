#pragma once

#include "Texture.h"

class TextureCube : public Texture
{
public:
	void Bind() override;
	void Unbind() override;

	void SetWrapMode(int wrapMode) override;
	void SetMinFilter(int minFilter) override;
	void SetMaxFilter(int maxFilter) override;

	void Load(std::string path) override;

private:
	void LoadFace(const std::string& path, int faceId);
};

