#include "pch.h"
#include "LevelDescription.h"

#include "MeshImporter.h"
#include "Components/MeshRenderer.h"
#include "Components/Light.h"
#include "Components/Transform.h"
#include "Components/CollisionController.h"
#include "Components/Player.h"
#include "Components/Enemy.h"

#include "Rendering/Renderer.h"
#include "Rendering/TextureCube.h"

#include "glm/gtx/string_cast.hpp"

void LevelDescription::Load()
{
	auto ccgo = new GameObject();
	auto collisionController = ccgo->AddComponent(new CollisionController());
	ccgo->AddComponent(new BoxCollider());

	CreateLights();
	CreatePlayer();

	// BARREL
	auto bDiff = std::make_shared<Texture>("resources/Models/barrel_03_1k/textures/barrel_03_diff_1k.jpg");
	auto bNorm = std::make_shared<Texture>("resources/Models/barrel_03_1k/textures/barrel_03_nor_gl_1k.jpg");
	auto bMask = std::make_shared<Texture>("resources/Models/barrel_03_1k/textures/barrel_03_arm_1k.jpg");

	auto bMat = std::make_shared<Material>(Shader::Find("Standard"));
	bMat->SetTexture("diffuseMap", bDiff);
	bMat->SetTexture("normalMap", bNorm);
	bMat->SetTexture("maskMap", bMask);

	auto barrelRenderer = LoadAndCreate("resources/Models/barrel_03_1k/barrel_03_1k.fbx");
	barrelRenderer->GetGameObject()->GetTransform()->Position = { 0.5f, .4f, 0.0f };
	barrelRenderer->materials.push_back(bMat);

	LoadLevel();
	CreateCollision(*collisionController);

	CreateEnemy(glm::vec3(14.0f, 0.0f, 13.0f));
	CreateEnemy(glm::vec3(17.0f, 0.0f, 12.0f));
	CreateEnemy(glm::vec3(10.0f, 0.0f, 14.0f));
	CreateEnemy(glm::vec3(7.0f, 0.0f, 12.0f));
	CreateEnemy(glm::vec3(8.0f, 0.0f, 11.0f));
	CreateEnemy(glm::vec3(30.0f, 0.0f, -4.0f));
	CreateEnemy(glm::vec3(50.0f, 0.0f, 12.0f));
	CreateEnemy(glm::vec3(52.0f, 0.0f, 8.0f));
	CreateEnemy(glm::vec3(54.0f, 0.0f, 6.0f));
	CreateEnemy(glm::vec3(46.0f, 0.0f, 14.0f));
	CreateEnemy(glm::vec3(48.0f, 0.0f, 2.0f));
}

void LevelDescription::LoadLevel()
{
	std::string basePath = "resources/Models/Level0/";
	auto lvl0Renderer = LoadAndCreate(basePath + "level-3.fbx");
	lvl0Renderer->GetGameObject()->GetTransform()->Rotation = { glm::radians(-90.0f), 0.0f, 0.0f};
	lvl0Renderer->GetGameObject()->GetTransform()->Scale = { 2.0f, 2.0f, 2.0f };

	// Wall 0
	auto wall0Diff = std::make_shared<Texture>(basePath + "sciana-normal/3_col.png");
	auto wall0Norm = std::make_shared<Texture>(basePath + "sciana-normal/3_nrm.png");
	auto wall0Mask = std::make_shared<Texture>(basePath + "sciana-normal/3_arm.png");

	auto wall0Mat = std::make_shared<Material>(Shader::Find("Standard"));
	wall0Mat->SetTexture("diffuseMap", wall0Diff);
	wall0Mat->SetTexture("normalMap", wall0Norm);
	wall0Mat->SetTexture("maskMap", wall0Mask);

	// Wall 1
	auto wall1Diff = std::make_shared<Texture>(basePath + "sciana-inna/Components_col.png");
	auto wall1Norm = std::make_shared<Texture>(basePath + "sciana-inna/Components_nrm.png");
	auto wall1Mask = std::make_shared<Texture>(basePath + "sciana-inna/Components_arm.png");

	auto wall1Mat = std::make_shared<Material>(Shader::Find("Standard"));
	wall1Mat->SetTexture("diffuseMap", wall1Diff);
	wall1Mat->SetTexture("normalMap", wall1Norm);
	wall1Mat->SetTexture("maskMap", wall1Mask);

	// Box
	auto boxDiff = std::make_shared<Texture>(basePath + "skrzynia/skrzynia_col.skrzynia.png");
	auto boxNorm = std::make_shared<Texture>(basePath + "skrzynia/skrzynia_nrm.skrzynia.png");
	auto boxMask = std::make_shared<Texture>(basePath + "skrzynia/skrzynia_arm.skrzynia.png");

	auto boxMat = std::make_shared<Material>(Shader::Find("Standard"));
	boxMat->SetTexture("diffuseMap", boxDiff);
	boxMat->SetTexture("normalMap", boxNorm);
	boxMat->SetTexture("maskMap", boxMask);

	// Floor & Ceiling
	auto floorDiff = std::make_shared<Texture>(basePath + "podloga-sufit/3_col.png");
	auto floorNorm = std::make_shared<Texture>(basePath + "podloga-sufit/3_nrm.png");
	auto floorMask = std::make_shared<Texture>(basePath + "podloga-sufit/3_arm.png");

	auto floorMat = std::make_shared<Material>(Shader::Find("Standard"));
	floorMat->SetTexture("diffuseMap", floorDiff);
	floorMat->SetTexture("normalMap", floorNorm);
	floorMat->SetTexture("maskMap", floorMask);

	// Pillar
	auto filarDiff = std::make_shared<Texture>(basePath + "filar/pillar_col.png");
	auto filarNorm = std::make_shared<Texture>(basePath + "filar/pillar_nrm.png");
	auto filarMask = std::make_shared<Texture>(basePath + "filar/pillar_arm.png");

	auto filarMat = std::make_shared<Material>(Shader::Find("Standard"));
	filarMat->SetTexture("diffuseMap", filarDiff);
	filarMat->SetTexture("normalMap", filarNorm);
	filarMat->SetTexture("maskMap", filarMask);

	// Windows
	auto winDiff = std::make_shared<Texture>(basePath + "okno/window_col.png");
	auto winNorm = std::make_shared<Texture>(basePath + "okno/window_nrm.png");
	auto winMask = std::make_shared<Texture>(basePath + "okno/window_arm.png");

	auto winMat = std::make_shared<Material>(Shader::Find("Standard"));
	winMat->SetTexture("diffuseMap", winDiff);
	winMat->SetTexture("normalMap", winNorm);
	winMat->SetTexture("maskMap", winMask);

	lvl0Renderer->materials.push_back(wall1Mat);
	lvl0Renderer->materials.push_back(filarMat);
	lvl0Renderer->materials.push_back(wall0Mat);
	lvl0Renderer->materials.push_back(floorMat);
	lvl0Renderer->materials.push_back(boxMat);
	lvl0Renderer->materials.push_back(winMat);
}

