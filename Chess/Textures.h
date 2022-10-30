#pragma once
#include <vector>
#include "SDL.h";

SDL_Texture* loadMenu(SDL_Renderer* renderer);
SDL_Texture* loadBoard(SDL_Renderer* renderer);

std::vector<SDL_Texture*> loadPieces(SDL_Renderer* renderer);