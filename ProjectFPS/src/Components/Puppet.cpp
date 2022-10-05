#include "pch.h"
#include "Puppet.h"

#include "Transform.h"
#include "CollisionController.h"
#include "CameraController.h"

#include "Rendering/Renderer.h"
#include "Shapes.h"

void Puppet::Start()
{
	collisionController = SceneManager::GetInstance().GetCurrentScene()->GetComponent<CollisionController>();
	if (collisionController == nullptr)
		Debug::LogError("Puppet", "Collision controller not found.");
}

void Puppet::HandlePhysics(float deltaTime)
{
	auto t = GetGameObject()->GetTransform();
	auto pos = t->Position;
	posChange = velocity * deltaTime;
	auto newPos = pos + posChange;
	auto velDir = glm::normalize(velocity);

	// Ground detection
	glm::vec3 roGround = pos + glm::vec3(0, 0.1f, 0.0f);
	glm::vec3 rdGround = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

	HitInfo groundHitInfo;
	if (collisionController->Raycast(roGround, rdGround, groundHitInfo, collider))
	{
		//glm::mat4 m(1.0f);
		//m = glm::translate(m, groundHitPoint);
		//Renderer::DrawMeshWireframe(Shapes::CreateCube(glm::vec3(0.1f)), nullptr, m, CameraController::GetCurrent()->camera);

		if (groundHitInfo.intersectionPoint.y > newPos.y)
		{
			isGrounded = true;
			velocity.y = 0;
		}

		newPos.y = fmax(groundHitInfo.intersectionPoint.y, newPos.y);
	}

	// Wall detection
	glm::vec3 roWall = pos + glm::vec3(0, 1.0f, 0.0f);
	glm::vec3 rdWall = glm::normalize(glm::vec3(velDir.x, 0.0f, velDir.z));

	for (int i = 0; i < collisionRays; i++)
	{
		HitInfo wallHitInfo;
		if (glm::length2(velocity) > 0.01f && !glm::isnan(rdWall).r)
		{
			rdWall = glm::quat(glm::vec3(0, glm::radians(360.0f / collisionRays), 0)) * rdWall;

			if (collisionController->Raycast(roWall, rdWall, wallHitInfo, collider))
			{
				//glm::mat4 m(1.0f);
				//m = glm::translate(m, wallHitPoint);
				//Renderer::DrawMeshWireframe(Shapes::CreateCube(glm::vec3(0.1f)), nullptr, m, CameraController::GetCurrent()->camera);

				glm::vec2 wallHitXZ = glm::vec2(wallHitInfo.intersectionPoint.x, wallHitInfo.intersectionPoint.z);
				glm::vec2 roWallXZ = glm::vec2(roWall.x, roWall.z);
				glm::vec2 newPosXZ = glm::vec2(newPos.x, newPos.z);

				float distance = glm::distance(wallHitXZ, roWallXZ) - radius;
				float newPosDistance = glm::distance(wallHitXZ, newPosXZ) - radius;

				if (newPosDistance < 0)
				{
					auto correctPos = wallHitInfo.intersectionPoint - rdWall * radius;
					newPos.x = correctPos.x;
					newPos.z = correctPos.z;
					velocity.x = 0;
					velocity.z = 0;
					posChange.x = 0;
					posChange.z = 0;
				}
			}
		}
	}

	// Deceleration
	velocity.x *= 0.96f;
	velocity.z *= 0.96f;
	velocity.x -= posChange.x;
	velocity.z -= posChange.z;

	if (posChange.y > 0.01f)
		isGrounded = false;

	t->Position = newPos;
}

void Puppet::TakeDamage(float damage)
{
	health -= damage;
}
