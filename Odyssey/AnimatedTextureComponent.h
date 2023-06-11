﻿#pragma once
#include "Component.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "glm/glm.hpp"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include "Time.h"

namespace ody
{
	class AnimatedTextureComponent final : public Component
	{
	public:
		AnimatedTextureComponent() = default;

		/**
		 * \brief Adds sprite sheet animations
		 * \param filename The name of the sprite sheet
		 * \param spriteCounts Number of columns are rows in the sprite sheet
		 * \param animationSpeed The amount of seconds between each frame
		 */
		AnimatedTextureComponent(const std::string& filename, const glm::ivec2& spriteCounts, float animationSpeed, float scale = 1.f) :
			Component()
		{
			m_pTexture = ody::ResourceManager::GetInstance().LoadTexture(filename);
			m_SpriteCounts = spriteCounts;
			m_AnimationSpeed = animationSpeed;
			m_FrameSize = { m_pTexture->GetSize().x / spriteCounts.x, m_pTexture->GetSize().y / spriteCounts.y };
			m_Scale = scale;
		}

		~AnimatedTextureComponent() override = default;
		AnimatedTextureComponent(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent(AnimatedTextureComponent&& other) = delete;
		AnimatedTextureComponent& operator=(const AnimatedTextureComponent& other) = delete;
		AnimatedTextureComponent& operator=(AnimatedTextureComponent&& other) = delete;

		void SetAnimationSpeed(float speed)
		{
			m_AnimationSpeed = speed;
		}

		virtual void Update() override;

		virtual void Render() const override
		{
			const auto pTransformComponent{ GetOwner()->GetComponent<TransformComponent>() };
			const glm::vec3 renderPosition{ pTransformComponent->GetWorldPosition() };
			const glm::vec2 sourcePos{ m_FrameSize.x* m_CurrentFrame.x, m_FrameSize.y* m_CurrentFrame.y };
			ody::Renderer::GetInstance().RenderTexture(*m_pTexture, renderPosition.x, renderPosition.y, m_FrameSize.x, m_FrameSize.y, sourcePos.x, sourcePos.y, m_Scale);
		}


	private:
		std::shared_ptr<ody::Texture2D> m_pTexture{};
		glm::ivec2 m_SpriteCounts{};
		glm::ivec2 m_CurrentFrame{};
		glm::vec2 m_FrameSize{};
		float m_AnimationSpeed{};
		float m_AnimationTime{};
		float m_Scale{};
	};
}
