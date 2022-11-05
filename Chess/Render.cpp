#include <iostream>
#include "Render.h"
#include "Textures.h"
#include "GameBoard.h"

constexpr int PIECE_SIZE = 60;
constexpr int BOARD_SIZE = 1000;
constexpr int BOARD_BORDER_SIZE = 36;
constexpr int BOARD_SKIP = (BOARD_SIZE - (BOARD_BORDER_SIZE * 2)) / 8;
constexpr int BOARD_X = 0;
constexpr int BOARD_Y = 0;
constexpr bool WHITE_DOWN = true;

constexpr SDL_Rect START_BUTTON_SRC = { 0,0,210,56 };
constexpr SDL_Rect START_BUTTON_DEST = { WINDOW_SIZE_W / 2 - (210 / 2),WINDOW_SIZE_H / 2 - (56 / 2) - 50,210,56 };
constexpr SDL_Rect EXIT_BUTTON_SRC = { START_BUTTON_SRC.x, START_BUTTON_SRC.h, START_BUTTON_SRC.w, START_BUTTON_SRC.h };
constexpr SDL_Rect EXIT_BUTTON_DEST = { START_BUTTON_DEST.x, START_BUTTON_DEST.y + 100, START_BUTTON_DEST.w, START_BUTTON_DEST.h };

Render::Render() {
	m_engine = {};
}

void Render::renderMainMenuFirstTime(SDL_Renderer* renderer)
{
	m_menuTexture = loadMenu(renderer);
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, m_menuTexture, &START_BUTTON_SRC, &START_BUTTON_DEST);

	SDL_RenderCopy(renderer, m_menuTexture, &EXIT_BUTTON_SRC, &EXIT_BUTTON_DEST);

	SDL_RenderPresent(renderer);
}

void Render::renderMainMenu(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	SDL_RenderClear(renderer);

	SDL_Rect src{};
	src.w = 210;
	src.h = 56;
	src.x = 0;
	src.y = 0;

	SDL_Rect dest{};
	dest.w = 210;
	dest.h = 56;
	dest.x = WINDOW_SIZE_W / 2 - (dest.w / 2);
	dest.y = WINDOW_SIZE_H / 2 - (dest.h / 2) - 50;

	SDL_RenderCopy(renderer, m_menuTexture, &src, &dest);

	src.y = src.h;
	dest.y = dest.y + 100;

	SDL_RenderCopy(renderer, m_menuTexture, &src, &dest);

	SDL_RenderPresent(renderer);
}

void Render::checkMenuInput(int x, int y, SDL_Renderer* renderer, State& state, GameBoard& board)
{
	if (x >= START_BUTTON_DEST.x && x <= START_BUTTON_DEST.x + START_BUTTON_DEST.w && y >= START_BUTTON_DEST.y && y <= START_BUTTON_DEST.y + START_BUTTON_DEST.h) {
		state = GAME;
		renderStartGame(renderer, board);
		std::cout << "Pressed the start button\n";
	}
	else if (x >= EXIT_BUTTON_DEST.x && x <= EXIT_BUTTON_DEST.x + EXIT_BUTTON_DEST.w && y >= EXIT_BUTTON_DEST.y && y <= EXIT_BUTTON_DEST.y + EXIT_BUTTON_DEST.h) {
		state = EXIT;
		std::cout << "Pressed the exit button\n";
	}
}

int Render::updateBoard(SDL_Renderer* renderer, GameBoard& board)
{
	renderBackBoard(renderer);
	renderPieces(renderer, board);
	SDL_RenderPresent(renderer);
	return 0;
}

