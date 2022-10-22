#include <vector>
#include <SDL.h>
#include <SDL_image.h>

std::vector<SDL_Texture*> loadPieces(SDL_Renderer* renderer)
{
	std::vector<SDL_Texture*> pieces;
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhPa.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhKn.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhBi.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhRo.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhQu.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/WhKi.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlPa.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlKn.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlBi.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlRo.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlQu.png"));
	pieces.push_back(IMG_LoadTexture(renderer, "./Textures/BlKi.png"));
	return pieces;
}

SDL_Texture* loadBoard(SDL_Renderer* renderer)
{
	return IMG_LoadTexture(renderer, "./Textures/board.jpg");
}
