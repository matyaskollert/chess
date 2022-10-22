#pragma once
#include <vector>
#include "SDL.h"
#include "GameBoard.h"

class Render {
public:
	int updateBoard(SDL_Renderer* renderer, GameBoard& board);
	int checkForInput(int x, int y, GameBoard& board, SDL_Renderer* renderer);
	int renderStartGame(SDL_Renderer* renderer, GameBoard& board);
private:
	int convertCharToIndex(char piece);
	std::vector<SDL_Texture*> m_pieces;
	SDL_Texture* m_board;
};