void LevelDescription::CreateLights()
{
	auto skybox = std::make_shared<TextureCube>();
	skybox->Load("resources/Textures/Skyboxes/sunflowers_polyhaven/.hdr");
	Renderer::SetSkybox(skybox);

	auto sunGO = new GameObject();
	auto sunLight = sunGO->AddComponent(new Light());
	sunLight->intensity = 1.0f;
	sunLight->color = { 1.0f, 0.97f, 0.9f };
	sunGO->GetTransform()->Position = { 0, 5, 0 };
	sunGO->GetTransform()->Rotation = { glm::radians(-45.0f), glm::radians(-10.0f), 0 };

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { -8.0f, 5.0f, 2.4f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.0f, 0.5f, 0.01f };
		light->type = LightType::Spot;
		light->intensity = 2.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { -8.0f, 5.0f, 11.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.0f, 0.15f, 0.01f };
		light->type = LightType::Spot;
		light->intensity = 10.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { 8.0f, 0.2f, 14.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 0.05f, 0.15f, 1.0f };
		light->type = LightType::Point;
		light->intensity = 4.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { 23.0f, 3.2f, 16.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.00f, 0.5f, 0.05f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { 24.0f, 8.2f, 5.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.00f, 0.5f, 0.05f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { 30.0f, 8.2f, -4.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.00f, 0.5f, 0.05f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	{
		auto lightGO = new GameObject();
		lightGO->GetTransform()->Position = { 50.0f, 3.2f, 10.0f };
		lightGO->GetTransform()->Rotation = { glm::radians(-90.0f), 0, 0 };
		auto light = lightGO->AddComponent(new Light());
		light->color = { 1.00f, 0.5f, 0.05f };
		light->type = LightType::Point;
		light->intensity = 1.0f;
	}

	/*for (size_t x = 0; x < 4; x++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			auto lightGO = new GameObject();
			lightGO->GetTransform()->Position = { x * 4.0f - 8, powf((x + y) / 16.0f, 4) / 4.0f + 0.1f, y * 4.0f - 8 };
			auto light = lightGO->AddComponent(new Light());
			light->color = { 0.5f * ((x + y) / 16.0f), ((x + y) / 16.0f), 1.0f };
			light->type = LightType::Point;
			light->intensity = 2.0f;
		}
	}*/
}

void LevelDescription::CreatePlayer()
{
	// Loading the weapon
	auto wDiff = std::make_shared<Texture>("resources/Models/gun/Gun_col.png");
	auto wNorm = std::make_shared<Texture>("resources/Models/gun/Gun_nrm.png");
	auto wMask = std::make_shared<Texture>("resources/Models/gun/Gun_arm.png");

	auto wMat = std::make_shared<Material>(Shader::Find("Standard"));
	wMat->SetTexture("diffuseMap", wDiff);
	wMat->SetTexture("normalMap", wNorm);
	wMat->SetTexture("maskMap", wMask);

	auto weaponRenderer = LoadAndCreate("resources/Models/gun/Gun.fbx");
	weaponRenderer->materials.push_back(wMat);
	weaponRenderer->materials.push_back(wMat);
	weaponRenderer->materials.push_back(wMat);
	weaponRenderer->GetGameObject()->GetTransform()->Position = { 0.0f, 1.0f, 0.0f };
	weaponRenderer->GetGameObject()->GetTransform()->Rotation = { 0.0f, glm::radians(90.0f), glm::radians(90.0f) };
	weaponRenderer->GetGameObject()->GetTransform()->Scale = {0.01f, 0.01f, 0.01f};

	// Player
	auto playerGO = new GameObject();
	auto player = playerGO->AddComponent(new Player());
	playerGO->GetTransform()->Position += glm::vec3(0, 1, 0);

	player->weapon = weaponRenderer->GetGameObject();
}

Enemy* LevelDescription::CreateEnemy(glm::vec3 pos)
{
	std::string basePath = "resources/Models/food_avocado_01_2k/";
	auto enemyRenderer = LoadAndCreate(basePath + "food_avocado_01_2k.fbx");
	enemyRenderer->GetGameObject()->GetTransform()->Position = pos;
	enemyRenderer->GetGameObject()->GetTransform()->FixedRotation = { glm::radians(-90.0f),0,0};
	enemyRenderer->GetGameObject()->GetTransform()->Scale = {20.0f, 20.0f, 20.0f};

	// Textures
	static auto diff = std::make_shared<Texture>(basePath + "textures/food_avocado_01_diff_2k.jpg");
	static auto norm = std::make_shared<Texture>(basePath + "textures/food_avocado_01_nor_gl_2k.jpg");
	static auto mask = std::make_shared<Texture>(basePath + "textures/food_avocado_01_arm_2k.jpg");

	auto enemyMaterial = std::make_shared<Material>(Shader::Find("Standard"));
	enemyMaterial->SetTexture("diffuseMap", diff);
	enemyMaterial->SetTexture("normalMap", norm);
	enemyMaterial->SetTexture("maskMap", mask);
	enemyRenderer->materials.push_back(enemyMaterial);

	auto enemy = new Enemy();
	enemy->collisionRays = 8;
	return enemyRenderer->GetGameObject()->AddComponent(enemy);
}

void LevelDescription::CreateCollision(CollisionController& collisionController)
{
	collisionController.colliders.push_back(new BoxCollider({ 3.0f,0,0 }, { 1,9,9 }, {0,0,0}));
	collisionController.colliders.push_back(new BoxCollider({ 0,0,-3.0f }, { 20,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 2.0f,0,3.0f }, { 9,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ -9.0f,0,0 }, { 1,9,32 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ -3.0f,0,6.5f }, { 1,9,8 }, { 0,0,0 }));

	glm::vec3 boxes = { 3.0f, 5.0f, 3.0f};
	collisionController.colliders.push_back(new BoxCollider({ -2.0f,0,11.0f }, boxes, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 7.0f,0,11.0f }, boxes, { 0,0,0 }));

	collisionController.colliders.push_back(new BoxCollider({ 5.0f,0,17.2f }, boxes * 1.1f, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 14.0f,0,19.0f }, boxes, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 26.0f,0,19.0f }, boxes, { 0,0,0 }));

	collisionController.colliders.push_back(new BoxCollider({ 20.0f,0,12.0f }, boxes * 1.8f, { 0,0,0 }));

	collisionController.colliders.push_back(new BoxCollider({ 9.5f,0,9.0f }, { 24,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 12.0f,0,21.0f }, { 30,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ -3.0f,0,18.0f }, { 1,9,7 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ -7.0f,0,15.0f }, { 9,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 27.0f,0,13.0f }, { 1,9,22 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 21.5f,0,-1.0f }, { 1,9,19 }, { 0,0,0 }));

	collisionController.colliders.push_back(new BoxCollider({ 30.5f,0,-9.2f }, { 27,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 31.0f,0,3.0f }, { 7,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 33.0f,0,1.0f }, { 1,9,9 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 35.0f,0,-3.0f }, { 5,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 39.0f,0,8.0f }, { 1,9,24 }, { 0,0,0 }));

	collisionController.colliders.push_back(new BoxCollider({ 45.0f,0,-7.0f }, { 1,9,9 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 56.0f,0,-3.0f }, { 25,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 50.0f,0, 15.0f }, { 35,9,1 }, { 0,0,0 }));
	collisionController.colliders.push_back(new BoxCollider({ 57.0f,0, 5.0f }, { 1,9,30 }, { 0,0,0 }));
}

MeshRenderer* LevelDescription::LoadAndCreate(const std::string& path)
{
	try
	{
		auto meshImporter = MeshImporter();
		auto model = meshImporter.Load(path);

		auto gameObject = new GameObject();
		auto meshRenderer = gameObject->AddComponent(new MeshRenderer());
		for (auto mesh : *model)
		{
			meshRenderer->meshes.push_back(mesh);
			//meshRenderer->materials.push_back(new Material(Shader::Find("Standard")));
		}

		return meshRenderer;
	}
	catch (const char* msg) {
		Debug::LogError("LevelDescription - LoadAndCreate(string)", msg);
	}

	return nullptr;
}
