#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Render.h"
#include "GameBoard.h"
#include "Common.h"

SDL_Window* WINDOW;
SDL_Renderer* RENDERER;
State STATE = MENU;
GameBoard BOARD{};
Render RENDER{};


void handleGameStates()
{
	bool running = true;
	int x, y;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
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
				switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					Uint32 buttons = SDL_GetMouseState(&x, &y);
					if (buttons & SDL_BUTTON_LMASK) {
						RENDER.checkGameInput(x, y, BOARD, RENDERER);
						SDL_Delay(150);
					}
					break;

				}
				break;
			case EXIT:
				running = false;
			default:
				break;
			}

		}
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	WINDOW = SDL_CreateWindow("KoliChess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_W, WINDOW_SIZE_H, SDL_WINDOW_SHOWN);
	RENDERER = SDL_CreateRenderer(WINDOW, -1, 0);

	RENDER.renderMainMenuFirstTime(RENDERER);
	handleGameStates();

	SDL_Quit();
	return 0;
}