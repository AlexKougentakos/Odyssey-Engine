#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include <Windows.h>
#include <Xinput.h>

bool dae::InputManager::ProcessInput()
{

	


	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...

		//process input for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}
