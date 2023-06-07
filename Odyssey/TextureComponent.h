#pragma once
#include "Component.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "glm/glm.hpp"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <memory>
#include <string>

namespace ody
{
	class TextureComponent final : public Component
	{
	public:
		TextureComponent() = default;
		TextureComponent(const std::string& filename) :
			Component()
		{
			m_pTexture = ody::ResourceManager::GetInstance().LoadTexture(filename);
		}

		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		glm::vec2 GetTextureSize() const
		{
			return m_pTexture->GetSize();
		}

		void SetTexture(std::shared_ptr<ody::Texture2D> pTexture)
		{
			m_pTexture = pTexture;
		}

		void SetTexture(const std::string& filename)
		{
			m_pTexture = ody::ResourceManager::GetInstance().LoadTexture(filename);
		}

		virtual void Update() override
		{

		}
		virtual void Render() const override
		{
			const auto pTransformComponent{ GetOwner()->GetComponent<TransformComponent>()};
			glm::vec3 renderPosition{ pTransformComponent->GetWorldPosition() };

			ody::Renderer::GetInstance().RenderTexture(*m_pTexture, renderPosition.x, renderPosition.y);
		}

	private:
		std::shared_ptr<ody::Texture2D> m_pTexture{};

	};
}