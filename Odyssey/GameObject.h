#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Component.h"

#define NO_PARENT nullptr

namespace ody
{
	class Component;
}
namespace dae
{
	class Texture2D;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime);
		void Render() const;

		size_t GetChildCount() const;
		std::shared_ptr<GameObject> GetChildAt(unsigned int index);
		const std::vector<dae::GameObject*>& GetChildren() const;

		std::shared_ptr<GameObject> GetParent() const;
		void SetParent(std::shared_ptr<GameObject> newParent);

		template <typename T, typename... Targs>
		T& AddComponent(Targs&&... args)
		{
			T* newComponent(new T(std::forward<Targs>(args)...));

			newComponent->SetOwner(this);
			m_Components.emplace_back(newComponent);
			return *newComponent;
		}

		template<typename  T>
		inline std::shared_ptr<T> GetComponent() const
		{
			static_assert(std::is_base_of<ody::Component, T>(), "T needs to be derived from the Component class");

			for (const std::shared_ptr<ody::Component>& pComponent : m_Components)
			{
				std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

				if (derivedComponent) return derivedComponent;
			}

			return nullptr;
		}

		template<typename T>
		inline bool RemoveComponent() //Todo test this function
		{
			static_assert(std::is_base_of<ody::Component, T>(), "T needs to be derived from the Component class");

			// Find the first component of type T in the vector
			auto it = std::find_if(m_Components.begin(), m_Components.end(), [](const std::shared_ptr<ody::Component>& component)
				{
					return dynamic_cast<const T*>(component.get()) != nullptr;
				});

			// If a component of type T was found, remove it from the vector and return true
			if (it != m_Components.end())
			{
				m_Components.erase(it);
				return true;
			}
			return false;
		}

	private:
		std::vector<std::shared_ptr<ody::Component>> m_Components{};
		//std::vector <std::weak_ptr<GameObject>> m_pChildren{};
		std::vector<GameObject*> m_pChildren{};

		std::shared_ptr<GameObject> m_pParent{ NO_PARENT };

		bool RemoveChild(unsigned int index);
		bool RemoveChild(std::shared_ptr<dae::GameObject> child);
		void AddChild(const std::shared_ptr<dae::GameObject> gameObject);
	};
}