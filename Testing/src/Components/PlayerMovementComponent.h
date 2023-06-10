#pragma once
#include "Component.h"
#include "SceneManager.h"

namespace ody
{
	class RigidBodyComponent;
}

struct PlayerMovementSettings
{
	float speed = 1.0f;
	float acceleration = 1.0f;
	float jumpForce = 1.0f;
};

class PlayerMovementComponent : public ody::Component
{
public:
	PlayerMovementComponent(const ody::RigidBodyComponent* rigidBody);
	~PlayerMovementComponent() override = default;
	PlayerMovementComponent(const PlayerMovementComponent& other) = delete;
	PlayerMovementComponent(PlayerMovementComponent&& other) = delete;
	PlayerMovementComponent& operator=(const PlayerMovementComponent& other) = delete;
	PlayerMovementComponent& operator=(PlayerMovementComponent&& other) = delete;

	void Update() override;
	void Jump();

private:
	const ody::RigidBodyComponent* m_pRigidBodyComponent{};

	bool m_HasJumped{};
	bool m_IsGrounded{};
};