#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Render.h"
#include "GameBoard.h"
constexpr int WINDOW_SIZE = 1000;


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	GameBoard board{};
	Render render{};

	render.renderStartGame(renderer, board);

	while (true) {
		int x, y;
		SDL_PumpEvents();
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		if (buttons & SDL_BUTTON_LMASK) {
			render.checkForInput(x, y, board, renderer);
			SDL_Delay(150);
		}
	}

	SDL_Delay(3000);

	SDL_Quit();

	return 0;
}