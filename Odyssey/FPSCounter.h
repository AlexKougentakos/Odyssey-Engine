#pragma once
#include <memory>

#include "TextComponent.h"
#include "Component.h"

#include <iostream>

namespace ody
{

	class FPSCounter final : public Component
	{
	public:
		FPSCounter() = default;

		virtual ~FPSCounter() = default;
		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		virtual void Update(float deltaTime)
		{
			auto textComponent{ m_Owner->GetComponent<TextComponent>() };
			
			m_ElapsedSec += deltaTime;

			if (m_ElapsedSec >= m_UpdateInverval)
			{
				m_ElapsedSec -= m_UpdateInverval;
				
				int FPS{ static_cast<int>(1 / deltaTime) };
				textComponent->SetText(std::to_string(FPS));
			}

		}
		virtual void Render() const
		{

		}
	private:
		float m_ElapsedSec{};
		const float m_UpdateInverval{ 0.5f };
	};
}