int Render::renderStartGame(SDL_Renderer* renderer, GameBoard& board)
{
	m_board = loadBoard(renderer);
	m_pieces = loadPieces(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	updateBoard(renderer, board);
	return 0;
}

int Render::checkGameInput(int x, int y, GameBoard& board, SDL_Renderer* renderer) {
	//mouse outside the board
	if (x < BOARD_X + BOARD_BORDER_SIZE || x > BOARD_X + BOARD_SIZE - BOARD_BORDER_SIZE || y < BOARD_Y + BOARD_BORDER_SIZE || y > BOARD_Y + BOARD_SIZE - BOARD_BORDER_SIZE) {

	}
	else {
		int col = ((x - BOARD_BORDER_SIZE) / BOARD_SKIP);
		int row = ((y - BOARD_BORDER_SIZE) / BOARD_SKIP);
		if (WHITE_DOWN) {
			row = 7 - row;
		}
		else {
			col = 7 - col;
		}
		char pieceChar = board.checkForInput(row, col);
		updateBoard(renderer, board);
		//m_engine.getEvaluation(board);
		if (!board.m_whiteMove) {
			PairPair pp = m_engine.getBestMove(board);
			board.checkForInput(pp.x.x, pp.x.y);
			board.checkForInput(pp.y.x, pp.y.y);
			if (board.m_blackPromotion) {
				board.checkForInput(0, 0);
			}
			updateBoard(renderer, board);
		}

		/*int n = convertCharToIndex(pieceChar);
		SDL_Rect dest;
		dest.x = x - PIECE_SIZE / 2;
		dest.y = y - PIECE_SIZE / 2;
		dest.w = PIECE_SIZE;
		dest.h = PIECE_SIZE;
		SDL_RenderCopy(renderer, m_pieces[n], NULL, &dest);
		SDL_RenderPresent(renderer);*/
	}
	return 0;
}

void Render::renderBackBoard(SDL_Renderer* renderer) {
	SDL_Rect boardDest{};
	boardDest.x = BOARD_X;
	boardDest.y = BOARD_Y;
	boardDest.w = BOARD_SIZE;
	boardDest.h = BOARD_SIZE;

	SDL_RenderCopy(renderer, m_board, NULL, &boardDest);
}

void Render::renderPieces(SDL_Renderer* renderer, GameBoard& board) {
	char pieceChar;

	SDL_Rect destPiece;
	SDL_Rect destValid;
	SDL_Rect destLastMove;
	destPiece.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2;
	destPiece.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2;
	destPiece.h = PIECE_SIZE;
	destPiece.w = PIECE_SIZE;
	destValid.h = PIECE_SIZE;
	destValid.w = PIECE_SIZE;
	destLastMove.h = PIECE_SIZE;
	destLastMove.w = PIECE_SIZE;

	if (board.m_lastMove.col != -1) {
		if (WHITE_DOWN) {
			destLastMove.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * board.m_lastMove.col;
			destLastMove.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * (7 - board.m_lastMove.row);
		}
		else {
			destLastMove.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * (7 - board.m_lastMove.col);
			destLastMove.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * board.m_lastMove.row;
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderDrawRect(renderer, &destLastMove);
	}

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			destPiece.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * col;
			destPiece.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * row;
			if (WHITE_DOWN) {
				pieceChar = board.m_boardArray[7 - row][col];
				destValid.x = destPiece.x;
				destValid.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * (7 - row);
			}
			else {
				pieceChar = board.m_boardArray[row][7 - col];
				destValid.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2 + BOARD_SKIP * (7 - col);
				destValid.y = destPiece.y;
			}
			int n = convertCharToIndex(pieceChar);

			if (board.m_selectedFigure != 0 && board.m_validMoveArray[board.m_selectedSquare.row][board.m_selectedSquare.col][row][col]) {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_RenderDrawRect(renderer, &destValid);

			}
			if (n == -1) {
				continue;
			}

			SDL_RenderCopy(renderer, m_pieces[n], NULL, &destPiece);

		}
	}
}

int Render::convertCharToIndex(char piece) {
	switch (piece) {
	case 'P':
		return 0;
	case 'N':
		return 1;
	case 'B':
		return 2;
	case 'R':
		return 3;
	case 'Q':
		return 4;
	case 'K':
		return 5;
	case 'p':
		return 6;
	case 'n':
		return 7;
	case 'b':
		return 8;
	case 'r':
		return 9;
	case 'q':
		return 10;
	case 'k':
		return 11;
	default:
		return -1;
	}
}
