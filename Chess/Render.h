#pragma once
#include <vector>
#include "SDL.h"
#include "GameBoard.h"

constexpr int WINDOW_SIZE_W = 1200;
constexpr int WINDOW_SIZE_H = 1000;

class Render {
public:
	int updateBoard(SDL_Renderer* renderer, GameBoard& board);
	int checkGameInput(int x, int y, GameBoard& board, SDL_Renderer* renderer);
	int renderStartGame(SDL_Renderer* renderer, GameBoard& board);
	void renderMainMenu(SDL_Renderer* renderer);
private:
	int convertCharToIndex(char piece);
	void renderBackBoard(SDL_Renderer* renderer);
	void renderPieces(SDL_Renderer* renderer, GameBoard& board);
	std::vector<SDL_Texture*> m_pieces;
	SDL_Texture* m_board;
};

