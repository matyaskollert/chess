#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Render.h"
#include "GameBoard.h"
#include "Common.h"
#include "GameEngine.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


SDL_Window* WINDOW;
SDL_Renderer* RENDERER;
State STATE = MENU;
GameBoard BOARD{};
Render RENDER{};
GameEngine m_engine{};

bool ENABLE_AI = true;
bool PLAYER_WHITE = true;
bool AI_THINKING = false;

void handleGameEvents(SDL_Event& e, int& x, int& y) {

	if ((BOARD.m_whiteMove == PLAYER_WHITE || !ENABLE_AI) && e.type == SDL_MOUSEBUTTONDOWN) {
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		if (buttons & SDL_BUTTON_LMASK) {
			RENDER.checkGameInput(x, y, BOARD, RENDERER);
			SDL_Delay(150);
		}
	}
	else if (ENABLE_AI && BOARD.m_whiteMove == !PLAYER_WHITE && !AI_THINKING) {
		m_engine.getBestMovePromise(BOARD);
		AI_THINKING = true;
	}
	else if (ENABLE_AI && BOARD.m_whiteMove == !PLAYER_WHITE && AI_THINKING) {
		if (m_engine.ready) {
			const auto pp = m_engine.getBestMove();
			BOARD.checkForInput(pp.x.x, pp.x.y);
			BOARD.checkForInput(pp.y.x, pp.y.y);
			if (BOARD.m_blackPromotion) {
				BOARD.checkForInput(0, 0);
			}
			AI_THINKING = false;
		}
	}
	RENDER.updateBoard(RENDERER, BOARD);
}


void handleGameStates()
{
	bool running = true;
	int x, y;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {

			ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT) {
				STATE = EXIT;
			}
			switch (STATE) {
			case MENU:
				switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					Uint32 buttons = SDL_GetMouseState(&x, &y);
					if (buttons & SDL_BUTTON_LMASK) {
						RENDER.checkMenuInput(x, y, RENDERER, STATE, BOARD);
						SDL_Delay(150);
					}
					break;

				}
				break;
			case GAME:
				SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
				SDL_RenderClear(RENDERER);
				handleGameEvents(e, x, y);
				break;
			case EXIT:
				running = false;
			default:
				break;
			}
		}

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Main GUI");

		ImGui::Checkbox("Emable AI", &ENABLE_AI);
		ImGui::Checkbox("Player White", &PLAYER_WHITE);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(RENDERER);
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	WINDOW = SDL_CreateWindow("KoliChess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_W, WINDOW_SIZE_H, SDL_WINDOW_SHOWN);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(WINDOW, RENDERER);
	ImGui_ImplSDLRenderer_Init(RENDERER);

	RENDER.renderMainMenuFirstTime(RENDERER);
	handleGameStates();

	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_Quit();
	return 0;
}