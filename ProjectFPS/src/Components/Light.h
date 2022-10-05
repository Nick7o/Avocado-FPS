#pragma once

#include "glm/glm.hpp"

#include "Component.h"
#include "Camera.h"

enum class LightType { Directional, Spot, Point };

struct Attenuation
{
	float constant = 1.0f, linear = 0.7f, quadratic = 1.8f;

	glm::vec3 ToVec3() const { return glm::vec3(constant, linear, quadratic); }
};

class Light : public Component
{
public:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;
	float spotAngle = glm::radians(15.0f);
	float outerSpotAngle = glm::radians(25.0f);

	LightType type = LightType::Directional;
	Attenuation attenuation;

	void SetSpotAngleDegrees(float spotAngleInDegrees) { spotAngle = glm::radians(spotAngleInDegrees); }
	void SetSpotAngleDegrees(float spotAngleInDegrees, float outerAngle)
	{
		spotAngle = glm::radians(spotAngleInDegrees);
		outerSpotAngle = glm::radians(outerAngle);
	}

	/* Get a camera view from the light. Face id is only used for spot lights. */
	std::shared_ptr<Camera> GetCamera(int faceId = 0);

private:
	void Start() override;
	void Update(float deltaTime) override;

	std::shared_ptr<Camera> internalCameraRef;
};

