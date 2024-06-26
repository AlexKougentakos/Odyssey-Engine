#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <SDL_ttf.h>

#include "Component.h"
#include "TransformComponent.h"
#include "Font.h"
#include "Texture2D.h"
#include "glm/glm.hpp"
#include "Renderer.h"
#include "GameObject.h"

namespace ody
{
	class TextComponent final : public Component
	{
	public:
		TextComponent() = default;
		TextComponent(const std::string& text, const std::string& fontPath, unsigned int size) :
			Component()
		{
			m_Font = ody::ResourceManager::GetInstance().LoadFont(fontPath, size);
			m_Text = text;
		}

		TextComponent(const std::string& text, std::shared_ptr<ody::Font> font) :
			Component()
		{
			m_Font = font;
			m_Text = text;
		}

		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void SetPosition(glm::vec2 position)
		{
			m_Position = position;
		}

		void SetPosition(float x, float y)
		{
			m_Position.x = x;
			m_Position.y = y;

		}

		void SetText(const std::string& text)
		{
			m_Text = text;
			m_NeedsUpdate = true;
		}

		virtual void Update() override
		{
			if (m_NeedsUpdate)
			{
				constexpr SDL_Color color = { 255,255,255, 255 }; // only white text is supported now
				const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
				if (surf == nullptr)
				{
					throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
				}
				auto texture = SDL_CreateTextureFromSurface(ody::Renderer::GetInstance().GetSDLRenderer(), surf);
				if (texture == nullptr)
				{
					throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
				}
				SDL_FreeSurface(surf);
				m_TextTexture = std::make_shared<ody::Texture2D>(texture);
				m_NeedsUpdate = false;
			}
		}

		virtual void Render() const override
		{
			if (m_TextTexture != nullptr)
			{
				const auto pTransformComponent{ GetOwner()->GetComponent<TransformComponent>() };
				glm::vec3 renderPosition{ pTransformComponent->GetWorldPosition() };

				ody::Renderer::GetInstance().RenderTexture(*m_TextTexture, renderPosition.x, renderPosition.y);
			}
		}

	private:
		glm::vec2 m_Position{};
		std::string m_Text{};

		std::shared_ptr<ody::Font> m_Font;
		std::shared_ptr<ody::Texture2D> m_TextTexture;

		bool m_NeedsUpdate{true};

	};
}