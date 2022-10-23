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

int Render::updateBoard(SDL_Renderer* renderer, GameBoard& board)
{
	SDL_Rect destPiece;
	SDL_Rect destValid;
	destPiece.x = BOARD_BORDER_SIZE + PIECE_SIZE / 2;
	destPiece.y = BOARD_BORDER_SIZE + PIECE_SIZE / 2;
	destPiece.h = PIECE_SIZE;
	destPiece.w = PIECE_SIZE;
	destValid.h = PIECE_SIZE;
	destValid.w = PIECE_SIZE;

	SDL_RenderCopy(renderer, m_board, NULL, NULL);
	char pieceChar;

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
				SDL_RenderFillRect(renderer, &destValid);
			}
			if (n == -1) {
				continue;
			}

			SDL_RenderCopy(renderer, m_pieces[n], NULL, &destPiece);

		}
	}
	SDL_RenderPresent(renderer);
	return 0;
}

int Render::renderStartGame(SDL_Renderer* renderer, GameBoard& board)
{
	m_board = loadBoard(renderer);
	m_pieces = loadPieces(renderer);
	updateBoard(renderer, board);
	return 0;
}



int Render::checkForInput(int x, int y, GameBoard& board, SDL_Renderer* renderer) {
	//mouse inside the board
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
		char pieceChar = board.checkForInput(col, row);
		updateBoard(renderer, board);
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
