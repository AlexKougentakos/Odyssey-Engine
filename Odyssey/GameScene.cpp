#include "GameScene.h"
#include "GameTime.h"
#include "GameObject.h"

#include <Box2D/Box2D.h>

#include "CircleColliderComponent.h"
#include "ColliderComponent.h"
#include "DebugRenderer.h"
#include "IPrefab.h"
#include "RigidBodyComponent.h"
#include "Utils.h"
#include <imgui.h>
#include <iostream>

#include "ImGuiManager.h"
#include "PhysicsManager.h"

namespace ody
{
GameScene::GameScene(std::wstring sceneName)
	:m_SceneName(std::move(sceneName))
{
	
}

void GameScene::AddChild_Safe(GameObject* object)
{
	// Check if the object already exists in the array
	const auto it = std::find_if(m_pChildren.begin(), m_pChildren.end(), [object](const std::unique_ptr<GameObject>& ptr) 
		{
		return ptr.get() == object;
		});

	// If the object doesn't exist in the array, add it
	if (it == m_pChildren.end()) {
		m_pChildren.emplace_back(std::unique_ptr<GameObject>(object));
	}
}

//This function will place the children of the object, if any, to the parent above
void GameScene::RemoveChild(GameObject* object, bool keepChildren)
{
	// If we need to remove the children call the same function on the children
	if (!keepChildren)
	{
		for (auto it = m_pChildren.begin(); it != m_pChildren.end(); )
		{
			auto& p = *it;
			if (p->GetParent() == object)
			{
				RemoveChild(p.get(), false);
				it = m_pChildren.begin(); // start over, as the vector has been modified
			}
			else
			{
				++it;
			}
		}
	}


	if (const auto parent = object->GetParent())
	{
		// Transfer children to parent (if any)
		auto& children = parent->GetChildren();
		auto& object_children = object->GetChildren();
		children.insert(children.end(), make_move_iterator(object_children.begin()), make_move_iterator(object_children.end()));
		object_children.clear();

		// Remove object from its parent's list of children
		children.erase(std::remove(children.begin(), children.end(), object), children.end());
	}

	// Remove object from the scene
	auto& objects = m_pChildren;
	objects.erase(std::remove_if(objects.begin(), objects.end(),
		[object](const auto& p) { return p.get() == object; }), objects.end());
}

GameObject* GameScene::CreateGameObject()
{
	auto gameObject = new GameObject(this);
	gameObject->Initialize();

	m_pChildren.emplace_back(std::move(gameObject));

	return gameObject;
}

GameObject* GameScene::CreateGameObjectFromPrefab(const IPrefab& prefab)
{
	auto gameObject = new GameObject(this);
	gameObject->Initialize();
	//todo: test prefabs
	prefab.Configure(gameObject);

	m_pChildren.emplace_back(std::move(gameObject));

	return gameObject;
}


void GameScene::OnRootSceneActivated()
{
	m_pWorld = new b2World(b2Vec2(0.f, Utils::PixelsToMeters(100.f)));
	m_pWorld->SetDebugDraw(&DebugRenderer::GetInstance());
	DebugRenderer::GetInstance().SetWorld(m_pWorld);
	PhysicsManager::GetInstance().SetPhysicsWorld(m_pWorld);

	for (const auto& object : m_pChildren)
	{
		if (!object->GetComponent<RigidBodyComponent>()) continue;

		const auto transform = object->GetTransform();
		const auto rigidBody = object->GetComponent<RigidBodyComponent>();

		b2BodyDef bodyDef{};
		Utils::RigidbodySettingsToB2DBodyDef(rigidBody->GetSettings(), bodyDef);
		bodyDef.position.Set(Utils::PixelsToMeters(transform->GetWorldPosition().x), Utils::PixelsToMeters(transform->GetWorldPosition().y));

		b2Body* pBody = m_pWorld->CreateBody(&bodyDef);
		rigidBody->SetRuntimeBody(pBody);
		pBody->SetFixedRotation(bodyDef.fixedRotation);

		//Colliders 
		const auto boxCollider = object->GetComponent<ody::ColliderComponent>();
		const auto circleCollider = object->GetComponent<ody::CircleColliderComponent>();

		if (!boxCollider && !circleCollider) continue;

		b2CircleShape circleShape{};
		b2PolygonShape boxShape{};
		b2FixtureDef fixtureDef{};
		b2Vec2 center;

		if (circleCollider)
		{
			// Circle Collider
			circleShape.m_radius = Utils::PixelsToMeters(circleCollider->GetRadius()); // Set the radius of the circle
			center = b2Vec2{ Utils::PixelsToMeters(circleCollider->GetRadius()), Utils::PixelsToMeters(circleCollider->GetRadius()) };

			circleShape.m_p = center; // Set the center of the circle
			fixtureDef.shape = &circleShape;

			Utils::ColliderSettingsToB2DFixtureDef(circleCollider->GetSettings(), fixtureDef);
		}
		else
		{
			// Box Collider
			center = b2Vec2{ Utils::PixelsToMeters(boxCollider->GetDimensions().x), Utils::PixelsToMeters(boxCollider->GetDimensions().y) };

			boxShape.SetAsBox(Utils::PixelsToMeters(boxCollider->GetDimensions().x), Utils::PixelsToMeters(boxCollider->GetDimensions().y), center, 0.f);

			fixtureDef.shape = &boxShape;

			Utils::ColliderSettingsToB2DFixtureDef(boxCollider->GetSettings(), fixtureDef);
		}

		b2FixtureUserData userData{};
		userData.pointer = reinterpret_cast<uintptr_t>(object.get());
		fixtureDef.userData = userData;

		b2Fixture* pFixture = pBody->CreateFixture(&fixtureDef);

		// Update the runtime fixture for both ColliderComponent and CircleColliderComponent
		if (boxCollider)
		{
			boxCollider->SetRuntimeFixture(pFixture);
			boxCollider->InitializeFilter();
		}
		else
		{
			circleCollider->SetRuntimeFixture(pFixture);
			circleCollider->InitializeFilter();
		}
	}

	OnSceneActivated();
}

void GameScene::RootRender()
{

}


void GameScene::OnRootSceneDeactivated()
{
	OnSceneDeactivated();

	delete m_pWorld;
	m_pWorld = nullptr;
}

void GameScene::FixedUpdate()
{
	//Physics
	constexpr int32_t velocityIterations = 8;
	constexpr int32_t positionIterations = 3;

	constexpr float calculationHz = 60.f;
	constexpr float ts = 1.f / calculationHz;

	m_pWorld->Step(ts, velocityIterations, positionIterations);
	ImGuiManager::GetInstance().AddCheckBox("Debug Draw Box2D", true);
	if (ImGuiManager::GetInstance().GetCheckBoxState("Debug Draw Box2D"))
		m_pWorld->DebugDraw();

	for (const auto& object : m_pChildren)
	{
		if (!object->GetComponent<RigidBodyComponent>()) continue;

		const auto transform = object->GetTransform();
		const auto rigidBody = object->GetComponent<RigidBodyComponent>();

		const b2Body* body = rigidBody->GetRuntimeBody();

		const auto& position = body->GetPosition();

		transform->Translate(Utils::MetersToPixels(position.x), Utils::MetersToPixels(position.y));
		//todo: rotate too
	}
}

void GameScene::RootUpdate()
{

	Update();
}

void GameScene::RootOnGUI()
{
	ImGui::Begin("Debug Window");
	OnGUI();
	ImGui::End();
}

}
