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
#include "RotatorComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = new dae::GameObject{};

	scene.AddGameObject(go);

	auto go2 = new dae::GameObject{};
	scene.AddGameObject(go2);

	auto go3 = new dae::GameObject{};
	scene.AddGameObject(go3);

	go3->SetParent(go2);
	go2->SetParent(go);

	scene.RemoveGameObject(go2, true);

	//auto gameObject = new dae::GameObject{};
	//gameObject->AddComponent<ody::TransformComponent>(0.f, 0.f);
	//gameObject->AddComponent<ody::TextureComponent>("background.tga");
	//scene.AddGameObject(gameObject);

	//gameObject = new dae::GameObject{};
	//gameObject->AddComponent<ody::TransformComponent>(216.f, 180.f);
	//gameObject->AddComponent<ody::TextureComponent>("logo.tga");
	//scene.AddGameObject(gameObject);

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = new dae::GameObject{};
	//to->AddComponent<ody::TransformComponent>(80.f, 20.f);
	//to->AddComponent<ody::TextComponent>("Programming 4 Assignment", font);
	//scene.AddGameObject(to);

	//gameObject = new dae::GameObject{};
	//gameObject->AddComponent<ody::TextComponent>("0", font);
	//gameObject->AddComponent<ody::TransformComponent>(10.f, 10.f);
	//gameObject->AddComponent<ody::FPSCounter>();
	//scene.AddGameObject(gameObject);
	//
	//auto pacman = new dae::GameObject{};
	//pacman->AddComponent<ody::TransformComponent>(0.f, 0.f);
	//pacman->AddComponent<ody::TextureComponent>("pacman.tga");
	//pacman->AddComponent<ody::RotatorComponent>();

	//auto ghost = new dae::GameObject{};
	//ghost->AddComponent<ody::TransformComponent>(100.f, 100.f);
	//ghost->AddComponent<ody::TextureComponent>("ghost.tga");
	//ghost->AddComponent<ody::RotatorComponent>();

	//ghost->SetParent(pacman);

	//scene.AddGameObject(pacman);
	//scene.AddGameObject(ghost);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}