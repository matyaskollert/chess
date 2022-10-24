#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Render.h"
#include "GameBoard.h"
int state = 0;


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("KoliChess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_W, WINDOW_SIZE_H, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	GameBoard board{};
	Render render{};

	render.renderMainMenu(renderer);
	//render.renderStartGame(renderer, board);

	int x, y;
	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (state) {
			case 0:
				switch (e.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					Uint32 buttons = SDL_GetMouseState(&x, &y);
					/*if (buttons & SDL_BUTTON_LMASK) {
						render.checkForInput(x, y, board, renderer);
						SDL_Delay(150);
					}*/
					break;

				}
				break;
			case 1:
				switch (e.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					Uint32 buttons = SDL_GetMouseState(&x, &y);
					if (buttons & SDL_BUTTON_LMASK) {
						render.checkGameInput(x, y, board, renderer);
						SDL_Delay(150);
					}
					break;

				}
				break;
			default:
				break;
			}

		}
	}

	SDL_Delay(3000);

	SDL_Quit();

	return 0;
}