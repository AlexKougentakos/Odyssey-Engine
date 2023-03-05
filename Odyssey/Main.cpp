#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Odyssey.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSCounter.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<ody::TransformComponent>(0.f, 0.f);
	gameObject->AddComponent<ody::TextureComponent>("background.tga");
	scene.Add(gameObject);

	//gameObject->GetComponent<TransformComponent>()->SetPosition(10.f,10.f,10.f);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<ody::TransformComponent>(216.f, 180.f);
	gameObject->AddComponent<ody::TextureComponent>("logo.tga");
	scene.Add(gameObject);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<ody::TransformComponent>(80.f, 20.f);
	to->AddComponent<ody::TextComponent>("Programming 4 Assignment", font);
	scene.Add(to);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<ody::TextComponent>("0", font);
	gameObject->AddComponent<ody::TransformComponent>(10.f, 10.f);
	gameObject->AddComponent<ody::FPSCounter>();
	scene.Add(gameObject);


	gameObject = std::make_shared<dae::GameObject>();
	
	auto gameObject2 = std::make_shared<dae::GameObject>();
	gameObject2->SetParent(gameObject);

	auto gameObject3 = std::make_shared<dae::GameObject>();
	gameObject2->SetParent(gameObject3);


}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}